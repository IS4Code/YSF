# This file demonstrates how to compile the YSF project on Linux.
#
# To compile YSF do:
#
# make YSF
#

GPP = g++
GCC = gcc
YSF_OUTFILE = "./YSF.so"

COMPILE_FLAGS = -c -O3 -fPIC -w -D LINUX -I ./SDK/amx/

YSF = -D YSF $(COMPILE_FLAGS)

all: YSF

clean:
	-rm -f *~ *.o *.so

YSF: clean
	$(GCC) $(YSF) ./SDK/amx/*.c
	$(GPP) $(YSF) ./SDK/*.cpp
	$(GPP) $(YSF) *.cpp
	$(GPP) -O2 -fshort-wchar -shared -o $(YSF_OUTFILE) *.o
