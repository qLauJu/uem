%skeleton "lalr1.cc"
%require "3.0.4"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <string>
  #include "AST.hh"
  #include "BinOp.hh"
  #include "Type.hh"
  class Driver;

  using namespace grace;
}

// The parsing context.
%param { Driver &drv }

%locations
%define api.location.file "location.hh"
%define api.location.include {<location.hh>}


%define parse.trace
%define parse.error verbose

%code { 
  #include "Driver.hh"
}

%define api.token.prefix {TOK_}
%token
  END 0 "end of file"
  EQ "=="
  DIFF "!="
  ASSIGN "="
  MINUS "-"
  PLUS "+"
  STAR "*"
  SLASH "/"
  MOD "%"
  OR "||"
  AND "&&"
  NOT "!"
  LPAREN "("
  RPAREN ")"
  LBRACKET "["
  RBRACKET "]"
  LBRACE "{"
  RBRACE "}"
  COLON ":"
  SEMICOLON ";"
  LT "<"
  LTEQ "<="
  GT ">"
  GTEQ ">="
  COMMA ","
  QMARK "\""

  VAR "var"
  DEF "def"
  TRUE "true"
  FALSE "false"
  IF "if"
  ELSE "else"
  WHILE "while"
  FOR "for"
  RETURN "return"
  STOP "stop"
  SKIP "skip"
  WRITE "write"
  READ "read"
;

//definir precedência
%left OR
%left AND
%left EQ DIFF
%left LT LTEQ
%left GT GTEQ
%left PLUS MINUS
%left STAR SLASH MOD
%right NOT


%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%token <bool> BOOL_LITERAL "bool literal"

%token <std::string> TYPE_INT "type_int"
%token <std::string> TYPE_STRING "type_string"
%token <std::string> TYPE_BOOL "type_bool"
%token <std::string> STRING_LITERAL

%type <StmtNode*> stmt func_decl proc_decl if_then_else_stmt while_stmt for_stmt return_stmt
%type <AssignNode *> assign_stmt assign_expr
%type <BlockNode*> stmts block

%type <VarDeclNodeListStmt *> var_decl
%type <grace::Type *> data_type
%type <SpecVar *> spec_var spec_var_simple spec_var_simple_init
%type <SpecVarList *> spec_var_list
%type <ExprNode *> expr
%type <CallExprNode *> call_expr
%type <LiteralNode *> literal
%type <ParamList*> param_list
%type <Param*> param
%type <ExprList*> expr_list


%printer { yyoutput << $$; } <*>;

%start program;
%%

program: stmts { drv.program = $1; }
       ;

stmts: stmt { $$ = new BlockNode(@$); $$->Stmts.push_back($1); }
      | stmts stmt { $1->Stmts.push_back($2); $$ = $1; }
      ;

stmt: var_decl { $$ = $1; }
    | func_decl { $$ = $1; }
    | proc_decl { $$ = $1; }
	| if_then_else_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    | for_stmt {$$ = $1; }
    | return_stmt { $$ = $1; }
    | SKIP SEMICOLON { $$ = new SkipNode(@1); }
    | STOP SEMICOLON { $$ = new StopNode(@1); }
    | assign_stmt { $$ = $1; }
    | WRITE expr_list SEMICOLON { $$ = new WriteNode(@1, $2); }
//    | call_expr SEMICOLON { $$ =  }
  //  | READ var_expr SEMICOLON { $$ = new ReadNode(@1 $2); }
    ;

if_then_else_stmt: IF LPAREN expr RPAREN block { $$ = new IfThenElseNode(@$, $3, $5, NULL); }
				| IF LPAREN expr RPAREN block ELSE block { $$ = new IfThenElseNode(@$, $3, $5, $7); }
        ;

var_decl: VAR spec_var_list COLON data_type SEMICOLON { $$ = new VarDeclNodeListStmt(@$);
                                                        for (auto spec : *$2) {
                                                          $$->varDeclList.push_back(
                                                            new VarDeclNode(spec->loc, spec->Id, spec->Assign, $4)
                                                          );
                                                          delete spec;
                                                        }
                                                        delete $2;
                                                      }
       ;

spec_var_list: spec_var { $$ = new SpecVarList(); $$->push_back($1); }
             | spec_var_list COMMA spec_var { $1->push_back($3); $$ = $1; }
             ;

spec_var: spec_var_simple { $$ = $1; }
       | spec_var_simple_init { $$ = $1; }
       ;

spec_var_simple: IDENTIFIER { $$ = new SpecVar(@$, $1, NULL); }
              ;

spec_var_simple_init: IDENTIFIER ASSIGN expr { $$ = new SpecVar(@$, $1, new AssignNode(@2, $1, $3)); }
                    ;

literal: STRING_LITERAL { $$ = new LiteralStringNode(@$, $1); }
       | NUMBER { $$ = new LiteralIntNode(@$, $1); }
       | BOOL_LITERAL { $$ = new LiteralBoolNode(@$, $1); };

