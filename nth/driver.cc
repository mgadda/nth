//
//  driver.cpp
//  nth
//
//  Created by Matt Gadda on 6/21/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include <cerrno>
#include "driver.h"
// scan.h must be included /after/ driver.h so that YY_DECL has already been defined
#include "scan.h"

/*
extern void yyrestart(FILE*);
//extern "C" int yyparse();
extern int yylineno;
//extern int yydebug;
extern FILE* yyin;
extern YY_BUFFER_STATE yy_scan_string(const char *str);
*/

extern int yy_flex_debug;

namespace nth {

Driver::Driver()
  : should_trace_scanning(false), should_trace_parsing(false) {}

Driver::~Driver() {}

void Driver::scanBegin() {
  yy_flex_debug = should_trace_scanning;

  if (file.empty() || file == "-") {
    yyin = stdin;
  } else if (!(yyin = fopen(file.c_str(), "r"))) {
    error("cannot open " + file + ": " + strerror(errno));
    exit(EXIT_FAILURE);
  }
  
  yyrestart(yyin);
}

void Driver::scanEnd() {
  fclose(yyin);
}


int Driver::parse(const std::string& f) {
  file = f;
  scanBegin();
  yy::parser parser(*this);
  parser.set_debug_level(should_trace_parsing);
  int ret = parser.parse();
  scanEnd();

  return ret;
}

int Driver::parseString(const std::string &s) {
  YY_BUFFER_STATE bs = yy_scan_string(s.c_str());

  yy::parser parser(*this);
  parser.set_debug_level(should_trace_parsing);
  int ret = parser.parse();
  yy_delete_buffer(bs);

  return ret;
}

void Driver::error(const yy::location& l, const std::string& msg) {
  std::cerr << l << ": " << msg << '\n';
}

void Driver::error(const std::string& msg) {
  std::cerr << msg << '\n';
}

}
