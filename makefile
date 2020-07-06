# This file demonstrates how to compile the YSF project on Linux.
#
# To compile YSF do:
#
# make YSF
#

GPP = g++ -D _GLIBCXX_USE_CXX11_ABI=0 -m32 -std=c++11 -Ilib -fno-stack-protector -nodefaultlibs
GCC = gcc -D _GLIBCXX_USE_CXX11_ABI=0 -m32 -Ilib -fno-stack-protector -nodefaultlibs

GPP_STATIC = g++ -D _GLIBCXX_USE_CXX11_ABI=0 -m32 -std=c++11 -Ilib -fno-stack-protector -static-libgcc -static-libstdc++
GCC_STATIC = gcc -D _GLIBCXX_USE_CXX11_ABI=0 -m32 -Ilib -fno-stack-protector -static-libgcc -static-libstdc++

LINK = $(GCC)
YSF_OUTFILE = "./YSF.so"

COMPILE_FLAGS = -c -O3 -fPIC -w -DLINUX
COMPILE_FLAGS_DEBUG = -c -Og -fPIC -w -DLINUX -ggdb3

YSF = -D YSF $(COMPILE_FLAGS)

.PHONY: all
all: YSF

.PHONY: clean
clean:
	-rm -f *~ *.o *.so

.PHONY: static
static: GPP = $(GPP_STATIC)
static: GCC = $(GCC_STATIC)
static: LINK = $(GPP)
static: YSF_OUTFILE = "./YSF_static.so"
static: all

.PHONY: static-debug
static-debug: GPP = $(GPP_STATIC)
static-debug: GCC = $(GCC_STATIC)
static-debug: LINK = $(GPP)
static-debug: COMPILE_FLAGS = $(COMPILE_FLAGS_DEBUG)
static-debug: YSF_OUTFILE = "./YSF_static_d.so"
static-debug: all

.PHONY: debug
debug: COMPILE_FLAGS = $(COMPILE_FLAGS_DEBUG)
debug: YSF_OUTFILE = "./YSF_debug.so"
debug: all

YSF: clean
	$(GPP) $(YSF) ./lib/sdk/*.cpp
	$(GPP) $(YSF) ./lib/raknet/*.cpp
	$(GCC) $(YSF) ./lib/subhook/subhook.c
	$(GPP) $(YSF) ./src/*.cpp
	$(GPP) $(YSF) ./src/natives/*.cpp
	$(GPP) $(YSF) ./src/includes/*.cpp
	$(LINK) -fshort-wchar -shared -o $(YSF_OUTFILE) *.o
