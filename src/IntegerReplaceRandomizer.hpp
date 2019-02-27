#ifndef __INTEGERREPLACERANDOMIZER_HPP
#define	__INTEGERREPLACERANDOMIZER_HPP

#include "BaseRandomizer.hpp"

#define injectIntN(nBits) \
	static void  \
	injectInt##nBits(llvm::Module& M, Function *fnRand) \
	{ \
		LLVMContext& ctx = M.getContext();  \
		std::string int##nBits##_rand = "__zap_randomizer_i" #nBits "__"; \
		Constant *cTmp = M.getOrInsertFunction(int##nBits##_rand, \
			Type::getInt##nBits##Ty(ctx), \
			Type::getInt##nBits##Ty(ctx), \
			NULL);  \
		Function *zap_i##nBits = cast<Function>(cTmp);  \
		zap_i##nBits->setCallingConv(CallingConv::C); \
	\
		Argument& inArg0 = zap_i##nBits->getArgumentList().front(); \
		inArg0.setName("inArg0"); \
		BasicBlock *blkEntry = BasicBlock::Create(ctx, "entry", zap_i##nBits);  \
		BasicBlock *blkZapIt = BasicBlock::Create(ctx, "zap_it", zap_i##nBits); \
		BasicBlock *blkReturn = BasicBlock::Create(ctx, "return", zap_i##nBits);  \
		IRBuilder<> builder(blkEntry);  \
	\
		Value *callArc4Random = builder.CreateCall(fnRand, None, "__zap_rand_", nullptr);  \
		Value *flipMean = ConstantInt::get(IntegerType::get(ctx, 32), 2147483648, false); \
		Value *lessThan = builder.CreateICmpULE(callArc4Random, flipMean, "__zap_lessthan_"); \
		Value *branchZap = builder.CreateCondBr(lessThan, blkZapIt, blkReturn); \
		builder.SetInsertPoint(blkZapIt); \
		Value *castRand = builder.CreateZExtOrTrunc(callArc4Random, Type::getInt##nBits##Ty(ctx), "__zap_cast_");  \
		Value *cr = builder.CreateRet(castRand);  \
	\
		builder.SetInsertPoint(blkReturn);  \
		Value *cr2 = builder.CreateRet(&inArg0);  \
	} \

class IntegerReplaceRandomizer : BaseRandomizer {

	public:
		static void inject(llvm::Module& M);
};


#if 0
		errs() << "Inserted function:\n"; \
		blkEntry->dump(); \
		blkZapIt->dump(); \
		blkReturn->dump();  \
		errs() << "End of inserted function.\n";  \
    } \

#endif

#endif	// !__INTEGERREPLACERANDOMIZER_HPP
