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
  BIT_NOT "~"
  PERIOD  "."
  COMMA   ","
  LPAREN  "("
  RPAREN  ")"
  LBRACKET "["
  RBRACKET "]"
  LCURLY  "{"
  RCURLY  "}"
  COLON   ":"
  SEMICOLON ";"
  NEWLINE "\n"
  ;

%token <nth::Comparison::Type> CMP
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
%left BIT_NOT
%left NOT
%left "&&" "||"

%type <nth::Block*> file;
%type <nth::Block*> statements block;
%type <nth::Expression*> statement expr literal compound_literal binary_op unary_op;
%type <nth::ExpressionList*> exprlist;
%type <nth::Array*> array;
%type <nth::Map*> map;
%type <nth::Range*> range;
%type <nth::Tuple*> tuple;
%type <nth::ExpressionMap*> key_val_list;
%type <std::pair<nth::String*, nth::Expression*>> key_value;
%type <nth::String*> key;
%type <nth::BinaryOperation*> math_op bitwise_op boolean_op comparison_op subscript tuple_field_access;


 // %type <std::unique_ptr<nth::BinaryOperation> > binary_operation;

%start file

  // %printer { yyoutput << $$; } <*>;
%%

file: statements  { driver.result = $1; }
    ;

statements: statement             { $$ = new nth::Block($1); }
          | statements NEWLINE statement  {
              std::swap($$, $1);
              if ($3) {
                $$->insertAfter($3);
              }
            }
          ;

statement: expr       { std::swap($$, $1); }
         | val_def
         | func_def
         | END        { $$ = nullptr; }
         ;

/*
file: expressions  { driver.result = $1; }
    ;

expressions: expr              { $$ = new nth::Block($1); }
           | expressions expr  { std::swap($$, $1); $$->insertAfter($2); }
           ;
*/

expr: literal   { std::swap($$, $1); }
    | binary_op { std::swap($$, $1); }
    | unary_op  { std::swap($$, $1); }
    | lambda
    | if_else
    | func_call
    | "(" expr ")"
    ;

literal: INT     { $$ = new nth::Integer($1); }
       | FLOAT   { $$ = new nth::Float($1); }
       | STRING  { $$ = new nth::String($1.substr(1, $1.size()-2)); }
       | TRUE    { $$ = new nth::True; }
       | FALSE   { $$ = new nth::False; }
       | IDENT   { $$ = new nth::Identifier($1); }
       | compound_literal { std::swap($$, $1); }
       ;

compound_literal: array { nth::Expression *e = $1; std::swap($$, e); }
                | map   { nth::Expression *e = $1; std::swap($$, e); }
                | range { nth::Expression *e = $1; std::swap($$, e); }
                | tuple { nth::Expression *e = $1; std::swap($$, e); }
                ;


  /* Array */
array: "[" exprlist "]" { $$ = new nth::Array(*$2); }
     | "[" "]"          { $$ = new nth::Array(); }
     ;

exprlist: expr               { $$ = new nth::ExpressionList(1, $1); }
        | exprlist "," expr  { std::swap($$, $1); $$->push_back($3); }
        ;


  /* Map */
map: "{" key_val_list "}" { $$ = new nth::Map(*$2); }
    | "{" NEWLINE key_val_list "}" { $$ = new nth::Map(*$3); }
    | "{" key_val_list NEWLINE "}" { $$ = new nth::Map(*$2); }
    | "{" "}"              { $$ = new nth::Map(); }
    | "{" NEWLINE "}"              { $$ = new nth::Map(); }
    ;

key_val_list: key_value                   { $$ = new nth::ExpressionMap(); $$->push_back($1); }
            | key_val_list "," key_value  { std::swap($$, $1); $$->push_back($3); }
            | key_val_list "," NEWLINE key_value  { std::swap($$, $1); $$->push_back($4); }
            ;

key_value: key ":" expr { $$ = std::make_pair($1, $3); }
         | key ":" expr NEWLINE { $$ = std::make_pair($1, $3); }
         ;

key: STRING { $$ = new nth::String($1.substr(1, $1.size()-2)); }
   ;


  /* Range */
