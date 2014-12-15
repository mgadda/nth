//
//  type_literal.cc
//  nth
//
//  Created by Matt Gadda on 12/14/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include "type_literal.h"

using namespace nth;

TypeRefList FunctionTypeRef::concat_ctr_args(TypeRefList argList, TypeRef *returnType) {
  argList.push_back(returnType);
  return argList;
}