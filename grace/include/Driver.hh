#pragma once

#include "AST.hh"
#include "Parser.hh"
#include <map>
#include <string>

// Tell Flex the lexer's prototype
#define YY_DECL yy::parser::symbol_type yylex(Driver &drv)

YY_DECL;

class Driver {
public:
  Driver();

  std::map<std::string, int> variables;
  BlockNode *program;

  // Run the parser on file F. Return 0 on success.
  int parse(const std::string &f);

  // The Name of the file being parsed.
  std::string file;

  // Whether to generate parser debug traces.
  bool trace_parsing;

  // Handling the scanner.
  void scan_begin();

  void scan_end();

  // Whether to generate scanner debug traces.
  bool trace_scanning;

  bool dump_ast;
  bool dump_ir;

  // The token's location used by the scanner.
  yy::location location;
};
