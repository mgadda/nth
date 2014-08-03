//
//  objc_helper.h
//  nth
//
//  Created by Matt Gadda on 7/13/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include <string>
#include <sstream>

#include "ast_visitor.h"


std::string getResourcePath();

class AstPrinter : public nth::Visitor {
 public:
  void visit(nth::Block *block);
  void visit(nth::String *string);
  void visit(nth::Integer *integer);
  void visit(nth::Float *flt);
  void visit(nth::True *tru);
  void visit(nth::False *flse);
  void visit(nth::Identifier *ident);
  void visit(nth::Array *array);
  void visit(nth::Map *map);
  void visit(nth::Add *add);
  void visit(nth::Subtract *subtract);
  void visit(nth::Multiply *multiply);
  void visit(nth::Divide *divide);
  void visit(nth::Exponentiate *exp);
  void visit(nth::Modulo *modulo);
  
  std::string getOutput();
 protected:
  std::stringstream ast_output;
};