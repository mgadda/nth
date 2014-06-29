//
//  main.cpp
//  nth
//
//  Created by Matt Gadda on 6/5/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include <iostream>
#include <cstdio>

#include "driver.h"
#include "parse.hh"

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

  if (argc < 2) {
    std::cout << "No input file specified.\n";
    return -1;
  }
  
  std::string filename = std::string(argv[1]);
  return nth::parse(filename);
}

namespace yy {
  void parser::error(location const &loc, const std::string& s) {
    std::cerr << "error at " << loc << ": " << s << '\n';
  }
}