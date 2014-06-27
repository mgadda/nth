%code requires {
  namespace nth {
    // Forward declaration because Context 
    // is used in generated header(s)
    class Context;
  }
}


%{
  #include <iostream>

  #include "context.h"
  #include "ast.h"

  using namespace std;
  using namespace nth;

/*  extern int yylineno;
  extern int yyloc; */
  int exit_status;

  /*void yyerror(const char *s, ...) { 
    va_list ap;
    va_start(ap, s);
    
    fprintf(stderr, "\n\n%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n\n");
    
    exit_status = 1;
    //printf(">>>> ERROR: line %d, %s\n", yylineno, s); 
  } */
  
/*
  typedef enum {
    EqualTo,
    NotEqualTo,
    LessThan,
    GreaterThan,
    LessThanOrEqualTo,
    GreaterThanOrEqualTo
  } ComparisonOperators;
*/

%}

%language "C++"
%defines /* forces creation of y.tab.h */

%debug
%error-verbose
%locations

%parse-param { nth::Context &ctx }
%lex-param { nth::Context &ctx }

%union {
  // Literals
  double f;
  long l;
  char *str; // used for strings, identifiers
}

%token CMP 
%token AND OR NOT IF ELSE DEF VAL CLASS
%token TRUE FALSE
%token FLOAT INT
%token STRING
%token IDENT
%token HASH_ROCKET
%token LSHIFT RSHIFT DOUBLE_DOT TRIPLE_DOT

%{
  extern int yylex(
    yy::parser::semantic_type* yylval,
    yy::parser::location_type* yylloc,
    nth::Context &ctx);

%}
%start file

%%


file: expressions;

expressions: expr 
          | expr expressions
          ;

expr: literal
    | binary_operation
    | func_def
    | val_def 
    | if_else
    | func_call
    | deref
    | '(' expr ')'
    ;

literal: INT
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
array: '[' exprlist ']'

exprlist: expr 
        | expr ',' exprlist
        ;


  /* Hash */
hash: '{' key_val_list '}'
    | '{' '}'
    ;

key_val_list: key_value 
            | key_value ',' key_val_list
            ;

key_value: key ':' expr;

key: STRING
   | IDENT
   ;


  /* Range */
range: INT DOUBLE_DOT INT
     | INT TRIPLE_DOT INT 
     ;


  /* Tuple */
tuple: '(' exprlist ')';

  /* end literals */


binary_operation: boolean_op
                | comparison_op
                | math_op
                | bitwise_op
                ;

boolean_op: expr AND expr
          | expr OR expr
          | NOT expr
          ;

comparison_op: expr CMP expr;

math_op: expr '+' expr
       | expr '-' expr
       | expr '*' expr
       | expr '/' expr
       | expr '^' expr
       | expr '%' expr
       ;

bitwise_op: expr LSHIFT expr
          | expr RSHIFT expr
          | expr '|' expr
          | expr '&' expr
          ;

  /* end binary ops */

  /* Functions */
block: '{' expressions '}'

func_def: DEF IDENT '(' arglist ')' ':' type block
        | '{' '(' arglist ')' ':' type HASH_ROCKET expr '}'

arglist: arg
       | arg ',' arglist
       ;

arg: IDENT ':' type;

func_call: IDENT '(' exprlist ')';

  /* Variables */
val_def: VAL IDENT ':' type '=' expr;

deref: IDENT '[' expr ']'
     | IDENT '.' INT
     ;

  /* Control Flow */

if_else: IF '(' expr ')' block
       | IF '(' expr ')' block ELSE block
       ;

  /* Types */

typelist: type
        | type ',' typelist
        ;

type: IDENT
    | IDENT '[' typelist ']'
    | '(' typelist ')'
    | '(' typelist ')' HASH_ROCKET type
    ;
%%
