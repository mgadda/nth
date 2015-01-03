#include <iomanip>
#include <limits>

#include "ast_string_printer.h"
#include "type_literal.h"

namespace nth {

template <class InputIterator, class Function>
void join_values(InputIterator first, InputIterator last, std::string c, std::stringstream &ss, Function f)
{
  if (first == last) return;

  f(*first);
  ++first;
  for (; first != last; ++first) {
    ss << c;
    f(*first);
  }
}

std::string AstStringPrinter::getOutput() {
  return ast_output.str();
}

void AstStringPrinter::visit(Block *block) {
  ast_output << "block(";

  auto values = block->getNodes();
  join_values(values.begin(), values.end(), ",", ast_output, [this](decltype(values)::value_type value) {
    value->accept(*this);
  });

  ast_output << ")";
}
void AstStringPrinter::visit(String *string) {
  ast_output << "string(" << *string << ")";
}

void AstStringPrinter::visit(Integer *integer) {
  ast_output << "integer(" << *integer << ")";
}

void AstStringPrinter::visit(Float *flt) {
  ast_output << "float("
  << std::setprecision(std::numeric_limits<double>::digits10)
  << *flt
  << ")";
}

void AstStringPrinter::visit(True *tru) {
  ast_output << "boolean(true)";
}

void AstStringPrinter::visit(False *flse) {
  ast_output << "boolean(false)";
}

void AstStringPrinter::visit(Identifier *ident) {
  ast_output << "ident(" << *ident << ")";
}

void AstStringPrinter::visit(Array *array) {
  ast_output << "array(";

  auto values = array->getValues();
  join_values(values.begin(), values.end(), ",", ast_output, [this](ExpressionList::value_type value) {

    value->accept(*this);
  });

  ast_output << ")";
}

void AstStringPrinter::visit(Map *map) {
  ast_output << "map(";
  auto values = map->getValues();

  join_values(values.begin(), values.end(), ",", ast_output, [this](ExpressionMap::value_type value) {

    value.first->accept(*this);
    ast_output << ": ";
    value.second->accept(*this);

  });

  ast_output << ")";
}

void AstStringPrinter::visit(BinaryOperation *bin_op) {
  ast_output << "(";
  bin_op->getLeftValue()->accept(*this);
  ast_output << ",";
  bin_op->getRightValue()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(UnaryOperation *un_op) {}

void AstStringPrinter::visit(Add *add) {
  ast_output << "add";
}

void AstStringPrinter::visit(Subtract *subtract) {
  ast_output << "subtract";
}

void AstStringPrinter::visit(Multiply *multiply) {
  ast_output << "multiply";
}

void AstStringPrinter::visit(Divide *divide) {
  ast_output << "divide";
}

void AstStringPrinter::visit(Exponentiate *exp) {
  ast_output << "exp";
}

void AstStringPrinter::visit(Modulo *modulo) {
  ast_output << "mod";
}

void AstStringPrinter::visit(BitShiftLeft *shift_left) {
  ast_output << "bitshiftleft";
}

void AstStringPrinter::visit(BitShiftRight *shift_right) {
  ast_output << "bitshiftright";
}

void AstStringPrinter::visit(BitwiseOr *bitwise_or) {
  ast_output << "bitwiseor";
}

void AstStringPrinter::visit(BitwiseAnd *bitwise_and) {
  ast_output << "bitwiseand";
}

void AstStringPrinter::visit(BitwiseNot *bitwise_not) {
  ast_output << "bitwisenot(";
  bitwise_not->getValue()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(LogicalOr *logical_or) {
  ast_output << "logicalor";
}

void AstStringPrinter::visit(LogicalAnd *logical_and) {
  ast_output << "logicaland";
}

void AstStringPrinter::visit(LogicalNot *logical_not) {
  ast_output << "logicalnot(";
  logical_not->getValue()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(Range *range) {
  ast_output << "range(";
  range->getStart()->accept(*this);
  ast_output << ",";
  range->getEnd()->accept(*this);
  ast_output << ",";
  Range::Exclusivity ex = range->getExclusivity();
  if (ex == Range::Exclusivity::Exclusive)
    ast_output << "exclusive)";
  else if (ex == Range::Exclusivity::Inclusive)
    ast_output << "inclusive)";
}

void AstStringPrinter::visit(Tuple *tuple) {
  ast_output << "tuple(";

  auto values = tuple->getValues();
  join_values(values.begin(), values.end(), ",", ast_output, [this](ExpressionList::value_type value) {
    value->accept(*this);
  });

  ast_output << ")";
}

void AstStringPrinter::visit(Comparison *comparison) {
  std::string type;

  switch (comparison->getType()) {
    case Comparison::Type::Equality:
      type = "equality";
      break;
    case Comparison::Type::Inequality:
      type = "inequality";
      break;
    case Comparison::Type::LessThan:
      type = "lessthan";
      break;
    case Comparison::Type::GreaterThan:
      type = "greaterthan";
      break;
    case Comparison::Type::LessThanOrEqualTo:
      type = "lessthanorequalto";
      break;
    case Comparison::Type::GreaterThanOrEqualTo:
      type = "greaterthanorequalto";
      break;
  }

  ast_output << type;
}

void AstStringPrinter::visit(Subscript *subscript) {
  ast_output << "subscript";
}

void AstStringPrinter::visit(FieldAccess *field_access) {
  ast_output << "fieldaccess";
}

void AstStringPrinter::visit(TupleFieldAccess *tuple_field_access) {
  ast_output << "tuplefieldaccess";
}

void AstStringPrinter::visit(FunctionDef *functionDef) {
  ast_output << "funcdef(name(";
  functionDef->getName()->accept(*this);

  if (!functionDef->getTypeParameters().empty()) {
    ast_output << "),typeparamlist(";
    auto values = functionDef->getTypeParameters();

    join_values(values.begin(), values.end(), ",", ast_output, [this](TypeDefList::value_type value) {
      value->accept(*this);
    });
  }

  ast_output << "),arglist(";
  auto values = functionDef->getArguments();
  join_values(values.begin(), values.end(), ",", ast_output, [this](ArgList::value_type value) {
    value->accept(*this);
  });
  ast_output << "),returning(";
  functionDef->getReturnType()->accept(*this);
  ast_output << "),";
  functionDef->getBlock()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(LambdaDef *lambdaDef) {
  ast_output << "lambda(arglist(";
  auto values = lambdaDef->getArguments();
  join_values(values.begin(), values.end(), ",", ast_output, [this](ArgList::value_type value) {
    value->accept(*this);
  });
  ast_output << "),returning(";
  lambdaDef->getReturnType()->accept(*this);
  ast_output << "),";
  lambdaDef->getBody()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(FunctionCall *functionCall) {
  ast_output << "call(";
  functionCall->getCallable()->accept(*this);
  ast_output << ",arguments(";
  auto values = functionCall->getArguments();
  join_values(values.begin(), values.end(), ",", ast_output, [this](ExpressionList::value_type value) {
    value->accept(*this);
  });
  ast_output << "))";
}

void AstStringPrinter::visit(VariableDef *variableDef) {
  ast_output << "variabledef(name(";
  variableDef->getName()->accept(*this);
  ast_output << "),";
  variableDef->getVarType()->accept(*this);
  ast_output << ",";
  variableDef->getValue()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(Argument *argument) {
  ast_output << "argument(";
  argument->getName()->accept(*this);
  ast_output << ",";
  argument->getTypeRef()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(IfElse *ifElse) {
  ast_output << "ifelse(";
  ifElse->getCond()->accept(*this);
  ast_output << ",";
  ifElse->getIfBlock()->accept(*this);
  ast_output << ",";
  ifElse->getElseBlock()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(SimpleTypeRef *type) {
  ast_output << "simple_typeref(";
  type->getName()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(TemplatedTypeRef *type) {
  ast_output << "templated_typeref(";
  type->getName()->accept(*this);
  ast_output << ",";
  auto values = type->getSubtypes();
  join_values(values.begin(), values.end(), ",", ast_output, [this](TypeRefList::value_type value) {
    value->accept(*this);
  });

  ast_output << ")";
}

void AstStringPrinter::visit(SimpleTypeDef *type) {
  ast_output << "simple_typedef(";
  type->getName()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(TemplatedTypeDef *type) {
  ast_output << "templated_typedef(";
  type->getName()->accept(*this);
  ast_output << ",";
  auto values = type->getSubtypes();
  join_values(values.begin(), values.end(), ",", ast_output, [this](TypeDefList::value_type value) {
    value->accept(*this);
  });

  ast_output << ")";
}


void AstStringPrinter::visit(TypeAliasDef *typeAliasDef) {
  ast_output << "type_alias(";
  typeAliasDef->getLType()->accept(*this);
  ast_output << ",";
  typeAliasDef->getRType()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(TraitDef *traitDef) {
  ast_output << "trait_def(name(";
  traitDef->getName()->accept(*this);
  ast_output << ")";

  if (traitDef->getTypeParameters() && !traitDef->getTypeParameters()->empty()) {
    ast_output << ",typeparamlist(";
    auto values = *traitDef->getTypeParameters();

    join_values(values.begin(), values.end(), ",", ast_output, [this](TypeDefList::value_type value) {
      value->accept(*this);
    });
    ast_output << ")";
  }

  if (traitDef->getCtorArgs() && !traitDef->getCtorArgs()->empty()) {
    ast_output << ",ctor_arglist(";
    auto values = *traitDef->getCtorArgs();
    join_values(values.begin(), values.end(), ",", ast_output, [this](ArgList::value_type value) {
      value->accept(*this);
    });
    ast_output << ")";
  }

  if (traitDef->getBlock()) {
    ast_output << ",";
    traitDef->getBlock()->accept(*this);
  }

  ast_output << ")";
}

}