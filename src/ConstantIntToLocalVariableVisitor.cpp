#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Pass.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/SymbolTableListTraits.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
	struct ConstantIntToLocalVariableVisitor : public ModulePass {
		static char ID;
		ConstantIntToLocalVariableVisitor() : ModulePass(ID) {}

		virtual bool
		runOnModule(Module &M)
		{
			for (auto &f : M) {
				CheckCallInsts cCheck;
				cCheck.visit(f);
			}
			return false;
		}

		struct CheckCallInsts : public InstVisitor<CheckCallInsts> {
			void
			visitCallInst(CallInst &CI)
			{
				unsigned numArgOps = CI.getNumArgOperands();
				for (unsigned ii = 0; ii < numArgOps; ii++) {
					Value *va = CI.getArgOperand(ii);

					if (ConstantInt *con = dyn_cast<ConstantInt>(va)) {
							unsigned nBits = con->getBitWidth();	
							/* allocate local and store constant int value to it */
							AllocaInst *__zap_localized__alloc = new AllocaInst(IntegerType::get(CI.getContext(), nBits), "__zap_localized_"+std::to_string(new_vars));
							StoreInst *__zap_localized__store = new StoreInst(con, __zap_localized__alloc, false);
							CI.getParent()->getInstList().insert(CI.getIterator(), __zap_localized__alloc);
							CI.getParent()->getInstList().insert(CI.getIterator(), __zap_localized__store);
							/* Load value from ptr; insert before the CallInst */
  						LoadInst *__zap_localized__load = new LoadInst(__zap_localized__alloc, (const char *)"__zap_loaded_", (bool)false, (Instruction *)&CI);
							/* replace the constant */
							CI.setArgOperand(ii, __zap_localized__load);
							new_vars++;
					}
				}
			}
			private:
				unsigned new_vars = 0;
		};
	};

	char ConstantIntToLocalVariableVisitor::ID = 0;
	static RegisterPass<ConstantIntToLocalVariableVisitor> XX("ci2lvv", "ConstantInt to local variable visitor impl");
}
