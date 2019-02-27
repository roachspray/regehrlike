//#include "llvm/IR/Module.h"
//#include "llvm/IR/Function.h"
//#include "llvm/IR/InstVisitor.h"
//#include "llvm/Pass.h"
//#include "llvm/IR/InstIterator.h"
//#include "llvm/IR/CallSite.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"

#include "CodeModificationLocation.hpp"

using namespace llvm;


CallInst	*
CodeModificationLocation::getCallInst()
{
	return callLocation;
}

Type *
CodeModificationLocation::getArgumentType()
{
	return argumentType;
}

Value *
CodeModificationLocation::getArgumentValue()
{
	return argumentValue;
}

unsigned
CodeModificationLocation::getArgumentIdx()
{
	return argumentIdx;
}
