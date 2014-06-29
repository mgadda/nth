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


namespace nth {

// TODO: move this into Driver
int parse(std::string& filename);
  
class Driver {

 public:
  Driver();
  virtual ~Driver();
  
  int result;
  void scanBegin();
  void scanEnd();
  bool trace_scanning;
  
  int parse(const std::string& f);
  
  std::string file;
  
  bool trace_parsing;
  
  void error(const yy::location& l, const std::string& msg);
  void error(const std::string& msg);
};
  
}
#endif /* defined(__nth__driver__) */
