//
//  type_checker.cc
//  nth
//
//  Created by Matt Gadda on 11/6/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//
#include <set>

#include "type_checker.h"
#include "ast.h"
#include "type.h"
#include "symbol_table.h"

using namespace nth;

void TypeChecker::setTypeForString(Expression *node, std::string typeName) {
  Symbol *symbol = node->getNearestSymbolTable()->findSymbol(typeName);
  if (!symbol) throw std::string("type checker: unable to find symbol for '") + typeName + "'";
  node->setType(symbol->getType());
}

void TypeChecker::visit(Block *block) {
  SymbolTable *table = block->getNearestSymbolTable();
  if (!table) throw "type checker: no symbol table found";

  Symbol *sym = table->findSymbol("Object");
  if (!sym) throw "type checker: no symbol named Object found";
  
  Type *lastExprType = sym->getType();

  //Type *lastExprType = block->getNearestSymbolTable()->findSymbol("Object")->getType();

  for (auto node : block->getNodes()) {
    node->accept(*this);
    Expression *possibleExpr;
    if ((possibleExpr = dynamic_cast<Expression*>(node))) {
      lastExprType = possibleExpr->getType();
    }
  }

  block->setType(lastExprType);
}
void TypeChecker::visit(String *string) {
  setTypeForString(string, "String");
}

void TypeChecker::visit(Integer *integer) {
  setTypeForString(integer, "Integer");
}

void TypeChecker::visit(Float *flt) {
  setTypeForString(flt, "Float");
}

void TypeChecker::visit(True *tru) {
  setTypeForString(tru, "Boolean");
}

void TypeChecker::visit(False *flse) {
  setTypeForString(flse, "Boolean");
}

void TypeChecker::visit(Identifier *ident) {
}

void TypeChecker::visit(Array *array) {
  std::set<Type*> arrayElementTypes;
  for (auto value : array->getValues()) {
    value->accept(*this);
    arrayElementTypes.insert(value->getType());
  }
  Type *lubType = Type::computeLUB(arrayElementTypes);

  Symbol *arraySym = array->getNearestSymbolTable()->findSymbol(new Identifier("Array"));

  TemplatedType *arrayType = dynamic_cast<TemplatedType*>(arraySym->getType());
  TemplatedType *specializedType = arrayType->specialize({ lubType });

  array->setType(specializedType);
}

void TypeChecker::visit(Map *map) {
  std::set<Type*> mapKeyTypes;
  std::set<Type*> mapValueTypes;
  for (auto keyValue : map->getValues()) {
    keyValue.first->accept(*this);
    keyValue.second->accept(*this);
    mapKeyTypes.insert(keyValue.first->getType());
    mapValueTypes.insert(keyValue.second->getType());
  }
  Type *keyLUBType = Type::computeLUB(mapKeyTypes);
  Type *valueLUBType = Type::computeLUB(mapValueTypes);

  Symbol *mapSym = map->getNearestSymbolTable()->findSymbol(new Identifier("Map"));

  TemplatedType *mapType = dynamic_cast<TemplatedType*>(mapSym->getType());
  TemplatedType *specializedType = mapType->specialize({ keyLUBType, valueLUBType });

  map->setType(specializedType);
}

void TypeChecker::visit(BinaryOperation *bin_op) {}
void TypeChecker::checkBinaryOp(BinaryOperation *bin_op, std::string symbolName) {
  ExpressionPtr &left = bin_op->getLeftValue();
  ExpressionPtr &right = bin_op->getRightValue();
  left->accept(*this);
  right->accept(*this);

  Type *returnType = left->getType()->findMethodReturnTypeForArgs("+", { right->getType() });
  bin_op->setType(returnType);
}

void TypeChecker::visit(UnaryOperation *un_op) {}
void TypeChecker::checkUnaryOp(UnaryOperation *un_op, std::string symbolName) {
  ExpressionPtr &value = un_op->getValue();
  value->accept(*this);

  // TODO: raise appropriate errors here
  for(auto method : value->getType()->getMethods()) {
    if(*method == "!") {
      TemplatedType *methodType;
      if ((methodType = dynamic_cast<TemplatedType*>(method->getType()))) {
        if (*methodType->getName() == "Function0") {
          if (methodType->getSubtypes().size() == 1) {
            Type *returnType = methodType->getSubtypes().back();
            un_op->setType(returnType);
            return;
          }
        }
      }
    }
  }
}

