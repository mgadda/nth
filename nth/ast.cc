//
//  ast.cpp
//  nth
//
//  Created by Matt Gadda on 6/7/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include "ast.h"
#include "type_literal.h"

using namespace nth;

void ASTNode::setParent(ASTNode *node) {
  if (!node) throw "cannot set parent node to null";
  _parent = node;
}

SymbolTable *ASTNode::getNearestSymbolTable() {
  SymbolTable *table;
  if ((table = getSymbolTable())) {
    return table;
  }
  ASTNode *parent;
  if ((parent = getParent())) {
    table = parent->getNearestSymbolTable();
    return table;
  }
  throw "invalid node: no sybmol table found";
}

Identifier *Identifier::forTemplatedType(std::string name, size_t subtypeCount) {
  return new nth::Identifier(name + std::to_string(subtypeCount));
}

Block::Block() {}

Block::Block(ASTNode *node) {
  nodes.push_back(node);
}

void Block::insertAfter(ASTNode *node) {
  nodes.push_back(node);
  node->setParent(this);
}

NodeList &Block::getNodes() {
  return nodes;
}

Array::Array(ExpressionList &exprList) : values(exprList) {
  for (auto expr : exprList) {
    expr->setParent(this);
  }
}

Map::Map(ExpressionMap &exprmap) : values(exprmap) {
  for (auto keyValue : exprmap) {
    keyValue.first->setParent(this);
    keyValue.second->setParent(this);
  }
}

UnaryOperation::UnaryOperation(ExpressionPtr value)
 : value(std::move(value)) {
  getValue()->setParent(this);
}

BinaryOperation::BinaryOperation(ExpressionPtr left, ExpressionPtr right)
 : left(std::move(left)), right(std::move(right)) {
  getLeftValue()->setParent(this);
  getRightValue()->setParent(this);
}

Range::Range(Integer *start, Integer *end, Exclusivity exclusivity)
  : start(start), end(end), exclusivity(exclusivity) {
  start->setParent(this);
  end->setParent(this);
}

Tuple::Tuple(ExpressionList &values) : values(values) {
  for (auto expr : values) {
    expr->setParent(this);
  }
}

Argument::Argument(Identifier *name, TypeRef *type) : name(name), type(type) {
  name->setParent(this);
  type->setParent(this);
}

FunctionDef::FunctionDef(Identifier *name, ArgList &argList,
                         TypeRef *returnType,  Block *block,
                         TypeDefList &typeParameters)
 : name(name), argList(argList), returnType(returnType), block(block),
   typeParameters(typeParameters) {

  name->setParent(this);
  for (auto arg : argList) {
   arg->setParent(this);
  }
  returnType->setParent(this);
  block->setParent(this);

  for (auto typeParam : typeParameters) {
   typeParam->setParent(this);
  }
}

LambdaDef::LambdaDef(ArgList &argList, TypeRef *returnType, Expression *body)
: argList(argList), returnType(returnType), body(body) {
  for (auto arg : argList) {
    arg->setParent(this);
  }
  returnType->setParent(this);
  body->setParent(this);
}

FunctionCall::FunctionCall(Expression *callable, ExpressionList &arguments)
: callable(callable), arguments(arguments) {
  callable->setParent(this);
  for (auto arg : arguments) {
    arg->setParent(this);
  }
}

VariableDef::VariableDef(Identifier *name, TypeRef *varType, Expression *value)
 : name(name), varType(varType), value(value) {
  name->setParent(this);
  varType->setParent(this);
  value->setParent(this);
}

IfElse::IfElse(Expression *condExpr, Block *ifBlock, Block *elseBlock)
: condExpr(condExpr), ifBlock(ifBlock), elseBlock(elseBlock) {
  condExpr->setParent(this);
  ifBlock->setParent(this);
  elseBlock->setParent(this);
}

TypeAliasDef::TypeAliasDef(TypeDef *lType, TypeRef *rType)
: lType(lType), rType(rType) {
  lType->setParent(this);
  rType->setParent(this);
}

void Add::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void Subtract::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void Multiply::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void Divide::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void Exponentiate::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void Modulo::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void BitShiftLeft::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void BitShiftRight::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void BitwiseOr::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void BitwiseAnd::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void BitwiseNot::accept(Visitor &v) {
  UnaryOperation::accept(v);
  v.visit(this);
}

void LogicalOr::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void LogicalAnd::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void LogicalNot::accept(Visitor &v) {
  UnaryOperation::accept(v);
  v.visit(this);
}

void Comparison::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void Subscript::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void FieldAccess::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void TupleFieldAccess::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}
