cmake_minimum_required(VERSION 3.10)

project(grace)

set(CMAKE_CXX_STANDARD 14)
add_definitions(-D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)

include_directories(include)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(GraceParser ${CMAKE_SOURCE_DIR}/Parser.yy ${CMAKE_CURRENT_BINARY_DIR}/Parser.cc DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/Parser.hh)
FLEX_TARGET(GraceScanner ${CMAKE_SOURCE_DIR}/Scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/Scanner.cc)

ADD_FLEX_BISON_DEPENDENCY(GraceScanner GraceParser)

find_package(LLVM)

if( NOT LLVM_FOUND )
  message(FATAL_ERROR "LLVM package can't be found. Set CMAKE_PREFIX_PATH variable to LLVM's installation prefix.")
endif()

include_directories( ${LLVM_INCLUDE_DIRS} )
link_directories( ${LLVM_LIBRARY_DIRS} )
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(grace 
                ${BISON_GraceParser_OUTPUTS}
                ${FLEX_GraceScanner_OUTPUTS}
                main.cc
                Driver.cc 
                Dump.cc
                Codegen.cc Context.cc Error.cc Type.cc SymbolTable.cc BinOp.cc Log.cc include/Log.hh include/location.hh)

llvm_map_components_to_libnames(REQ_LLVM_LIBRARIES native)
target_link_libraries(grace ${REQ_LLVM_LIBRARIES}
        LLVMLTO LLVMPasses LLVMObjCARCOpts LLVMSymbolize LLVMDebugInfoPDB LLVMDebugInfoDWARF LLVMMIRParser LLVMFuzzMutate LLVMCoverage LLVMTableGen LLVMDlltoolDriver LLVMOrcJIT LLVMXCoreDisassembler LLVMXCoreCodeGen LLVMXCoreDesc LLVMXCoreInfo LLVMXCoreAsmPrinter LLVMSystemZDisassembler LLVMSystemZCodeGen LLVMSystemZAsmParser LLVMSystemZDesc LLVMSystemZInfo LLVMSystemZAsmPrinter LLVMSparcDisassembler LLVMSparcCodeGen LLVMSparcAsmParser LLVMSparcDesc LLVMSparcInfo LLVMSparcAsmPrinter LLVMPowerPCDisassembler LLVMPowerPCCodeGen LLVMPowerPCAsmParser LLVMPowerPCDesc LLVMPowerPCInfo LLVMPowerPCAsmPrinter LLVMNVPTXCodeGen LLVMNVPTXDesc LLVMNVPTXInfo LLVMNVPTXAsmPrinter LLVMMSP430CodeGen LLVMMSP430Desc LLVMMSP430Info LLVMMSP430AsmPrinter LLVMMipsDisassembler LLVMMipsCodeGen LLVMMipsAsmParser LLVMMipsDesc LLVMMipsInfo LLVMMipsAsmPrinter LLVMLanaiDisassembler LLVMLanaiCodeGen LLVMLanaiAsmParser LLVMLanaiDesc LLVMLanaiAsmPrinter LLVMLanaiInfo LLVMHexagonDisassembler LLVMHexagonCodeGen LLVMHexagonAsmParser LLVMHexagonDesc LLVMHexagonInfo LLVMBPFDisassembler LLVMBPFCodeGen LLVMBPFAsmParser LLVMBPFDesc LLVMBPFInfo LLVMBPFAsmPrinter LLVMARMDisassembler LLVMARMCodeGen LLVMARMAsmParser LLVMARMDesc LLVMARMInfo LLVMARMAsmPrinter LLVMARMUtils LLVMAMDGPUDisassembler LLVMAMDGPUCodeGen LLVMAMDGPUAsmParser LLVMAMDGPUDesc LLVMAMDGPUInfo LLVMAMDGPUAsmPrinter LLVMAMDGPUUtils LLVMAArch64Disassembler LLVMAArch64CodeGen LLVMAArch64AsmParser LLVMAArch64Desc LLVMAArch64Info LLVMAArch64AsmPrinter LLVMAArch64Utils LLVMObjectYAML LLVMLibDriver LLVMOption LLVMWindowsManifest LLVMX86Disassembler LLVMX86AsmParser LLVMX86CodeGen LLVMGlobalISel LLVMSelectionDAG LLVMAsmPrinter LLVMX86Desc LLVMMCDisassembler LLVMX86Info LLVMX86AsmPrinter LLVMX86Utils LLVMMCJIT LLVMLineEditor LLVMInterpreter LLVMExecutionEngine LLVMRuntimeDyld LLVMCodeGen LLVMTarget LLVMCoroutines LLVMipo LLVMInstrumentation LLVMVectorize LLVMScalarOpts LLVMLinker LLVMIRReader LLVMAsmParser LLVMInstCombine LLVMBitWriter LLVMAggressiveInstCombine LLVMTransformUtils LLVMAnalysis LLVMProfileData LLVMObject LLVMMCParser LLVMMC LLVMDebugInfoCodeView LLVMDebugInfoMSF LLVMBitReader LLVMCore LLVMBinaryFormat LLVMSupport LLVMDemangle)