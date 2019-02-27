#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

#include "IntegerReplaceRandomizer.hpp"
#include "IntegerBaseBitFlipRandomizer.hpp"

using namespace llvm;

namespace {
	struct InjectRandomizers : public ModulePass {
		static char ID;

		InjectRandomizers() : ModulePass(ID) {}

		virtual bool
		runOnModule(Module &M)
		{
			IntegerReplaceRandomizer::inject(M);
			IntegerBaseBitFlipRandomizer::inject(M);
			return false;
		}
	};

	char InjectRandomizers::ID = 0;
	static RegisterPass<InjectRandomizers> XX("inject-randomizers", "Inject randomizer functions");
}
