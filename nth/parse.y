%code requires {
  #include "ast.h"
  #include "type_literal.h"

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
  ;

%token <nth::Comparison::Type> CMP
%token AND "&&" OR "||" NOT "!" IF ELSE DEF VAL CLASS TRAIT TYPE
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
%type <nth::Block*> statements block optional_block;
%type <nth::ASTNode*> statement;
%type <nth::Expression*> expr literal compound_literal binary_op unary_op;
%type <nth::ExpressionList*> exprlist;
%type <nth::Array*> array;
%type <nth::Map*> map;
%type <nth::Range*> range;
%type <nth::Tuple*> tuple;
%type <nth::ExpressionMap*> key_val_list;
%type <std::pair<nth::String*, nth::Expression*>> key_value;
%type <nth::String*> key;
%type <nth::BinaryOperation*> math_op bitwise_op boolean_op comparison_op subscript field_access;
%type <nth::FunctionDef*> func_def func_def_with_type_param func_def_without_type_param;
%type <nth::LambdaDef*> lambda;
%type <nth::FunctionCall*> func_call;
%type <nth::VariableDef*> val_def;
%type <nth::ArgList*> arglist optional_ctor_args;
%type <nth::TypeRefList*> typeref_list
%type <nth::TypeDefList*> optional_type_param type_param type_param_list;
%type <nth::TypeRef*> typeref
%type <nth::TypeDef*> typedef;
%type <nth::Argument*> arg;
%type <nth::IfElse*> if_else;
%type <nth::TypeAliasDef*> type_alias_def;
%type <nth::TraitDef*> trait_def;

%start file

  // %printer { yyoutput << $$; } <*>;
%%


file: statements  { driver.result = $1; }
    ;

statements: statement             { $$ = new nth::Block($1); }
          | statements statement  { std::swap($$, $1); $$->insertAfter($2); }
          ;

statement: expr           { nth::ASTNode* node = $1; std::swap($$, node); }
         | val_def        { nth::ASTNode* node = $1; std::swap($$, node); }
         | func_def       { nth::ASTNode* node = $1; std::swap($$, node); }
         | trait_def      { nth::ASTNode* node = $1; std::swap($$, node); }
         | type_alias_def { nth::ASTNode* node = $1; std::swap($$, node); }
         ;

expr: literal   { std::swap($$, $1); }
    | binary_op { std::swap($$, $1); }
    | unary_op  { std::swap($$, $1); }
    | lambda    { nth::Expression *expr = $1; std::swap($$, expr); }
    | if_else   { nth::Expression *expr = $1; std::swap($$, expr); }
    | func_call { nth::Expression *expr = $1; std::swap($$, expr); }
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
    | "{" "}"              { $$ = new nth::Map(); }
    ;

key_val_list: key_value                   { $$ = new nth::ExpressionMap(); $$->push_back($1); }
            | key_val_list "," key_value  { std::swap($$, $1); $$->push_back($3); }
            ;

key_value: key ":" expr { $$ = std::make_pair($1, $3); }
         ;

key: STRING { $$ = new nth::String($1.substr(1, $1.size()-2)); }
   ;


  /* Range */
range: INT ".." INT   { $$ = new nth::Range(new nth::Integer($1), new nth::Integer($3), nth::Range::Exclusivity::Exclusive); }
     | INT "..." INT  { $$ = new nth::Range(new nth::Integer($1), new nth::Integer($3), nth::Range::Exclusivity::Inclusive); }
     ;


  /* Tuple */
tuple: "(" exprlist ")" { $$ = new nth::Tuple(*$2); }
     ;

  /* end literals */


binary_op: boolean_op     { nth::Expression *e = $1; std::swap($$, e); }
         | comparison_op  { nth::Expression *e = $1; std::swap($$, e); }
         | math_op        { nth::Expression *e = $1; std::swap($$, e); }
         | bitwise_op     { nth::Expression *e = $1; std::swap($$, e); }
         | subscript      { nth::Expression *e = $1; std::swap($$, e); }
         | field_access { nth::Expression *e = $1; std::swap($$, e); }
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

field_access: expr "." IDENT { $$ = new nth::FieldAccess($1, new nth::Identifier($IDENT, @IDENT)); }
            | expr "." INT   { $$ = new nth::TupleFieldAccess($1, new nth::Integer($INT)); }
            ;


  /* end binary ops */

  /* Functions */
block: "{" statements "}"  { std::swap($$, $2); }

func_def: func_def_without_type_param { std::swap($$, $1); }
        | func_def_with_type_param    { std::swap($$, $1); }
        ;

func_def_with_type_param: DEF IDENT type_param "(" arglist ")" ":" typeref block {
            $$ = new nth::FunctionDef(
              new nth::Identifier($2),
              *$5, $8, $9, *$3
            );
          }
        ;

func_def_without_type_param: DEF IDENT "(" arglist ")" ":" typeref block {
            $$ = new nth::FunctionDef(
              new nth::Identifier($2),
              *$4, $7, $8, *(new nth::TypeDefList)
            );
          }
        ;

type_param: "[" type_param_list "]"  { std::swap($$, $2); }
          ;

trait_def: TRAIT IDENT optional_type_param optional_ctor_args optional_block {
              $$ = new nth::TraitDef(
                new nth::Identifier($IDENT),
                $optional_type_param,
                $optional_ctor_args,
                $optional_block
              );
            }
         ;

optional_type_param: /* this space intentional left blank */  { $$ = nullptr; }
                   | type_param { $$ = $1; }
                   ;

optional_ctor_args: /* this space intentional left blank */   { $$ = nullptr; }
                  | "(" arglist ")"                           { std::swap($$, $2); }
                  ;

optional_block: /* this space intetionally left blank */      { $$ = nullptr; }
              | block { $$ = $1; }
              ;

type_alias_def: TYPE IDENT "=" typeref { $$ = new nth::TypeAliasDef(new nth::SimpleTypeDef(new nth::Identifier($2)), $4); }
              ;

lambda: "(" arglist ")" ":" typeref "=>" expr { $$ = new nth::LambdaDef(*$2, $5, $7); }
      ;

arglist: arg               { $$ = new nth::ArgList(1, $1); }
       | arg "," arglist   { std::swap($$, $3); $$->push_front($1); }
       | /* no arguments */{ $$ = new nth::ArgList(); }
       ;

arg: IDENT ":" typeref { $$ = new nth::Argument(new nth::Identifier($1), $3); }
   ;

func_call: expr "(" exprlist ")"  { $$ = new nth::FunctionCall($1, *$3); }
         | expr "(" ")"           { $$ = new nth::FunctionCall($1, *(new nth::ExpressionList)); }
         ;

  /* Variables */
val_def: VAL IDENT ":" typeref "=" expr {
           $$ = new nth::VariableDef(new nth::Identifier($2), $4, $6);
         }
       ;

  /* Control Flow */

if_else: IF "(" expr ")" block             { $$ = new nth::IfElse($3, $5, nullptr); }
       | IF "(" expr ")" block ELSE block  { $$ = new nth::IfElse($3, $5, $7); }
       ;

  /* Types */

typeref_list: typeref                  { $$ = new nth::TypeRefList(1, $1); }
           | typeref "," typeref_list  { std::swap($$, $3); $$->push_front($1); }
           ;

typeref: IDENT                          { $$ = new nth::SimpleTypeRef(new nth::Identifier($IDENT, @IDENT)); }
       | IDENT "[" typeref_list "]"      { $$ = new nth::TemplatedTypeRef(new nth::Identifier($1), *$3); }
       | "(" typeref_list ")"            { $$ = new nth::TupleTypeRef(*$2); } /* TODO: replace N with length of typeref_list */
       | "(" typeref_list ")" "=>" typeref  { $$ = new nth::FunctionTypeRef(*$2, $5); } /* TODO: look up typeref instance by string */
       | "(" ")" "=>" typeref              { $$ = new nth::FunctionTypeRef(*(new nth::TypeRefList()), $4); }
       ;

type_param_list: typedef                    { $$ = new nth::TypeDefList(1, $1); }
               | typedef "," type_param_list  { std::swap($$, $3); $$->push_front($1); }
               ;

typedef: IDENT  { $$ = new nth::SimpleTypeDef(new nth::Identifier($1)); }
       ;

%%

void yy::parser::error(const location_type& l, const std::string& m) {
  driver.error(l, m);
}
