#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Pass.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/SymbolTableListTraits.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/Support/raw_ostream.h"

#include "CodeModificationLocation.hpp"
#include "ZapTypeValueSupport.hpp"
#include "ZapConfig.hpp"

using namespace llvm;

/*
 * Formerly ZapInteger32.. genericizing...
 * it is a good question if that is reasonable or not... 
 * but I think we iterate once over calls and then
 * farm out to modifiers
 *
 * I think.? hah. whatever. This could be the wrong way. :P
 */

namespace {
	struct ZapReplacer: public ModulePass {
		static char ID;
		std::vector<CodeModificationLocation>	modificationList;
		std::vector<std::string> skip_replace_functions = { "__zap_randomizer_int32__", "__zap_bitflip_randomizer_int32__"};	// Might be join'd with some config from user.

		// XXX Currently unsure if will use at CodeModificationLocation insertion time or
		// at runOnModule() CallInst insertion... I think the later first.
		// in the visitor, we just do a simple "is anything here supported" and add it.
		// hmm.. meh. oh well.
		ZapConfig *_zConfig;

		ZapReplacer() : ModulePass(ID) { _zConfig = new ZapConfig("replace.cfg"); }
		
		virtual bool
		runOnModule(Module &M)
		{
			// Uhm be also we may move to a config.. or join'd with a config from user.
			std::vector<std::string> skip_callsiteanalysis_functions = { "arc4random", "__zap_randomizer_int32__" , "__zap_bitflip_randomizer_int32__"};

			for (auto &f : M) {
				// Skip performing call site checking if function in skip_functions
				if (std::find(skip_callsiteanalysis_functions.begin(), skip_callsiteanalysis_functions.end(), f.getName()) != skip_callsiteanalysis_functions.end()) {
					errs() << "Skipping function: " << f.getName() << "\n";
					continue;
				}
				errs() << "Checking function " << f.getName() << "\n";
				CheckCallInsts cCheck;
				cCheck.setZapReplacer(this);
				cCheck.visit(f);
			}
		
			// I guess I do ant to be sure that values are sane when I use them. Like .. does the visitor pattern
			// have some implications about pointer values? I do not know :-((
			std::vector<CodeModificationLocation>::iterator iml;
			for (iml = modificationList.begin(); iml != modificationList.end(); ++iml) {

// XXX This assume integer type
				CodeModificationLocation cLoc = *iml;
				CallInst *ci = cLoc.getCallInst();
				IntegerType *t = cast<IntegerType>(cLoc.getArgumentType());
				unsigned nBits = t->getBitWidth();
//				Function *insertedZap = ci->getModule()->getFunction("__zap_randomizer_i"+std::to_string(nBits)+"__");
				Function *insertedZap = ci->getModule()->getFunction("__zap_bitflip_randomizer_i"+std::to_string(nBits)+"__");
				if (insertedZap == NULL) {
					// XXX Should error harder than this :-P.
					errs() << "No zap found! WTF.\n";
					continue;
				}
				CallInst *callNewFunc = CallInst::Create(insertedZap, { cLoc.getArgumentValue() }, "__zap_replicant_", ci);
				ci->setArgOperand(cLoc.getArgumentIdx(), callNewFunc);
				errs() << "Inserted zap in function " << ci->getFunction()->getName() << " for: \n";
				cLoc.getCallInst()->dump();
			}
			return false;
		}

		struct CheckCallInsts : public InstVisitor<CheckCallInsts> {
			ZapReplacer *__ziInst;

			void
			setZapReplacer(ZapReplacer *p) { __ziInst = p; }

			void
			visitCallInst(CallInst &CI)
			{
				errs() << "Found CallInst inside function " << CI.getFunction()->getName() << "\n";

				Function *called = CI.getCalledFunction();
				// Skip performing argument replacement if function in skip_replace_functions
				if (std::find(__ziInst->skip_replace_functions.begin(), __ziInst->skip_replace_functions.end(), called->getName()) != __ziInst->skip_replace_functions.end()) {
					errs() << "Skipping replace function: " << called->getName() << "\n";
					return;
				}

				unsigned numArgOps = CI.getNumArgOperands();
				for (unsigned ii = 0; ii < numArgOps; ii++) {
					Value *va = CI.getArgOperand(ii);
					Type *ta = va->getType();

					if (ZapTypeValueSupport::isSupported(ta, va)) {
							CodeModificationLocation lll = CodeModificationLocation(&CI, ta, va, ii);
							__ziInst->modificationList.push_back(lll);
							errs() << "Inserted a CodeModificationLocation\n";
					}
				}
			}
			private:
		};
	};

	char ZapReplacer::ID = 0;
	static RegisterPass<ZapReplacer> XX("replacer", "Zap Replace Arguments");
}
