#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

#include "IntegerReplaceRandomizer.hpp"

using namespace llvm;

injectIntN(64)
injectIntN(32)
injectIntN(16)
injectIntN(8)

void
IntegerReplaceRandomizer::inject(Module& M)
{
	LLVMContext& ctx = M.getContext();

	/* Lookup i32 arc4random().. let it be declared. */
	Constant *lookupRand = M.getOrInsertFunction("arc4random", Type::getInt32Ty(ctx), NULL);
	if (lookupRand == NULL) {
		// XXX Handle better.
		errs() << "Unable to getFunction() for arc4random... sigh.\n";
		exit(-2);
	} else {
		//lookupRand->dump();
	}
	Function *fnRand = cast<Function>(lookupRand);

	injectInt64(M, fnRand);
	injectInt32(M, fnRand);
	injectInt16(M, fnRand);
	injectInt8(M, fnRand);
}