data_type: TYPE_INT { $$ = new grace::IntType(); }
    | TYPE_STRING { $$ = new grace::StringType(); }
    | TYPE_BOOL { $$ = new grace::BoolType(); }
    ;

func_decl: DEF IDENTIFIER LPAREN RPAREN COLON data_type block { $$ = new FuncDeclNode(@$, $2, $6, new ParamList(), $7); }
    | DEF IDENTIFIER LPAREN param_list RPAREN COLON data_type block { $$ = new FuncDeclNode(@$, $2, $7, $4, $8); }
    ;

proc_decl: DEF IDENTIFIER LPAREN RPAREN block { $$ = new ProcDeclNode(@$, $2, new ParamList(), $5); };
  | DEF IDENTIFIER LPAREN param_list RPAREN block { $$ = new ProcDeclNode(@$, $2, $4, $6); };

param_list: param { $$ = new ParamList(); $$->push_back($1); }
  | param_list COMMA param { $1->push_back($3); $$ = $1; }
  ;

param: IDENTIFIER COLON data_type { $$ = new Param(@$, $1, $3); }
  ;

block: LBRACE stmts RBRACE { $$ = $2; }
     | LBRACE RBRACE { $$ = new BlockNode(@$); };

expr: IDENTIFIER { $$ = new VariableExprNode(@$, $1); }
    | literal { $$ = $1; }
    | NOT expr { $$ = new ExprNotNode(@$, $2); }
    | MINUS expr { $$ = new ExprNegativeNode(@$, $2); }
    | expr PLUS expr { $$ = new ExprOperationNode(@$, $1, BinOp::PLUS, $3); }
    | expr MINUS expr { $$ = new ExprOperationNode(@$, $1, BinOp::MINUS, $3); }
    | expr STAR expr { $$ = new ExprOperationNode(@$, $1, BinOp::TIMES, $3); }
    | expr SLASH expr { $$ = new ExprOperationNode(@$, $1, BinOp::DIV, $3); }
    | expr MOD expr { $$ = new ExprOperationNode(@$, $1, BinOp::MOD, $3); }
    | expr LT expr { $$ = new ExprOperationNode(@$, $1, BinOp::LT, $3); }
    | expr LTEQ expr { $$ = new ExprOperationNode(@$, $1, BinOp::LTEQ, $3); }
    | expr GT expr { $$ = new ExprOperationNode(@$, $1, BinOp::GT, $3); }
    | expr GTEQ expr { $$ = new ExprOperationNode(@$, $1, BinOp::GTEQ, $3); }
    | expr EQ expr { $$ = new ExprOperationNode(@$, $1, BinOp::EQ, $3); }
    | expr DIFF expr { $$ = new ExprOperationNode(@$, $1, BinOp::DIFF, $3); }
    | expr AND expr { $$ = new ExprOperationNode(@$, $1, BinOp::AND, $3); }
    | expr OR expr { $$ = new ExprOperationNode(@$, $1, BinOp::OR, $3); }
    | LPAREN expr RPAREN { $$ = $2; }
    | call_expr { $$ = $1; }
    ;

expr_list: expr { $$ = new ExprList(); $$->push_back($1); }
    | expr_list COMMA expr { $1->push_back($3); $$ = $1; }
    ;

call_expr: IDENTIFIER LPAREN RPAREN { $$ = new CallExprNode(@$, $1, new ExprList()); }
          | IDENTIFIER LPAREN expr_list RPAREN { $$ = new CallExprNode(@$, $1, $3); }
          ;

while_stmt: WHILE LPAREN expr RPAREN block { $$ = new WhileNode(@$, $3, $5); };

for_stmt: FOR LPAREN assign_expr SEMICOLON expr SEMICOLON assign_expr RPAREN block { $$ = new ForNode(@$, $3, $5, $7, $9); };

return_stmt: RETURN SEMICOLON { $$ = new ReturnNode(@$, nullptr); }
            | RETURN expr SEMICOLON { $$ = new ReturnNode(@$, $2); };

assign_expr: IDENTIFIER ASSIGN expr { $$ = new AssignNode(@$, $1, $3); }
            | IDENTIFIER PLUS ASSIGN expr { $$ = new CompoundAssignNode(@$, $1, BinOp::PLUS, $4); }
            | IDENTIFIER MINUS ASSIGN expr { $$ = new CompoundAssignNode(@$, $1, BinOp::MINUS, $4); }
            | IDENTIFIER STAR ASSIGN expr { $$ = new CompoundAssignNode(@$, $1, BinOp::TIMES, $4); }
            | IDENTIFIER SLASH ASSIGN expr { $$ = new CompoundAssignNode(@$, $1, BinOp::DIV, $4); }
            ;

assign_stmt: assign_expr SEMICOLON { $$ = $1; };
%%

void yy::parser::error(const location_type &l, const std::string &m) {
  std::cerr << l << ": " << m << '\n';
}
