#include "Context.hh"
#include "Driver.hh"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include <iostream>

int main(int argc, char **argv) {
  Driver drv;

  for (int i = 1; i < argc; ++i) {
    if (argv[i] == std::string("-p")) {
      drv.trace_parsing = true;
    } else if (argv[i] == std::string("-s")) {
      drv.trace_scanning = true;
    } else if (argv[i] == std::string("--dump-ast")) {
      drv.dump_ast = true;
    } else if (argv[i] == std::string("--dump-ir")) {
      drv.dump_ir = true;
    } else if (!drv.parse(argv[i])) {
      if (drv.dump_ast)
        drv.program->dumpAST(std::cout, 0);
    }
  }

  Context C;
  drv.program->codegen(C);

  if (drv.dump_ir)
    C.dumpIR();

  verifyModule(C.getModule());

  InitializeAllTargetInfos();
  InitializeAllTargets();
  InitializeAllTargetMCs();
  InitializeAllAsmParsers();
  InitializeAllAsmPrinters();

  auto TargetTriple = sys::getDefaultTargetTriple();
  C.getModule().setTargetTriple(TargetTriple);

  std::string Error;
  auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

  if (!Target) {
    errs() << Error;
    return -1;
  }

  auto CPU = "generic";
  auto Features = "";

  TargetOptions Opt;
  auto RM = Optional<Reloc::Model>();
  auto TheTargetMachine =
      Target->createTargetMachine(TargetTriple, CPU, Features, Opt, RM);

  C.getModule().setDataLayout(TheTargetMachine->createDataLayout());

  std::error_code EC;
  raw_fd_ostream dest("output.o", EC, sys::fs::F_None);

  if (EC) {
    errs() << "Could not open file: " << EC.message();
    return 1;
  }

  legacy::PassManager pass;
  auto FileType = TargetMachine::CGFT_ObjectFile;

  if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    errs() << "TheTargetMachine can't emit a file of this type";
    return 1;
  }

  pass.run(C.getModule());
  dest.flush();

  system("clang output.o");
  system("rm -f output.o");

  return 0;
}