#include <vector>
#include <map>
#include <set>
#include "type.h"
#include "symbol_table.h"

using namespace nth;

template <class T, class U>
void multi_for_each(std::list<T*> &left, std::list<U*> &right, std::function<void(T &t, U &u)> fun) {
  typename std::list<T*>::iterator leftIt;
  typename std::list<U*>::iterator rightIt;

  for (leftIt = left.begin(), rightIt = right.begin();
       leftIt != left.end() && rightIt != right.end();
       ++leftIt, ++rightIt) {
    fun(**leftIt, **rightIt);
  }

}

bool Type::operator==(Type *rhs) {
  return this == rhs || (
    *getName() == *(rhs->getName()) &&
    (rhs->getParents().empty() || getParents() == rhs->getParents()) &&
    getVariables() == rhs->getVariables() &&
    getMethods() == rhs->getMethods()
  );
}

bool Type::operator!=(Type *rhs) {
  return !(*this == rhs);
}

// TODO: this no longer works now that types have multiple parents
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
      if ((parent = typeVec[i]->getParents().front())) {
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
    _objectType = new SimpleType(new Identifier("Object"), VariableSet(), MethodSet(), TypeList());
  }
  return _objectType;
}

Type *Type::findMethodReturnTypeForArgs(std::string methodSymbolName, std::list<Type*> args) {
  for (auto &method : getMethods()) {
    // look for named symbol with type Function1[T1, R]
    // sepcialize that type into Function1[rightType, R]
    // that accepts a single argument of type right->getType()

    // TODO: clean this up
    if(*method == methodSymbolName) {
      FunctionType *methodType;

      if ((methodType = dynamic_cast<FunctionType*>(method->getType()))) {
        Identifier *expectedFunctionTypeName = Identifier::forTemplatedType("Function", args.size());

        if (*methodType->getName() == *expectedFunctionTypeName) {

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

  // no matches yet? try the parent types
  // starting with right most (lexically speaking) type
  // trait Foo: Bar, Baz, Qux
  // we look for the method in Qux, then Baz, finally Bar
  if (getParents().size() > 0) {
    for (auto parent : getParents()) {
      Type *match = parent->findMethodReturnTypeForArgs(methodSymbolName, args);
      if (match) return match;
    }
  }
  // TODO: actually save this Error and output to cout
  // instead of throwing exception
  throw "type checker: no method found with specified arguments";
}

TemplatedType::TemplatedType(
  Identifier*     name,
  VariableSet     variables,
  MethodSet       methods,
  TypeList        parents,
  GenericTypeList genericTypes)
: ConcreteType(name, variables, methods, parents),
  _genericTypes(genericTypes) {}

TemplatedType *TemplatedType::specialize(TypeList types) {
  if (types.size() != _genericTypes.size()) {
    throw "type checker: incorrect number of types during specialization";
  }

  GenericTypeList specializedTypes;

  multi_for_each<GenericType, Type>(_genericTypes, types,
      [&specializedTypes](GenericType &genericType, Type &type) {
    specializedTypes.push_back(genericType.specialize(&type));
  });

  auto type = new TemplatedType(getName(), variables, methods, getParents(), specializedTypes);
  return type;
}

bool TemplatedType::isSpecialized() {
  return std::all_of(_genericTypes.begin(), _genericTypes.end(), [](GenericType *type){
    return type->isSpecialized();
  });
}

GenericTypeList FunctionType::buildGenericTypes(TypeList argTypes, Type *returnType) {
  // build list of generic types (T1, T2, .. TN, R)
  // for each generic type, specialize immediately with the passed in arg types
  GenericTypeList typeList;
  int i=1;
  for (auto argType : argTypes) {
    Identifier *argPositionIdent = new Identifier(std::string("T") + std::to_string(i));
    typeList.push_back(new GenericType(argPositionIdent, argType));
    i++;
  }

  typeList.push_back(new GenericType(new Identifier("R"), returnType));
  return typeList;
}

GenericType *GenericType::specialize(Type *delegateType) {
  return new GenericType(getName(), delegateType);
}

FunctionType::FunctionType(TypeList argTypes, Type *returnType)
: TemplatedType(
    Identifier::forTemplatedType("Function", argTypes.size()),
    VariableSet(),
    MethodSet(),
    { Type::objectType() },
    FunctionType::buildGenericTypes(argTypes, returnType)) {
}

TypeList FunctionType::getArgTypes() {
  auto subtypes = getGenericTypes();
  return TypeList(subtypes.begin(), --subtypes.end());
}
Type *FunctionType::getReturnType() {
  return getGenericTypes().back();
}



