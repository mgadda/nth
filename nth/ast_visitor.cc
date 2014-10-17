//
//  ast_Visitor::visitor.cc
//  nth
//
//  Created by Matt Gadda on 10/2/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include "ast_visitor.h"
#include "type.h"

using namespace nth;

// Provides default implementation which traverses all nodes
// so that concrete classes only have to implement the parts
// they care about

void Visitor::visit(Block *block) {
  for (auto node : block->getNodes()) {
    node->accept(*this);
  }
}

void Visitor::visit(String *string) {}
void Visitor::visit(Integer *integer) {}
void Visitor::visit(Float *flt) {}
void Visitor::visit(True *tru) {}
void Visitor::visit(False *flse) {}
void Visitor::visit(Identifier *ident) {}

void Visitor::visit(Array *array) {
  for (auto value : array->getValues()) {
    value->accept(*this);
  }
}

void Visitor::visit(Map *map) {
  for (auto keyValue : map->getValues()) {
    keyValue.second->accept(*this);
    keyValue.first->accept(*this);
  }
}

void Visitor::visit(BinaryOperation *bin_op) {
  bin_op->getLeftValue()->accept(*this);
  bin_op->getRightValue()->accept(*this);
}

void Visitor::visit(UnaryOperation *un_op) {
  un_op->getValue()->accept(*this);
}

// Binary operations:
void Visitor::visit(Add *add) {}
void Visitor::visit(Subtract *subtract) {}
void Visitor::visit(Multiply *multiply) {}
void Visitor::visit(Divide *divide) {}
void Visitor::visit(Exponentiate *exp) {}
void Visitor::visit(Modulo *modulo) {}
void Visitor::visit(BitShiftLeft *shift_left) {}
void Visitor::visit(BitShiftRight *shift_right) {}
void Visitor::visit(BitwiseOr *bitwise_or) {}
void Visitor::visit(BitwiseAnd *bitwise_and) {}
void Visitor::visit(LogicalOr *logical_or) {}
void Visitor::visit(LogicalAnd *logical_and) {}

// Unary operations:
void Visitor::visit(BitwiseNot *bitwise_not) {}
void Visitor::visit(LogicalNot *logical_not) {}

void Visitor::visit(Range *range) {
  range->getStart()->accept(*this);
  range->getEnd()->accept(*this);
}

void Visitor::visit(Tuple *tuple) {
  for (auto value : tuple->getValues()) {
    value->accept(*this);
  }
}

// Binary operations:
void Visitor::visit(Comparison *comparison) {}
void Visitor::visit(Subscript *subscript) {}
void Visitor::visit(FieldAccess *field_access) {}
void Visitor::visit(TupleFieldAccess *tuple_field_access) {}

void Visitor::visit(FunctionDef *functionDef) {
  functionDef->getName()->accept(*this);
  for (auto typeParam : functionDef->getTypeParameters()) {
    typeParam->accept(*this);
  }
  for (auto arg : functionDef->getArguments()) {
    arg->accept(*this);
  }
  functionDef->getReturnType()->accept(*this);
  functionDef->getBlock()->accept(*this);
}

void Visitor::visit(LambdaDef *lambdaDef) {
  for (auto arg : lambdaDef->getArguments()) {
    arg->accept(*this);
  }
  lambdaDef->getReturnType()->accept(*this);
  lambdaDef->getBody()->accept(*this);
}

void Visitor::visit(FunctionCall *functionCall) {
  functionCall->getCallable()->accept(*this);
  for (auto argExpr : functionCall->getArguments()) {
    argExpr->accept(*this);
  }
}

void Visitor::visit(VariableDef *variableDef) {
  variableDef->getName()->accept(*this);
  variableDef->getVarType()->accept(*this);
  variableDef->getValue()->accept(*this);
}

void Visitor::visit(Argument *argument) {
  argument->getName()->accept(*this);
  argument->getType()->accept(*this);
}

void Visitor::visit(IfElse *ifElse) {
  ifElse->getCond()->accept(*this);
  ifElse->getIfBlock()->accept(*this);
  ifElse->getElseBlock()->accept(*this);
}

void Visitor::visit(SimpleTypeRef *type) {
  type->getName()->accept(*this);
}

void Visitor::visit(SimpleTypeDef *type) {
  type->getName()->accept(*this);
}

void Visitor::visit(TemplatedTypeRef *type) {
  type->getName()->accept(*this);
  for (auto subtype : type->getSubtypes()) {
    subtype->accept(*this);
  }
}

void Visitor::visit(TemplatedTypeDef *type) {
  type->getName()->accept(*this);
  for (auto subtype : type->getSubtypes()) {
    subtype->accept(*this);
  }
}

void Visitor::visit(TypeAliasDef *typeAliasDef) {
  typeAliasDef->getLType()->accept(*this);
  typeAliasDef->getRType()->accept(*this);
}