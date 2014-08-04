//
//  objc_helper.m
//  nth
//
//  Created by Matt Gadda on 7/13/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include "test_helper.h"
#include "ast.h"

#ifdef IS_XCODE
#include <CoreFoundation/CoreFoundation.h>
std::string getResourcePath() {
  CFBundleRef mainBundle = CFBundleGetMainBundle();

  CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(mainBundle);

  // Convert the URL reference into a string reference
  CFStringRef resourcePath = CFURLCopyFileSystemPath(resourceURL, kCFURLPOSIXPathStyle);

  // Get the system encoding method
  CFStringEncoding encodingMethod = CFStringGetSystemEncoding();

  // Convert the string reference into a C string
  const char *path = CFStringGetCStringPtr(resourcePath, encodingMethod);

  return std::string(path);
}
#else
std::string getResourcePath() {
  return std::string("../");
}
#endif


std::string AstPrinter::getOutput() {
  return ast_output.str();
}

void AstPrinter::visit(nth::Block *block) {
  ast_output << "block(";
  for(auto expr : block->getExpressions()) {
    expr->accept(*this);
    ast_output << ", ";
  }
  ast_output << ")";
}
void AstPrinter::visit(nth::String *string) {
  ast_output << "string(" << *string << ")";
}

void AstPrinter::visit(nth::Integer *integer) {
  ast_output << "integer(" << *integer << ")";
}

void AstPrinter::visit(nth::Float *flt) {
  ast_output << "float(" << *flt << ")";
}

void AstPrinter::visit(nth::True *tru) {
  ast_output << "true";
}

void AstPrinter::visit(nth::False *flse) {
  ast_output << "false";
}

void AstPrinter::visit(nth::Identifier *ident) {
  ast_output << "ident(" << *ident << ")";
}

void AstPrinter::visit(nth::Array *array) {
  ast_output << "array(";
  for (auto expr : array->getValues()) {
    expr->accept(*this);
  }
  ast_output << ")";
}

void AstPrinter::visit(nth::Map *map) {
  ast_output << "map(";
  for (auto key_value_pair : map->getValues()) {
    key_value_pair.first->accept(*this);
    ast_output << ": ";
    key_value_pair.second->accept(*this);
  }
  ast_output << ")";
}

void AstPrinter::visit(nth::Add *add) {
  ast_output << "add(";
  add->getLeftValue()->accept(*this);
  ast_output << ", ";
  add->getRightValue()->accept(*this);
  ast_output << ")";
}

void AstPrinter::visit(nth::Subtract *subtract) {
  ast_output << "subtract(";
  subtract->getLeftValue()->accept(*this);
  ast_output << ", ";
  subtract->getRightValue()->accept(*this);
  ast_output << ")";
}

void AstPrinter::visit(nth::Multiply *multiply) {
  ast_output << "multiply(";
  multiply->getLeftValue()->accept(*this);
  ast_output << ", ";
  multiply->getRightValue()->accept(*this);
  ast_output << ")";
}

void AstPrinter::visit(nth::Divide *divide) {
  ast_output << "divide(";
  divide->getLeftValue()->accept(*this);
  ast_output << ", ";
  divide->getRightValue()->accept(*this);
  ast_output << ")";
}

void AstPrinter::visit(nth::Exponentiate *exp) {
  ast_output << "exp(";
  exp->getLeftValue()->accept(*this);
  ast_output << ", ";
  exp->getRightValue()->accept(*this);
  ast_output << ")";
}

void AstPrinter::visit(nth::Modulo *modulo) {
  ast_output << "mod(";
  modulo->getLeftValue()->accept(*this);
  ast_output << ", ";
  modulo->getRightValue()->accept(*this);
  ast_output << ")";
}

void AstPrinter::visit(nth::BitShiftLeft *shift_left) {
  ast_output << "bitshiftleft(";
  shift_left->getLeftValue()->accept(*this);
  ast_output << ", ";
  shift_left->getRightValue()->accept(*this);
  ast_output << ")";
}

void AstPrinter::visit(nth::BitShiftRight *shift_right) {
  ast_output << "bitshiftright(";
  shift_right->getLeftValue()->accept(*this);
  ast_output << ", ";
  shift_right->getRightValue()->accept(*this);
  ast_output << ")";
}

void AstPrinter::visit(nth::BitwiseOr *bitwise_or) {
  ast_output << "bitwiseor(";
  bitwise_or->getLeftValue()->accept(*this);
  ast_output << ", ";
  bitwise_or->getRightValue()->accept(*this);
  ast_output << ")";
}

void AstPrinter::visit(nth::BitwiseAnd *bitwise_and) {
  ast_output << "bitwiseand(";
  bitwise_and->getLeftValue()->accept(*this);
  ast_output << ", ";
  bitwise_and->getRightValue()->accept(*this);
  ast_output << ")";
}

