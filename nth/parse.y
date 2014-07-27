%code requires {
  #include "ast.h"

  namespace nth {
    // Forward declaration because Driver
    // is used in generated header(s)
    class Driver;
  }
}

%{
  #include <iostream>
  #include <utility> // provides std::swap as of C++11
  #include "driver.h"

 int exit_status;

%}

%language "C++"
%defines /* forces creation of y.tab.h */
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define parse.trace
%define parse.error verbose
%debug
%locations
%initial-action {
  @$.begin.filename = @$.end.filename = &driver.file;
}
%define api.token.prefix {T_}

%param { nth::Driver &driver }

%token
  END 0   "end of file"
  PLUS    "+"
  MINUS   "-"
  TIMES   "*"
  DIVIDE  "/"
  ASSIGN  "="
  POW     "^"
  MODULO  "%"
  BIT_OR  "|"
  BIT_AND "&"
  PERIOD  "."
  COMMA   ","
  LPAREN  "("
  RPAREN  ")"
  LBRACKET "["
  RBRACKET "]"
  LCURLY  "{"
  RCURLY  "}"
  COLON   ":"
  ;

%token CMP
%token AND "&&" OR "||" NOT "!" IF ELSE DEF VAL CLASS
%token TRUE FALSE
%token <double> FLOAT
%token <long> INT
%token <std::string> STRING
%token <std::string> IDENT
%token HASH_ROCKET "=>"
%token LSHIFT "<<" RSHIFT ">>" DOUBLE_DOT ".." TRIPLE_DOT "..."

%left "="
%right VAL
%left "+" "-"
%left "*" "/"
%left "%" "|" "^" "&"
%left "<<" ">>"
%left CMP
%left NOT
%left "&&" "||"

%type <nth::Block*> file;
%type <nth::Block*> expressions;
%type <nth::Expression*> expr literal;
 // %type <std::unique_ptr<nth::BinaryOperation> > binary_operation;

%start file

  // %printer { yyoutput << $$; } <*>;
%%


file: expressions  { driver.result = $1; }
    ;

expressions: expr              { $$ = new nth::Block($1); }
           | expressions expr  { std::swap($$, $1); $$->insertAfter($2); }
           ;

expr: literal { std::swap($$, $1); }
    | binary_op
    | unary_op
    | func_def
    | val_def
    | if_else
    | func_call
    | deref
    | "(" expr ")"
    ;

literal: INT     { $$ = new nth::Integer($1); }
       | FLOAT
       | STRING
       | TRUE
       | FALSE
       | IDENT
       | compound_literal
       ;

compound_literal: array
                | hash
                | range
                | tuple
                ;


  /* Array */
array: "[" exprlist "]"

exprlist: expr
        | exprlist "," expr
        ;


  /* Hash */
hash: "{" key_val_list "}"
    | "{" "}"
    ;

key_val_list: key_value
            | key_val_list "," key_value
            ;

key_value: key ":" expr;

key: STRING
   | IDENT
   ;


  /* Range */
range: INT ".." INT
     | INT "..." INT
     ;


  /* Tuple */
tuple: "(" exprlist ")";

  /* end literals */


binary_op: boolean_op
         | comparison_op
         | math_op
         | bitwise_op
         ;

boolean_op: expr "&&" expr
          | expr "||" expr
          ;

comparison_op: expr CMP expr;

math_op: expr "+" expr
       | expr "-" expr
       | expr "*" expr
       | expr "/" expr
       | expr "^" expr
       | expr "%" expr
       ;

bitwise_op: expr "<<" expr
          | expr ">>" expr
          | expr "|" expr
          | expr "&" expr
          ;

unary_op: "!" expr %prec NOT

  /* end binary ops */

  /* Functions */
block: "{" expressions "}"

func_def: DEF IDENT "(" arglist ")" ":" type block
        | "{" "(" arglist ")" ":" type "=>" expr "}"

arglist: arg
       | arg "," arglist
       ;

arg: IDENT ":" type;

func_call: IDENT "(" exprlist ")";

  /* Variables */
val_def: VAL IDENT ":" type "=" expr;

deref: IDENT "[" expr "]"
     | IDENT "." INT
     ;

  /* Control Flow */

if_else: IF "(" expr ")" block
       | IF "(" expr ")" block ELSE block
       ;

  /* Types */

typelist: type
        | type "," typelist
        ;

type: IDENT
    | IDENT "[" typelist "]"
    | "(" typelist ")"
    | "(" typelist ")" "=>" type
    ;
%%

void yy::parser::error(const location_type& l, const std::string& m) {
  driver.error(l, m);
}
