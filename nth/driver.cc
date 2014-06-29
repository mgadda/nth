//
//  driver.cpp
//  nth
//
//  Created by Matt Gadda on 6/21/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include "driver.h"

extern void yyrestart(FILE*);
//extern "C" int yyparse();
extern int yylineno;
//extern int yydebug;

namespace nth {

int parse(std::string& filename) {
  FILE *f = fopen(filename.c_str(), "r");
  if (!f) {
    std::cout << "Cannot open " << filename << '\n';
    return -1;
  }
  
  yylineno = 1;
  yyrestart(f);
  
  Driver driver;
  yy::parser parser(driver);
  int ret = parser.parse();
  fclose(f);
  
  return ret;
}

Driver::Driver() {

}

Driver::~Driver() {
  
}
  
void Driver::scanBegin() {
  
}

void Driver::scanEnd() {
  
}


int Driver::parse(const std::string& f) {
  return 0;
}

void Driver::error(const yy::location& l, const std::string& msg) {
  
}
  
void Driver::error(const std::string& msg) {
  
}
  
}