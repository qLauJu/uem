%{
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>
#include "Driver.hh"  
#include "Parser.hh"

#undef yywrap
#define yywrap() 1

#if defined __GNUC__ && 7 <= __GNUC__
#pragma GCC diagnostic ignored "-Wnull-dereference"
#endif
%}

%option noyywrap nounput batch debug noinput

id [a-zA-Z_][a-zA-Z_0-9]*
int [0-9]+
blank [ \t]

%{
  // Code run each time a pattern is matched.
  #define YY_USER_ACTION loc.columns (yyleng);
%}

%%

%{
// A handy shortcut to the location held by the driver.
yy::location &loc = drv.location;
// Core run each time yylex is called.
loc.step();
%}

"for" return yy::parser::make_FOR(loc);
"if" return yy::parser::make_IF(loc);
"else" return yy::parser::make_ELSE(loc);
"def" return yy::parser::make_DEF(loc);
"var" return yy::parser::make_VAR(loc);
"true" return yy::parser::make_BOOL_LITERAL(true, loc);
"false" return yy::parser::make_BOOL_LITERAL(false, loc);
"while" return yy::parser::make_WHILE(loc);
"return" return yy::parser::make_RETURN(loc);
"stop" return yy::parser::make_STOP(loc);
"skip" return yy::parser::make_SKIP(loc);
"write" return yy::parser::make_WRITE(loc);
"read" return yy::parser::make_READ(loc);

"int" return yy::parser::make_TYPE_INT("type_int", loc);
"string" return yy::parser::make_TYPE_STRING("type_string", loc);
"bool" return yy::parser::make_TYPE_BOOL("type_bool", loc);

{blank}+ loc.step();
"//".* loc.step();
[\n]+ loc.lines(yyleng); loc.step();

\"(\\.|[^\\"])*\" return yy::parser::make_STRING_LITERAL(yytext, loc);

";" return yy::parser::make_SEMICOLON(loc);
":" return yy::parser::make_COLON(loc);
"-" return yy::parser::make_MINUS(loc);
"+" return yy::parser::make_PLUS(loc);
"*" return yy::parser::make_STAR(loc);
"%" return yy::parser::make_MOD(loc);
"/" return yy::parser::make_SLASH(loc);
"(" return yy::parser::make_LPAREN(loc);
")" return yy::parser::make_RPAREN(loc);
"[" return yy::parser::make_LBRACKET(loc);
"]" return yy::parser::make_RBRACKET(loc);
"{" return yy::parser::make_LBRACE(loc);
"}" return yy::parser::make_RBRACE(loc);
"=" return yy::parser::make_ASSIGN(loc);
"!" return yy::parser::make_NOT(loc);
"!=" return yy::parser::make_DIFF(loc);
"==" return yy::parser::make_EQ(loc);
"<" return yy::parser::make_LT(loc);
"<=" return yy::parser::make_LTEQ(loc);
">" return yy::parser::make_GT(loc);
">=" return yy::parser::make_GTEQ(loc);
"," return yy::parser::make_COMMA(loc);
"\"" return yy::parser::make_QMARK(loc);
"||" return yy::parser::make_OR(loc);
"&&" return yy::parser::make_AND(loc);



{int} {
  errno = 0;
  long n = strtol(yytext, NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error (loc, "integer is out of ranges: " + std::string(yytext));
    return yy::parser::make_NUMBER(n, loc);
}
{id} return yy::parser::make_IDENTIFIER(yytext, loc);
<<EOF>> return yy::parser::make_END(loc);
. {
  throw yy::parser::syntax_error (loc, "invalid character: " + std::string(yytext));
}
%%

void Driver::scan_begin() {
  yy_flex_debug = trace_scanning;
  if (file.empty() || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen(file.c_str(), "r"))) {
    std::cerr << "cannot open " << file << ": " << strerror(errno) << '\n';
    exit(EXIT_FAILURE);
  }
}

void Driver::scan_end() {
  fclose(yyin);
}
