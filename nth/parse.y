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
%}

%debug

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

%start start

%%

start: /* nothing */
     ;

%%
