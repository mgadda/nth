#include <vector>
#include <map>
#include <set>
#include "type.h"
#include "symbol_table.h"

using namespace nth;

Type::Type(Identifier *name, VariableSet variables, MethodSet methods, Type *parent)
: name(name), variables(variables), methods(methods), parent(parent) {}

bool Type::operator==(Type &rhs) {
  return *this == &rhs;
}

bool Type::operator==(Type *rhs) {
  return *name == *(rhs->name) &&
  (!rhs->parent || *parent == *(rhs->parent)) &&
  variables == rhs->variables &&
  methods == rhs->methods;
}

bool Type::operator!=(Type &rhs) {
  return *this != &rhs;
}

bool Type::operator!=(Type *rhs) {
  return !(*this == rhs);
}

Type *Type::computeLUB(std::set<Type*> types) {
  std::map<Type*, int> visitCounts;
  std::vector<bool> terminated(types.size(), false);

  const int64_t kColors = types.size();

  // TODO: eliminate use of vector, use set directly
  std::vector<Type*> typeVec(types.begin(), types.end());

  while (!std::all_of(terminated.begin(), terminated.end(), [](bool val) {
    return val; })) {

    for (size_t i=0; i<types.size(); i++) {
      visitCounts[typeVec[i]] += 1;
      if (visitCounts[typeVec[i]] == kColors) {
        return typeVec[i];
      }
      Type *parent;
      if ((parent = typeVec[i]->getParent())) {
        typeVec[i] = parent;
      } else {
        terminated[i] = true;
      }
    }
  }

  throw "type checker: no common ancestor for specified types. this shouldn't happen.";
}

Type *Type::_objectType = nullptr;

Type *Type::objectType() {
  if (!_objectType) {
    // TODO: give Object some useful methods
    _objectType = new SimpleType(new Identifier("Object"), VariableSet(), MethodSet(), nullptr);
  }
  return _objectType;
}

Type *Type::findMethodReturnTypeForArgs(std::string methodSymbolName, std::list<Type*> args) {
  for (auto &method : methods) {
    // look for named symbol with type Function1[T1, R]
    // sepcialize that type into Function1[rightType, R]
    // that accepts a single argument of type right->getType()

    // TODO: clean this up
    if(*method == methodSymbolName) {
      FunctionType *methodType;

      if ((methodType = dynamic_cast<FunctionType*>(method->getType()))) {
        Identifier *expectedFunctionTypeName = Identifier::forTemplatedType("Function", args.size());

        if (*methodType->getName() == *expectedFunctionTypeName) {

//          auto subtypes = methodType->getSubtypes();
//          Type *returnType = subtypes.back();
//          decltype(subtypes) methodArgTypes(subtypes.begin(), --subtypes.end());

          Type *returnType = methodType->getReturnType();
          TypeList methodArgTypes = methodType->getArgTypes();
          
          // nullary methods match on name alone
          if (methodArgTypes.size() == 0 && args.size() == 0) {
            return returnType;
          }

          if (methodArgTypes.size() == args.size()) {
            auto mismatch = std::mismatch(methodArgTypes.begin(), methodArgTypes.end(), args.begin(), [](Type *t1, Type *t2) {
              return *t1 == t2;
            });

            if (mismatch.first == methodArgTypes.end() && mismatch.second == args.end()) {
              return returnType;
            }
          }
        }
      }
    }
  }

  // no matches yet? try the parent type
  if (parent) {
    return parent->findMethodReturnTypeForArgs(methodSymbolName, args);
  }
  // TODO: actually save this Error and output to cout
  // instead of throwing exception
  throw "type checker: no method found with specified arguments";
}

TemplatedType *TemplatedType::specialize(TypeList subtypes) {
  auto type = new TemplatedType(getName(), variables, methods, getParent(), subtypes);
  type->specialized = true;
  return type;
}

TypeList FunctionType::concat_ctr_args(TypeList argList, Type *returnType) {
  argList.push_back(returnType);
  return argList;
}

FunctionType::FunctionType(TypeList argTypes, Type *returnType)
: TemplatedType(
    Identifier::forTemplatedType("Function", argTypes.size()),
    VariableSet(),
    MethodSet(),
    Type::objectType(),
    FunctionType::concat_ctr_args(argTypes, returnType)) {
}

TypeList FunctionType::getArgTypes() {
  auto subtypes = getSubtypes();
  return TypeList(subtypes.begin(), --subtypes.end());
}
Type *FunctionType::getReturnType() {
  return getSubtypes().back();
}



