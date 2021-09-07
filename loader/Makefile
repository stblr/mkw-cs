# SPDX-License-Identifier: MIT

CC = ../tools/llvm-project/build/bin/clang
AS = $(CC)
LD = $(CC)

COMMONFLAGS = --target=powerpc-kuribo-eabi -nostdlib -Wall -Wextra
CFLAGS = $(COMMONFLAGS) -fno-asynchronous-unwind-tables -O2
ASFLAGS = $(COMMONFLAGS) -c
LDFLAGS = $(COMMONFLAGS) -fuse-ld=lld -Wl,--image-base=0x80810000,--no-dynamic-linker

all: boot.elf

boot.elf: apploader.o cache.o delay.o di.o ios.o main.o memcpy.o memset.o patches.o start.o strlen.o
	$(LD) $(LDFLAGS) $^ -o $@

apploader.o: apploader.c
cache.o: cache.S
delay.o: delay.S
di.o: di.c
ios.o: ios.c
main.o: main.c
memcpy.o: memcpy.c
memset.o: memset.c
patches.o: patches.S
start.o: start.S
strlen.o: strlen.c

.PHONY: clean
clean:
	$(RM) boot.elf *.o
