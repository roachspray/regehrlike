
# Regehr Like code (Gamma Ray Bit Flipper)

John Regehr of Utah did some work on random bit flipping of data via llvm.
I do not recall how he was 
exactly implementing his strategy (i read the paper in winter 15-16 and 
forget), but I think this is similar. Basically, there are a number of
ways to do things and this is just one (probably awful) approach.
I forget if there is instruction bit flipping that he did, but maybe I will
try that too.. but needs some thinking through.

...this is a WIP......no longer a wip, just trash code :P

Currently, what this does is replaces all 8, 16, 32, and 64 bit integers arguments to functions
with possibly randomly changed values. The purpose is to re-run test suites 
with the modified application in order to simulate the low-probability bit or
more flips that could occur in certain extreme situations. The aim is to perform
some basic analysis as to the stability of the code given random changes.

I have just added the single bit flip method and default to that now, but 
hopefully will do a configuration file thing so you can specify which is used
and when on what blah blah.

The RNG stuff may be overkill and we can just use a the one found in the ISO 
C standard.

Another thing is the randomizer insertion... all that could be C code that 
is linked in... So.... I dunno how I feel about that being LLVM generated.
It was fun though :-P

Perhaps a passmanager will be desirable in the future, but I have not 
pursued that for a couple of reasons. One is that I wanted to chunk things.

# Requirements
- LLVM+Clang 3.8.1
... this was built on OS X... I'm sorry. 10.11.6. It uses a side-installed
llvm and clang, not the xcode.

# Building
Make.

# Running

Look at hack_runtest.sh
