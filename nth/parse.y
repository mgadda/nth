%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdarg.h>
  
  //#include "ast.h"

  #define YYDEBUG 1

  extern int yylex();
  extern int yylineno;

  int exit_status;

  void yyerror(const char *s, ...) { 
    va_list ap;
    va_start(ap, s);
    
    fprintf(stderr, "\n\n%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n\n");
    
    exit_status = 1;
    //printf(">>>> ERROR: line %d, %s\n", yylineno, s); 
  }
  
  typedef enum {
    EqualTo,
    NotEqualTo,
    LessThan,
    GreaterThan,
    LessThanOrEqualTo,
    GreaterThanOrEqualTo
  } ComparisonOperators;


%}

%debug
%error-verbose

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

%defines /* forces creation of y.tab.h */


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
    ;

literal: INT
       | FLOAT
       | STRING 
       | TRUE
       | FALSE
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
hash: '{' key_val_list '}';

key_val_list: key_value 
            | key_value ',' key_val_list
            ;

key_value: key ":" expr;

key: STRING
   | IDENT
   ;


  /* Range */
range: INT '.' '.' INT
     | INT '.' '.' '.' INT
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

bitwise_op: expr '|' expr
          | expr '&' expr
          ;

  /* end binary ops */

  /* Functions */
block: '{' expressions '}'

func_def: DEF IDENT '(' arglist ')' ':' IDENT block;

arglist: arg
       | arg ',' arglist
       ;

arg: IDENT ':' IDENT;

func_call: IDENT '(' exprlist ')';

  /* Variables */
val_def: VAL IDENT ':' IDENT '=' expr;

  /* Control Flow */

if_else: IF '(' expr ')' block
       | IF '(' expr ')' block ELSE block
       ;

%%
