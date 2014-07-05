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

  if (argc < 2) {
    std::cout << "No input file specified.\n";
    return -1;
  }

  nth::Driver driver;
  for (int i=1; i<argc; ++i) {
    std::cout << argv[i] << '\n';
    if (argv[i] == std::string("--debug-parse")) {
      driver.should_trace_parsing = true;
    } else if (argv[i] == std::string("--debug-scan")) {
      driver.should_trace_scanning = true;
    } else {
      int ret = driver.parse(argv[i]);
      std::cout << driver.result << '\n';
      return ret;
    }
  }
  return -1;
}

namespace yy {
  void parser::error(location const &loc, const std::string& s) {
    std::cerr << "error at " << loc << ": " << s << '\n';
  }
}