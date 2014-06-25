//
//  main.cpp
//  nth
//
//  Created by Matt Gadda on 6/5/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include <iostream>
#include <cstdio>

#include "context.h"
#include "parse.tab.hh"

extern void yyrestart(FILE*);
//extern "C" int yyparse();
extern int yylineno;
//extern int yydebug;

using namespace std;

int main(int argc, const char * argv[])
{
  // Load files specified by filename in argv
  // For each file:
  // lex/parse -> AST
  // type check -> AST
  // generate IR -> IR
  // optimize -> IR
  // code gen -> ASM
  //yydebug = 0;

  FILE *f = fopen(argv[1], "r");
  if (!f) {
    printf("Cannot open %s\n", argv[1]);
    return -1;
  }

  yylineno = 1;
  yyrestart(f);

  nth::Context context;
  yy::parser parser(context);
  int ret = parser.parse();
  fclose(f);

  return ret;
}

namespace yy {
  void parser::error(location const &loc, const string& s) {
    cerr << "error at " << loc << ": " << s << endl;
  }
}

