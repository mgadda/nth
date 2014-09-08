#include <iomanip>
#include <limits>

#include "ast_string_printer.h"
#include "type.h"

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

void AstStringPrinter::visit(nth::Block *block) {
  ast_output << "block(";

  auto values = block->getNodes();
  join_values(values.begin(), values.end(), ",", ast_output, [this](decltype(values)::value_type value) {
    value->accept(*this);
  });

  ast_output << ")";
}
void AstStringPrinter::visit(nth::String *string) {
  ast_output << "string(" << *string << ")";
}

void AstStringPrinter::visit(nth::Integer *integer) {
  ast_output << "integer(" << *integer << ")";
}

void AstStringPrinter::visit(nth::Float *flt) {
  ast_output << "float("
  << std::setprecision(std::numeric_limits<double>::digits10)
  << *flt
  << ")";
}

void AstStringPrinter::visit(nth::True *tru) {
  ast_output << "boolean(true)";
}

void AstStringPrinter::visit(nth::False *flse) {
  ast_output << "boolean(false)";
}

void AstStringPrinter::visit(nth::Identifier *ident) {
  ast_output << "ident(" << *ident << ")";
}

void AstStringPrinter::visit(nth::Array *array) {
  ast_output << "array(";

  auto values = array->getValues();
  join_values(values.begin(), values.end(), ",", ast_output, [this](nth::ExpressionList::value_type value) {

    value->accept(*this);
  });

  ast_output << ")";
}

void AstStringPrinter::visit(nth::Map *map) {
  ast_output << "map(";
  auto values = map->getValues();

  join_values(values.begin(), values.end(), ",", ast_output, [this](nth::ExpressionMap::value_type value) {

    value.first->accept(*this);
    ast_output << ": ";
    value.second->accept(*this);

  });

  ast_output << ")";
}

