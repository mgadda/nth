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
class BitwiseNot;
class LogicalOr;
class LogicalAnd;
class LogicalNot;
class Range;
class Tuple;
class Comparison;
class Subscript;
class FieldAccess;
class TupleFieldAccess;
class FunctionDef;
class LambdaDef;
class FunctionCall;
class VariableDef;
class Argument;
class IfElse;

class SimpleTypeRef;
class SimpleTypeDef;
class TemplatedTypeRef;
class TemplatedTypeDef;
class TypeAliasDef;

class Visitor {
public:
  virtual void visit(Block *block);
  virtual void visit(String *string);
  virtual void visit(Integer *integer);
  virtual void visit(Float *flt);
  virtual void visit(True *tru);
  virtual void visit(False *flse);
  virtual void visit(Identifier *ident);
  virtual void visit(Array *array);
  virtual void visit(Map *map);
  virtual void visit(BinaryOperation *bin_op);
  virtual void visit(UnaryOperation *un_op);
  virtual void visit(Add *add);
  virtual void visit(Subtract *subtract);
  virtual void visit(Multiply *multiply);
  virtual void visit(Divide *divide);
  virtual void visit(Exponentiate *exp);
  virtual void visit(Modulo *modulo);
  virtual void visit(BitShiftLeft *shift_left);
  virtual void visit(BitShiftRight *shift_right);
  virtual void visit(BitwiseOr *bitwise_or);
  virtual void visit(BitwiseAnd *bitwise_and);
  virtual void visit(BitwiseNot *bitwise_not);
  virtual void visit(LogicalOr *logical_or);
  virtual void visit(LogicalAnd *logical_and);
  virtual void visit(LogicalNot *logical_not);
  virtual void visit(Range *range);
  virtual void visit(Tuple *tuple);
  virtual void visit(Comparison *comparison);
  virtual void visit(Subscript *subscript);
  virtual void visit(FieldAccess *field_access);
  virtual void visit(TupleFieldAccess *tuple_field_access);
  virtual void visit(FunctionDef *functionDef);
  virtual void visit(LambdaDef *lambdaDef);
  virtual void visit(FunctionCall *functionCall);
  virtual void visit(VariableDef *variableDef);
  virtual void visit(Argument *argument);
  virtual void visit(IfElse *ifElse);

  virtual void visit(SimpleTypeRef *type);
  virtual void visit(SimpleTypeDef *type);
  virtual void visit(TemplatedTypeRef *type);
  virtual void visit(TemplatedTypeDef *type);
  virtual void visit(TypeAliasDef *typeAliasDef);
};

class Visitable {
public:
  virtual void accept(Visitor &v)=0;
};

}

#endif
