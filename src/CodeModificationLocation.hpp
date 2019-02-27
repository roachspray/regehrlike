#ifndef	__CODEMODIFICATIONLOCATION_HPP
#define	__CODEMODIFICATIONLOCATION_HPP

using namespace llvm;

// Possibly subclass per-type being Zapped.
class CodeModificationLocation {
	private:
		CallInst	*callLocation;
		Type			*argumentType;
		Value			*argumentValue;
		unsigned	 argumentIdx;

	public:
		CodeModificationLocation(CallInst *c, Type *t, Value *v, unsigned i) : callLocation(c), argumentType(t), argumentValue(v), argumentIdx(i) { };
		CallInst	*getCallInst();
		Type			*getArgumentType();
		Value			*getArgumentValue();
		unsigned	 getArgumentIdx();
};

#endif // !__CODEMODIFICATIONLOCATION_HPP
