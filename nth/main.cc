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
#include "ast_string_printer.h"

void writeOutput(nth::Driver &driver, nth::AstPrinter &printer, std::string filename);

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
  
  bool should_dump_parse_tree_dot = false;
  bool should_dump_parse_tree_string = false;

  nth::Driver driver;
  for (int i=1; i<argc; ++i) {
    std::cout << argv[i] << '\n';
    if (argv[i] == std::string("--debug-parse")) {
      driver.should_trace_parsing = true;
    } else if (argv[i] == std::string("--debug-scan")) {
      driver.should_trace_scanning = true;
    } else if (argv[i] == std::string("--parse-tree=dot")) {
      should_dump_parse_tree_dot = true;
    } else if (argv[i] == std::string("--parse-tree=string")) {
      should_dump_parse_tree_string = true;
    } else {
      int ret = driver.parse(argv[i]);

      std::string inputfilename = std::string(argv[i]);
      std::string outputfilename;
      if (inputfilename.empty() || inputfilename == "-") {
        inputfilename = "a.out";
      }

      if (should_dump_parse_tree_dot || should_dump_parse_tree_string) {
        if (should_dump_parse_tree_dot) {
          outputfilename = inputfilename + ".dot";
          nth::AstDotPrinter p;
          writeOutput(driver, p, outputfilename);
        }

        if (should_dump_parse_tree_string) {
          nth::AstStringPrinter p;
          outputfilename = inputfilename + ".txt";
          writeOutput(driver, p, outputfilename);
        }
      }
      return ret;
    }
  }
  return -1;
}

void writeOutput(nth::Driver &driver, nth::AstPrinter &printer, std::string filename) {
  driver.result->accept(printer);
  FILE *file;
  if (!(file = fopen(filename.c_str(), "w"))) {
    std::cerr << "cannot open " << filename << ": " << strerror(errno);
    exit(EXIT_FAILURE);
  }

  fprintf(file, "%s", printer.getOutput().c_str());
}