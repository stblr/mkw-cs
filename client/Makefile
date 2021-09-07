# SPDX-License-Identifier: MIT

CC = ../tools/llvm-project/build/bin/clang
CXX = ../tools/llvm-project/build/bin/clang++
LD = CC
LINKER = ../tools/linker/target/release/linker

COMMONFLAGS = -nostdlib --target=powerpc-kuribo-eabi
CXXFLAGS = $(COMMONFLAGS) -fno-asynchronous-unwind-tables -fno-exceptions -fno-rtti -fshort-wchar -isystem ../tools/mkw/source -iquote ../common -O2 -std=c++14
LDFLAGS = $(COMMONFLAGS) -fuse-ld=lld -Wl,-r

all: client.bin

client.bin: client.o
	$(LINKER) ../tools/symbols.txt removals.txt replacements.txt $^

client.o: CsBootStrapScene.o CsSceneCreatorStatic.o
	$(LD) $(LDFLAGS) $^ -o $@

CsBootStrapScene.o: CsBootStrapScene.cpp
CsSceneCreatorStatic.o: CsSceneCreatorStatic.cpp
