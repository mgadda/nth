//
//  ast_visitor.h
//  nth
//
//  Created by Matt Gadda on 8/3/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include "ast.h"

#ifndef nth_ast_visitor_h
#define nth_ast_visitor_h

namespace nth {

class Block;
class String;
class Integer;
class Float;
class True;
class False;
class Identifier;
class Array;
class Map;
class BinaryOperation;
class UnaryOperation;
class Add;
class Subtract;
class Multiply;
class Divide;
class Exponentiate;
class Modulo;
class BitShiftLeft;
class BitShiftRight;
class BitwiseOr;
class BitwiseAnd;
class LogicalOr;
class LogicalAnd;
class LogicalNot;
class Range;
class Tuple;
class Comparison;
  
class Visitor {
public:
  virtual void visit(Block *block)=0;
  virtual void visit(String *string)=0;
  virtual void visit(Integer *integer)=0;
  virtual void visit(Float *flt)=0;
  virtual void visit(True *tru)=0;
  virtual void visit(False *flse)=0;
  virtual void visit(Identifier *ident)=0;
  virtual void visit(Array *array)=0;
  virtual void visit(Map *map)=0;
  virtual void visit(BinaryOperation *bin_op)=0;
  virtual void visit(UnaryOperation *un_op)=0;
  virtual void visit(Add *add)=0;
  virtual void visit(Subtract *subtract)=0;
  virtual void visit(Multiply *multiply)=0;
  virtual void visit(Divide *divide)=0;
  virtual void visit(Exponentiate *exp)=0;
  virtual void visit(Modulo *modulo)=0;
  virtual void visit(BitShiftLeft *shift_left)=0;
  virtual void visit(BitShiftRight *shift_right)=0;
  virtual void visit(BitwiseOr *bitwise_or)=0;
  virtual void visit(BitwiseAnd *bitwise_and)=0;
  virtual void visit(LogicalOr *logical_or)=0;
  virtual void visit(LogicalAnd *logical_and)=0;
  virtual void visit(LogicalNot *logical_not)=0;
  virtual void visit(Range *range)=0;
  virtual void visit(Tuple *tuple)=0;
  virtual void visit(Comparison *comparison)=0;
};

class Visitable {
public:
  virtual void accept(Visitor &v)=0;
};

}

#endif
