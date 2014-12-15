//
//  type_checker.h
//  nth
//
//  Created by Matt Gadda on 11/6/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#ifndef __nth__type_checker__
#define __nth__type_checker__

#include <stdio.h>
#include "ast_visitor.h"

// Takes an alterantive approach to the visitor pattern
// An experiment to see if that makes sending information
// up and down the tree easier
namespace nth {

class Expression;

class TypeChecker : public Visitor {
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

 private:
  void setTypeForString(Expression *node, std::string typeName);
  void checkBinaryOp(BinaryOperation *bin_op, std::string symbolName);
  void checkUnaryOp(UnaryOperation *un_op, std::string symbolName);
};

}

#endif /* defined(__nth__type_checker__) */
