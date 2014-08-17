//
//  main.cpp
//  nth
//
//  Created by Matt Gadda on 6/5/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <memory>
#include <cerrno>

#include "driver.h"
#include "parse.hh"

#include "ast.h"
#include "ast_dot_printer.h"

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
  
  bool should_dump_parse_tree = false;
  
  nth::Driver driver;
  for (int i=1; i<argc; ++i) {
    std::cout << argv[i] << '\n';
    if (argv[i] == std::string("--debug-parse")) {
      driver.should_trace_parsing = true;
    } else if (argv[i] == std::string("--debug-scan")) {
      driver.should_trace_scanning = true;
    } else if (argv[i] == std::string("--debug-parse-tree")) {
      should_dump_parse_tree = true;
    } else {
      int ret = driver.parse(argv[i]);
      
      if (should_dump_parse_tree) {
        AstDotPrinter printer;
        driver.result->accept(printer);
        std::string inputfilename = std::string(argv[i]);
        if (inputfilename.empty() || inputfilename == "-") {
          inputfilename = "a.out";
        }
        std::string dotfilename = inputfilename + ".dot";
        FILE *dotfile;
        if (!(dotfile = fopen(dotfilename.c_str(), "w"))) {
          std::cerr << "cannot open " << dotfilename << ": " << strerror(errno);
          exit(EXIT_FAILURE);
        }
      
        fprintf(dotfile, "%s", printer.getOutput().c_str());
        
      }
      return ret;
    }
  }
  return -1;
}