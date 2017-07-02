# This file demonstrates how to compile the YSF project on Linux.
#
# To compile YSF do:
#
# make YSF
#

GPP = g++ -m32 -std=c++11 -Ilib -fno-stack-protector
GCC = gcc -m32 -Ilib -fno-stack-protector
YSF_OUTFILE = "./YSF.so"

COMPILE_FLAGS = -c -O3 -fpack-struct=1 -fPIC -w -DLINUX

YSF = -D YSF $(COMPILE_FLAGS)

all: YSF

clean:
	-rm -f *~ *.o *.so

YSF: clean
	$(GPP) $(YSF) ./lib/sdk/*.cpp
	$(GPP) $(YSF) ./lib/raknet/*.cpp
	$(GCC) $(YSF) ./lib/subhook/subhook.c
	$(GPP) $(YSF) ./src/*.cpp
	$(GPP) $(YSF) ./src/natives/*.cpp	
	$(GCC) -nodefaultlibs -fshort-wchar -shared -o $(YSF_OUTFILE) *.o
