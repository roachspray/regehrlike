#ifndef __BASERANDOMIZER_HPP
#define	__BASERANDOMIZER_HPP

class BaseRandomizer {
	public:
		static void inject(llvm::Module& M);
};


#endif	// !__BASERANDOMIZER_HPP
