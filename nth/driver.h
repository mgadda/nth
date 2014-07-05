//
//  driver.h
//  nth
//
//  Created by Matt Gadda on 6/21/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#ifndef __nth__driver__
#define __nth__driver__

#include <iostream>
#include "parse.hh"

#define YY_DECL \
  yy::parser::symbol_type yylex(nth::Driver& driver)
     // ... and declare it for the parser's sake.
YY_DECL;

namespace nth {

class Driver {

 public:
  Driver();
  virtual ~Driver();

  int result;
  void scanBegin();
  void scanEnd();
  bool should_trace_scanning;

  int parse(const std::string& f);

  std::string file;

  bool should_trace_parsing;

  void error(const yy::location& l, const std::string& msg);
  void error(const std::string& msg);
};

}
#endif /* defined(__nth__driver__) */
