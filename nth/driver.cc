//
//  driver.cpp
//  nth
//
//  Created by Matt Gadda on 6/21/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include "driver.h"

namespace nth {
  
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