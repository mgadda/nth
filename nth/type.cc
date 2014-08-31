#include "type.h"

using namespace nth;

TypeList FunctionType::concat_ctr_args(TypeList argList, Type *returnType) {
  argList.push_back(returnType);
  return argList;
}