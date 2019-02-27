#!/bin/bash

CXX=/usr/local/bin/clang
OPT=/usr/local/bin/opt
LLC=/usr/local/bin/llc
DIS=/usr/local/bin/llvm-dis
RNGOPT=
#RNGOPT="-rng-seed=20"

echo "[hack] Building from C"
$CXX -emit-llvm -o test/foo.bc -c test/foo.c
echo "[hack] Converting ConstantInt arguments to local variables"
$OPT $RNGOPT -load built/ZapEm.dylib -ci2lvv  -o test/foo2.bc < test/foo.bc
echo "[hack] Injecting Zap randomizer functions"
$OPT $RNGOPT -load built/ZapEm.dylib -inject-randomizers  -o test/foo3.bc < test/foo2.bc
echo "[hack] Replacing call site i32 arguments with a Zap randomizer return value"
$OPT $RNGOPT -load built/ZapEm.dylib -replacer -o test/foo4.bc < test/foo3.bc
echo "[hack] Translating to assembler"
$LLC -o=test/foo4.s test/foo4.bc
echo "[hack] Producing binary executable"
$CXX -o test/foo4 test/foo4.s
echo "[hack] Disassembling bitcode files"
$DIS -o=test/foo.ll test/foo.bc
$DIS -o=test/foo2.ll test/foo2.bc
$DIS -o=test/foo3.ll test/foo3.bc
$DIS -o=test/foo4.ll test/foo4.bc
echo "[hack] Done"
