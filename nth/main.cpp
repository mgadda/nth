//
//  main.cpp
//  nth
//
//  Created by Matt Gadda on 6/5/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include <iostream>
#include <stdio.h>

extern "C" void yyrestart(FILE*);
extern "C" int yyparse();
extern "C" int yylex();
extern int yylineno;
extern int yydebug;

int main(int argc, const char * argv[])
{
  // Load files specified by filename in argv
  // For each file:
  // lex/parse -> AST
  // type check -> AST
  // generate IR -> IR
  // optimize -> IR
  // code gen -> ASM
  yydebug = 1;

  FILE *f = fopen(argv[1], "r");
  if (!f) {
    printf("Cannot open %s\n", argv[1]);
    return errno;
  }

  yylineno = 1;
  yyrestart(f);
  int ret = yyparse();
  fclose(f);

  return ret;
}