range: INT ".." INT   { $$ = new nth::Range(new nth::Integer($1), new nth::Integer($3), nth::Range::Exclusivity::Exclusive); }
     | INT "..." INT  { $$ = new nth::Range(new nth::Integer($1), new nth::Integer($3), nth::Range::Exclusivity::Inclusive); }
     ;


  /* Tuple */
tuple: "(" exprlist ")" { $$ = new nth::Tuple(*$2); }
     | "(" ")"          { $$ = new nth::Tuple(); }

  /* end literals */


binary_op: boolean_op     { nth::Expression *e = $1; std::swap($$, e); }
         | comparison_op  { nth::Expression *e = $1; std::swap($$, e); }
         | math_op        { nth::Expression *e = $1; std::swap($$, e); }
         | bitwise_op     { nth::Expression *e = $1; std::swap($$, e); }
         | subscript      { nth::Expression *e = $1; std::swap($$, e); }
         | tuple_field_access { nth::Expression *e = $1; std::swap($$, e); }
         ;

boolean_op: expr "&&" expr  { $$ = new nth::LogicalAnd(nth::ExpressionPtr($1), nth::ExpressionPtr($3)); }
          | expr "||" expr  { $$ = new nth::LogicalOr(nth::ExpressionPtr($1), nth::ExpressionPtr($3)); }
          ;

comparison_op: expr CMP expr { $$ = new nth::Comparison(nth::ExpressionPtr($1), nth::ExpressionPtr($3), $2); }
             ;

math_op: expr "+" expr  { $$ = new nth::Add(nth::ExpressionPtr($1), nth::ExpressionPtr($3)); }
       | expr "-" expr  { $$ = new nth::Subtract(nth::ExpressionPtr($1), nth::ExpressionPtr($3)); }
       | expr "*" expr  { $$ = new nth::Multiply(nth::ExpressionPtr($1), nth::ExpressionPtr($3)); }
       | expr "/" expr  { $$ = new nth::Divide(nth::ExpressionPtr($1), nth::ExpressionPtr($3)); }
       | expr "^" expr  { $$ = new nth::Exponentiate(nth::ExpressionPtr($1), nth::ExpressionPtr($3)); }
       | expr "%" expr  { $$ = new nth::Modulo(nth::ExpressionPtr($1), nth::ExpressionPtr($3)); }
       ;

bitwise_op: expr "<<" INT { $$ = new nth::BitShiftLeft(nth::ExpressionPtr($1), std::unique_ptr<nth::Integer>(new nth::Integer($3))); }
          | expr ">>" INT { $$ = new nth::BitShiftRight(nth::ExpressionPtr($1), std::unique_ptr<nth::Integer>(new nth::Integer($3))); }
          | expr "|" expr { $$ = new nth::BitwiseOr(nth::ExpressionPtr($1), nth::ExpressionPtr($3)); }
          | expr "&" expr { $$ = new nth::BitwiseAnd(nth::ExpressionPtr($1), nth::ExpressionPtr($3)); }
          ;

unary_op: "!" expr %prec NOT      { $$ = new nth::LogicalNot(nth::ExpressionPtr($2)); }
        | "~" expr %prec BIT_NOT  { $$ = new nth::BitwiseNot(nth::ExpressionPtr($2)); }
        ;

subscript: expr "[" expr "]" { $$ = new nth::Subscript($1, $3); }
         ;

tuple_field_access: expr "." INT { $$ = new nth::TupleFieldAccess($1, new nth::Integer($3)); }
                  ;


  /* end binary ops */

  /* Functions */
block: "{" statements "}"
     | "{" NEWLINE statements NEWLINE "}"
     ;

func_def: DEF IDENT "(" arglist ")" ":" type block
        ;

lambda: "{" "(" arglist ")" ":" type "=>" expr "}"
      ;

arglist: arg
       | arg "," arglist
       ;

arg: IDENT ":" type;

func_call: expr "(" exprlist ")";

  /* Variables */
val_def: VAL IDENT ":" type "=" expr;

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
