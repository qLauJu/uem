#include "Driver.hh"

Driver::Driver()
    : trace_parsing(false), trace_scanning(false), dump_ast(false),
      dump_ir(false) {}

int Driver::parse(const std::string &f) {
  file = f;
  location.initialize(&file);
  scan_begin();
  yy::parser parser(*this);
  parser.set_debug_level(trace_parsing);
  int res = parser.parse();
  scan_end();
  return res;
}