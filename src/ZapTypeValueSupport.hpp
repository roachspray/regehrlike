#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"

using namespace llvm;

/*
 * Really this should just be Type support since any
 * value specifics, I think, we can handle via ``normalizing''
 * passes that are run ahead of the replacement pass.
 */
class ZapTypeValueSupport {

	public:
		ZapTypeValueSupport() {}
		static bool
		isSupported(Type *t, Value *v) {
			/*
			 * Currently we only support
			 * 	. non-constant 32-bit integer
			 */
			if (IntegerType *intType = dyn_cast<IntegerType>(t)) {	
				if (isa<ConstantInt>(v) == true) {
					// We should assert() likely. But maybe config'd option. the ConstantInt->Variable should have been run.

				} else {
					if (intType->getBitWidth() == 64) {
						return true;
					} else if (intType->getBitWidth() == 32) {
						return true;
					} else if (intType->getBitWidth() == 16) {
						return true;
					} else if (intType->getBitWidth() == 8) {
						return true;
					}
				}
			}
			return false;
		}
};