void TypeChecker::visit(Add *add) {
  checkBinaryOp(add, "+");
}
void TypeChecker::visit(Subtract *subtract) {
  checkBinaryOp(subtract, "-");
}
void TypeChecker::visit(Multiply *multiply) {
  checkBinaryOp(multiply, "*");
}
void TypeChecker::visit(Divide *divide) {
  checkBinaryOp(divide, "/");
}
void TypeChecker::visit(Exponentiate *exp) {
  checkBinaryOp(exp, "^");
}
void TypeChecker::visit(Modulo *modulo) {
  checkBinaryOp(modulo, "%");
}
void TypeChecker::visit(BitShiftLeft *shift_left) {
  checkBinaryOp(shift_left, "<<");
}
void TypeChecker::visit(BitShiftRight *shift_right) {
  checkBinaryOp(shift_right, ">>");
}
void TypeChecker::visit(BitwiseOr *bitwise_or) {
  checkBinaryOp(bitwise_or, "|");
}
void TypeChecker::visit(BitwiseAnd *bitwise_and) {
  checkBinaryOp(bitwise_and, "&");
}
void TypeChecker::visit(BitwiseNot *bitwise_not) {
  checkUnaryOp(bitwise_not, "~");
}
void TypeChecker::visit(LogicalOr *logical_or) {
  checkBinaryOp(logical_or, "||");
}
void TypeChecker::visit(LogicalAnd *logical_and) {
  checkBinaryOp(logical_and, "&&");
}
void TypeChecker::visit(LogicalNot *logical_not) {
  checkUnaryOp(logical_not, "!");
}
void TypeChecker::visit(Range *range) {
  range->getStart()->accept(*this);
  range->getEnd()->accept(*this);
  Type *startType = range->getStart()->getType();
  Type *endType = range->getEnd()->getType();

  Type *intType = range->getNearestSymbolTable()->findSymbol("Int")->getType();

  if (*startType != intType || *endType != intType) {
    throw "type checker: range with invalid types";
  }

  Symbol *rangeSymbol = range->getNearestSymbolTable()->findSymbol("Range");
  if (!rangeSymbol)
    throw "type checker: Range symbol not found";

  Type *rangeType = rangeSymbol->getType();
  if (!rangeType)
    throw "type checker: Range type not found";

  range->setType(rangeType);
}

void TypeChecker::visit(Tuple *tuple) {}
void TypeChecker::visit(Comparison *comparison) {}
void TypeChecker::visit(Subscript *subscript) {}
void TypeChecker::visit(FieldAccess *field_access) {}
void TypeChecker::visit(TupleFieldAccess *tuple_field_access) {}
void TypeChecker::visit(FunctionDef *functionDef) {}
void TypeChecker::visit(LambdaDef *lambdaDef) {}
void TypeChecker::visit(FunctionCall *functionCall) {
  Expression *callableExpr = functionCall->getCallable();
  callableExpr->accept(*this);
  Type *callableExprType = callableExpr->getType();

  // callable expression must evaluate something with type
  // FunctionN[T0, .. TN-1, R] where N is equal to number of supplied arguments
  // the types of those arguments must also match the types
  // specified by FunctionN[T0, T1, ... TN-1, R]

  TemplatedType *callableFunctionType;
  Identifier *expectedFunctionTypeName =
    Identifier::forTemplatedType("Function", functionCall->getArguments().size());

  if ((callableFunctionType = dynamic_cast<TemplatedType*>(callableExprType))) {
    if (*callableFunctionType->getName() == *expectedFunctionTypeName) {
      Type *returnType = callableFunctionType->getSubtypes().back();
      functionCall->setType(returnType);
    }
  }
}
void TypeChecker::visit(VariableDef *variableDef) {}
void TypeChecker::visit(Argument *argument) {}
void TypeChecker::visit(IfElse *ifElse) {
  Expression *condExpr = ifElse->getCond();
  Block *ifBlock = ifElse->getIfBlock();
  Block *elseBlock = ifElse->getElseBlock();

  condExpr->accept(*this);
  ifBlock->accept(*this);
  if (elseBlock) elseBlock->accept(*this);

  // Check that if conditional expression evaluates to a boolean
  Type *condType = condExpr->getType();
  Symbol *boolSymbol = ifElse->getNearestSymbolTable()->findSymbol("Boolean");

  if (condType != boolSymbol->getType()) {
    throw "type checker: condition is not a boolean";
  }

  // Compute type for if-else expression
  Type *ifElseType;
  if (!elseBlock) {
    ifElseType = ifBlock->getType();
  } else {
    ifElseType = Type::computeLUB({ ifBlock->getType(), elseBlock->getType() });
  }
  ifElse->setType(ifElseType);
}

void TypeChecker::visit(SimpleTypeRef *type) {}
void TypeChecker::visit(SimpleTypeDef *type) {}
void TypeChecker::visit(TemplatedTypeRef *type) {}
void TypeChecker::visit(TemplatedTypeDef *type) {}
void TypeChecker::visit(TypeAliasDef *typeAliasDef) {}