void AstStringPrinter::visit(nth::BinaryOperation *bin_op) {
  ast_output << "(";
  bin_op->getLeftValue()->accept(*this);
  ast_output << ",";
  bin_op->getRightValue()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(nth::UnaryOperation *un_op) {}

void AstStringPrinter::visit(nth::Add *add) {
  ast_output << "add";
}

void AstStringPrinter::visit(nth::Subtract *subtract) {
  ast_output << "subtract";
}

void AstStringPrinter::visit(nth::Multiply *multiply) {
  ast_output << "multiply";
}

void AstStringPrinter::visit(nth::Divide *divide) {
  ast_output << "divide";
}

void AstStringPrinter::visit(nth::Exponentiate *exp) {
  ast_output << "exp";
}

void AstStringPrinter::visit(nth::Modulo *modulo) {
  ast_output << "mod";
}

void AstStringPrinter::visit(nth::BitShiftLeft *shift_left) {
  ast_output << "bitshiftleft";
}

void AstStringPrinter::visit(nth::BitShiftRight *shift_right) {
  ast_output << "bitshiftright";
}

void AstStringPrinter::visit(nth::BitwiseOr *bitwise_or) {
  ast_output << "bitwiseor";
}

void AstStringPrinter::visit(nth::BitwiseAnd *bitwise_and) {
  ast_output << "bitwiseand";
}

void AstStringPrinter::visit(nth::BitwiseNot *bitwise_not) {
  ast_output << "bitwisenot(";
  bitwise_not->getValue()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(nth::LogicalOr *logical_or) {
  ast_output << "logicalor";
}

void AstStringPrinter::visit(nth::LogicalAnd *logical_and) {
  ast_output << "logicaland";
}

void AstStringPrinter::visit(nth::LogicalNot *logical_not) {
  ast_output << "logicalnot(";
  logical_not->getValue()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(nth::Range *range) {
  ast_output << "range(";
  range->getStart()->accept(*this);
  ast_output << ",";
  range->getEnd()->accept(*this);
  ast_output << ",";
  nth::Range::Exclusivity ex = range->getExclusivity();
  if (ex == nth::Range::Exclusivity::Exclusive)
    ast_output << "exclusive)";
  else if (ex == nth::Range::Exclusivity::Inclusive)
    ast_output << "inclusive)";
}

void AstStringPrinter::visit(nth::Tuple *tuple) {
  ast_output << "tuple(";

  auto values = tuple->getValues();
  join_values(values.begin(), values.end(), ",", ast_output, [this](nth::ExpressionList::value_type value) {
    value->accept(*this);
  });

  ast_output << ")";
}

void AstStringPrinter::visit(nth::Comparison *comparison) {
  std::string type;

  switch (comparison->getType()) {
    case nth::Comparison::Type::Equality:
      type = "equality";
      break;
    case nth::Comparison::Type::Inequality:
      type = "inequality";
      break;
    case nth::Comparison::Type::LessThan:
      type = "lessthan";
      break;
    case nth::Comparison::Type::GreaterThan:
      type = "greaterthan";
      break;
    case nth::Comparison::Type::LessThanOrEqualTo:
      type = "lessthanorequalto";
      break;
    case nth::Comparison::Type::GreaterThanOrEqualTo:
      type = "greaterthanorequalto";
      break;
  }

  ast_output << type;
}

void AstStringPrinter::visit(nth::Subscript *subscript) {
  ast_output << "subscript";
}

void AstStringPrinter::visit(nth::FieldAccess *field_access) {
  ast_output << "fieldaccess";
}

void AstStringPrinter::visit(nth::FunctionDef *functionDef) {
  ast_output << "funcdef(name(";
  functionDef->getName()->accept(*this);
  ast_output << "),arglist(";
  auto values = functionDef->getArguments();
  join_values(values.begin(), values.end(), ",", ast_output, [this](nth::ArgList::value_type value) {
    value->accept(*this);
  });
  ast_output << "),returning(";
  functionDef->getReturnType()->accept(*this);
  ast_output << "),";
  functionDef->getBlock()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(nth::LambdaDef *lambdaDef) {
  ast_output << "lambda(arglist(";
  auto values = lambdaDef->getArguments();
  join_values(values.begin(), values.end(), ",", ast_output, [this](nth::ArgList::value_type value) {
    value->accept(*this);
  });
  ast_output << "),returning(";
  lambdaDef->getReturnType()->accept(*this);
  ast_output << "),";
  lambdaDef->getBody()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(nth::FunctionCall *functionCall) {
  ast_output << "call(";
  functionCall->getCallable()->accept(*this);
  ast_output << ",arguments(";
  auto values = functionCall->getArguments();
  join_values(values.begin(), values.end(), ",", ast_output, [this](nth::ExpressionList::value_type value) {
    value->accept(*this);
  });
  ast_output << "))";
}

void AstStringPrinter::visit(nth::VariableDef *variableDef) {
  ast_output << "variabledef(name(";
  variableDef->getName()->accept(*this);
  ast_output << "),";
  variableDef->getVarType()->accept(*this);
  ast_output << ",";
  variableDef->getValue()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(nth::Argument *argument) {
  ast_output << "argument(";
  argument->getName()->accept(*this);
  ast_output << ",";
  argument->getType()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(nth::IfElse *ifElse) {
  ast_output << "ifelse(";
  ifElse->getCond()->accept(*this);
  ast_output << ",";
  ifElse->getIfBlock()->accept(*this);
  ast_output << ",";
  ifElse->getElseBlock()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(nth::SimpleType *type) {
  ast_output << "simple_type(";
  type->getName()->accept(*this);
  ast_output << ")";
}

void AstStringPrinter::visit(nth::TemplatedType *type) {
  ast_output << "templated_type(";
  type->getName()->accept(*this);
  ast_output << ",";
  auto values = type->getSubtypes();
  join_values(values.begin(), values.end(), ",", ast_output, [this](nth::TypeList::value_type value) {
    value->accept(*this);
  });

  ast_output << ")";
}

void AstStringPrinter::visit(nth::TypeAliasDef *typeAliasDef) {
  ast_output << "type_alias(";
  typeAliasDef->getLType()->accept(*this);
  ast_output << ",";
  typeAliasDef->getRType()->accept(*this);
  ast_output << ")";
}

