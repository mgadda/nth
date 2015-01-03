//
//  ast_dot_printer.cc
//  nth
//
//  Created by Matt Gadda on 8/14/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include "ast_dot_printer.h"
#include "type_literal.h"
#include "ast.h"

namespace nth {

DummyNode *AstDotPrinter::makeDummy(std::string name, ASTNode *parent) {
  DummyNode *dummy = new DummyNode();
  edges.push_back(std::make_pair(parent, dummy));
  nodes[dummy] = name;
  return dummy;
}

AstDotPrinter::AstDotPrinter() {
}

std::string AstDotPrinter::getOutput() {
  ast_output << "digraph ast {\nordering=out;\n";

  for (auto node : nodes) {

    ast_output << (int64_t)node.first;
    ast_output << " [label=\"" << node.second << "\"";
    if (node.second == "simple_type" || node.second == "templated_type") {
      ast_output << " shape=box";
    }
    ast_output << "];\n";
  }
  for (auto edge : edges) {
    ast_output << (int64_t)edge.first
               << " -> "
               << (int64_t)edge.second
               << ";\n";
  }
  ast_output << "}";
  return ast_output.str();
}

void AstDotPrinter::visit(Block *block) {
  nodes[block] = "block";

  for (auto child : block->getNodes()) {
    if (child) {
      edges.push_back(std::make_pair(block, child));
      child->accept(*this);
    }
  }
}

void AstDotPrinter::visit(String *string) {
  nodes[string] = string->getValue();
}

void AstDotPrinter::visit(Integer *integer) {
  nodes[integer] = std::to_string(integer->getValue());
}

void AstDotPrinter::visit(Float *flt) {
  nodes[flt] = std::to_string(flt->getValue());
}

void AstDotPrinter::visit(True *tru) {
  nodes[tru] = "true";
}

void AstDotPrinter::visit(False *flse) {
  nodes[flse] = "false";
}

void AstDotPrinter::visit(Identifier *ident) {
  nodes[ident] = ident->getValue();
}

void AstDotPrinter::visit(Array *array) {
  nodes[array] = "array";

  for (auto child : array->getValues()) {
    if (child) {
      edges.push_back(std::make_pair(array, child));
      child->accept(*this);
    }
  }
}

void AstDotPrinter::visit(Map *map) {
  nodes[map] = "map";

  for (auto child : map->getValues()) {
    edges.push_back(std::make_pair(map, child.first));
    edges.push_back(std::make_pair(child.first, child.second));
    child.first->accept(*this);
    child.second->accept(*this);
  }
}

void AstDotPrinter::visit(BinaryOperation *bin_op) {
  ExpressionPtr &left = bin_op->getLeftValue();
  ExpressionPtr &right = bin_op->getRightValue();

  edges.push_back(std::make_pair(bin_op, right.get()));
  edges.push_back(std::make_pair(bin_op, left.get()));

  left->accept(*this);
  right->accept(*this);
}

void AstDotPrinter::visit(UnaryOperation *un_op) {
  ExpressionPtr &value = un_op->getValue();

  edges.push_back(std::make_pair(un_op, value.get()));

  value->accept(*this);
}

void AstDotPrinter::visit(Add *add) {
  nodes[add] = "+";
}

void AstDotPrinter::visit(Subtract *subtract) {
  nodes[subtract] = "-";
}

void AstDotPrinter::visit(Multiply *multiply) {
  nodes[multiply] = "*";
}

void AstDotPrinter::visit(Divide *divide) {
  nodes[divide] = "/";
}

void AstDotPrinter::visit(Exponentiate *exp) {
  nodes[exp] = "^";
}

void AstDotPrinter::visit(Modulo *modulo) {
  nodes[modulo] = "%";
}

void AstDotPrinter::visit(BitShiftLeft *shift_left) {
  nodes[shift_left] = "<<";
}

void AstDotPrinter::visit(BitShiftRight *shift_right) {
  nodes[shift_right] = ">>";
}

void AstDotPrinter::visit(BitwiseOr *bitwise_or) {
  nodes[bitwise_or] = "|";
}

void AstDotPrinter::visit(BitwiseAnd *bitwise_and) {
  nodes[bitwise_and] = "&";
}

void AstDotPrinter::visit(BitwiseNot *bitwise_not) {
  nodes[bitwise_not] = "~";
}

void AstDotPrinter::visit(LogicalOr *logical_or) {
  nodes[logical_or] = "||";
}

void AstDotPrinter::visit(LogicalAnd *logical_and) {
  nodes[logical_and] = "&&";
}

void AstDotPrinter::visit(LogicalNot *logical_not) {
  nodes[logical_not] = "!";
}

void AstDotPrinter::visit(Range *range) {
  nodes[range] = "range";

  edges.push_back(std::make_pair(range, range->getStart()));
  edges.push_back(std::make_pair(range, range->getEnd()));

  range->getStart()->accept(*this);
  range->getEnd()->accept(*this);
}

void AstDotPrinter::visit(Tuple *tuple) {
  nodes[tuple] = "tuple";

  for (auto value : tuple->getValues()) {
    edges.push_back(std::make_pair(tuple, value));
    value->accept(*this);
  }
}

void AstDotPrinter::visit(Comparison *comparison) {
  std::string type;

  switch (comparison->getType()) {
    case Comparison::Type::Equality:
      type = "==";
      break;
    case Comparison::Type::Inequality:
      type = "!=";
      break;
    case Comparison::Type::LessThan:
      type = "<";
      break;
    case Comparison::Type::GreaterThan:
      type = ">";
      break;
    case Comparison::Type::LessThanOrEqualTo:
      type = "<=";
      break;
    case Comparison::Type::GreaterThanOrEqualTo:
      type = ">=";
      break;
  }

  nodes[comparison] = type;
}

void AstDotPrinter::visit(Subscript *subscript) {
  nodes[subscript] = "subscript";

}

void AstDotPrinter::visit(FieldAccess *field_access) {
  nodes[field_access] = "field_access";
}

void AstDotPrinter::visit(TupleFieldAccess *tuple_field_access) {
  nodes[tuple_field_access] = "tuple_field_access";
}

void AstDotPrinter::visit(FunctionDef *functionDef) {
  nodes[functionDef] = "function_def";

  edges.push_back(std::make_pair(functionDef, functionDef->getName()));
  functionDef->getName()->accept(*this);

  // type parameter list
  DummyNode *dummy = makeDummy("type_list", functionDef);
  for (auto value : functionDef->getTypeParameters()) {
    edges.push_back(std::make_pair(dummy, value));
    value->accept(*this);
  }

  // arguments
  dummy = makeDummy("arguments", functionDef);
  for (auto value : functionDef->getArguments()) {
    edges.push_back(std::make_pair(dummy, value));
    value->accept(*this);
  }

  edges.push_back(std::make_pair(functionDef, functionDef->getReturnType()));
  functionDef->getReturnType()->accept(*this);

  edges.push_back(std::make_pair(functionDef, functionDef->getBlock()));
  functionDef->getBlock()->accept(*this);

}

void AstDotPrinter::visit(LambdaDef *lambdaDef) {
  nodes[lambdaDef] = "lambda_def";

  for (auto value : lambdaDef->getArguments()) {
    edges.push_back(std::make_pair(lambdaDef, value));
    value->accept(*this);
  }

  edges.push_back(std::make_pair(lambdaDef, lambdaDef->getReturnType()));
  lambdaDef->getReturnType()->accept(*this);

  edges.push_back(std::make_pair(lambdaDef, lambdaDef->getBody()));
  lambdaDef->getBody()->accept(*this);

}

void AstDotPrinter::visit(FunctionCall *functionCall) {
  nodes[functionCall] = "call";
  edges.push_back(std::make_pair(functionCall, functionCall->getCallable()));
  functionCall->getCallable()->accept(*this);

  for (auto value : functionCall->getArguments()) {
    edges.push_back(std::make_pair(functionCall, value));
    value->accept(*this);
  }
}

void AstDotPrinter::visit(VariableDef *variableDef) {
  nodes[variableDef] = "val_def";

  edges.push_back(std::make_pair(variableDef, variableDef->getName()));
  variableDef->getName()->accept(*this);

  edges.push_back(std::make_pair(variableDef, variableDef->getVarType()));
  variableDef->getVarType()->accept(*this);

  edges.push_back(std::make_pair(variableDef, variableDef->getValue()));
  variableDef->getValue()->accept(*this);
}

void AstDotPrinter::visit(Argument *argument) {
  nodes[argument] = "argument_def";

  edges.push_back(std::make_pair(argument, argument->getName()));
  argument->getName()->accept(*this);

  edges.push_back(std::make_pair(argument, argument->getTypeRef()));
  argument->getTypeRef()->accept(*this);
}

void AstDotPrinter::visit(IfElse *ifElse) {
  nodes[ifElse] = "ifelse";

  edges.push_back(std::make_pair(ifElse, ifElse->getCond()));
  ifElse->getCond()->accept(*this);

  edges.push_back(std::make_pair(ifElse, ifElse->getIfBlock()));
  ifElse->getIfBlock()->accept(*this);

  edges.push_back(std::make_pair(ifElse, ifElse->getElseBlock()));
  ifElse->getElseBlock()->accept(*this);
}

void AstDotPrinter::visit(SimpleTypeRef *type) {
  nodes[type] = "simple_typeref";

  edges.push_back(std::make_pair(type, type->getName()));
  type->getName()->accept(*this);
}

void AstDotPrinter::visit(TemplatedTypeRef *type) {
  nodes[type] = "templated_typeref";

  edges.push_back(std::make_pair(type, type->getName()));
  type->getName()->accept(*this);

  for (auto value : type->getSubtypes()) {
    edges.push_back(std::make_pair(type, value));
    value->accept(*this);
  }
}

void AstDotPrinter::visit(SimpleTypeDef *type) {
  nodes[type] = "simple_typedef";

  edges.push_back(std::make_pair(type, type->getName()));
  type->getName()->accept(*this);
}

void AstDotPrinter::visit(TemplatedTypeDef *type) {
  nodes[type] = "templated_typedef";

  edges.push_back(std::make_pair(type, type->getName()));
  type->getName()->accept(*this);

  for (auto value : type->getSubtypes()) {
    edges.push_back(std::make_pair(type, value));
    value->accept(*this);
  }
}

void AstDotPrinter::visit(TypeAliasDef *typeAliasDef) {
  nodes[typeAliasDef] = "type_alias_def";

  edges.push_back(std::make_pair(typeAliasDef, typeAliasDef->getLType()));
  typeAliasDef->getLType()->accept(*this);

  edges.push_back(std::make_pair(typeAliasDef, typeAliasDef->getRType()));
  typeAliasDef->getRType()->accept(*this);

}

}
