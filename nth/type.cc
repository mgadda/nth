#include "type.h"

using namespace nth;

TypeRefList FunctionType::concat_ctr_args(TypeRefList argList, TypeRef *returnType) {
  argList.push_back(returnType);
  return argList;
}