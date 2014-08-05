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

// Iterate from first to last and append c onto ss
// between each value in InputIterator
template <class InputIterator, class Function>
void join_values(InputIterator first, InputIterator last, std::string c, std::stringstream &ss, Function f);

class AstPrinter : public nth::Visitor {
 public:
  AstPrinter(bool pretty_print=false) : pretty_print(pretty_print) {}

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
  void visit(nth::BitShiftLeft *shift_left);
  void visit(nth::BitShiftRight *shift_right);
  void visit(nth::BitwiseOr *bitwise_or);
  void visit(nth::BitwiseAnd *bitwise_and);

  std::string getOutput();
 protected:
  bool pretty_print;
  std::stringstream ast_output;
};
