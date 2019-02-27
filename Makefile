#
# It could be that I switch to CMake at some point, but I do not wish to 
# go down that road just now. The Makefile is heavily inspired|ripped off
# of some code Jared Carlson gave me; thanks, buddy ;-).
#

LLVM_CONFIG?=/usr/local/bin/llvm-config
ifndef VERBOSE
QUIET:=@
endif

BUILDJSONCPP:=#
ifdef WITHJSONCPP
BUILDJSONCPP:=
endif

SRC_DIR?=$(PWD)/src
LDFLAGS+=$(shell $(LLVM_CONFIG) --ldflags) -Lthirdparty/jsoncpp-1.8.0/build/src/lib_json -ljsoncpp
COMMON_FLAGS=-Wall -Wextra -g

# Constants left from taking this from @jcarlson
PART_LLVM_FLAGS=-I/usr/local/include  -fPIC -fvisibility-inlines-hidden -Wall -W -Wno-unused-parameter \
     -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wcovered-switch-default \
     -Wnon-virtual-dtor -std=c++11  -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS

CXXFLAGS+=$(COMMON_FLAGS) $(shell $(LLVM_CONFIG) --cxxflags)
CPPFLAGS+=$(shell $(LLVM_CONFIG) --cppflags) -I$(SRC_DIR) -Ithirdparty/jsoncpp-1.8.0/include
CXXFLAGS+=-g
ifeq ($(shell uname),Darwin)
LOADABLE_MODULE_OPTIONS=-bundle -undefined dynamic_lookup
else
LOADABLE_MODULE_OPTIONS=-shared -Wl,-O1
endif
CXX=/usr/local/bin/clang++

PASS=ZapEm.dylib
PASS_OBJECTS=ConstantIntToLocalVariableVisitor.o  \
 ZapReplacer.o  \
 IntegerReplaceRandomizer.o  \
 IntegerBaseBitFlipRandomizer.o \
 InjectRandomizers.o  \
 ZapConfig.o \
 CodeModificationLocation.o  

default: prep $(PASS)

jsoncpp:
	@echo Building jsoncpp-1.8.0
	cd thirdparty && tar zxvf jsoncpp-1.8.0.tar.gz && cd jsoncpp-1.8.0 && mkdir build && cd build && cmake .. && make && cd ../../	

prep:
	$(QUIET)mkdir -p built
	

%.o : $(SRC_DIR)/%.cpp
	@echo "CPPFLAGS ${CPPFLAGS}"
	@echo "CXXFLAGS ${CXXFLAGS}"
	@echo Compiling $*.cpp
	$(QUIET)$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

$(PASS) : $(PASS_OBJECTS)
	@echo Linking $@
	$(QUIET)$(CXX) -o built/$@ $(LOADABLE_MODULE_OPTIONS) $(CXXFLAGS) $(LDFLAGS) $^
	$(QUIET)rm -f *.o

clean:
	$(QUIET)rm -rf *.o built
