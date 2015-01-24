# This file demonstrates how to compile the YSF project on Linux.
#
# To compile YSF do:
#
# make YSF
#

GPP = g++ -m32 -Ilib
GCC = gcc -m32
YSF_OUTFILE = "./YSF.so"

COMPILE_FLAGS = -c -O3 -fpack-struct=1 -fPIC -w -D LINUX

YSF = -D YSF $(COMPILE_FLAGS)

all: YSF

clean:
	-rm -f *~ *.o *.so

YSF: clean
	$(GCC) $(YSF) ./lib/sdk/*.cpp
	$(GPP) $(YSF) ./lib/raknet/*.cpp
	$(GCC) $(YSF) ./lib/subhook/subhook.c
	$(GPP) $(YSF) ./src/*.cpp
	$(GPP) -fshort-wchar -shared -o $(YSF_OUTFILE) *.o
