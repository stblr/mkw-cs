# SPDX-License-Identifier: MIT

CC = ../tools/llvm-project/build/bin/clang
AS = $(CC)

COMMONFLAGS = --target=powerpc-kuribo-eabi -nostdlib -Wall -Wextra
CFLAGS = $(COMMONFLAGS) -fno-asynchronous-unwind-tables -O2
ASFLAGS = $(COMMONFLAGS) -c
LDFLAGS = $(COMMONFLAGS) -fuse-ld=lld -Wl,--image-base=0x80800000,--no-dynamic-linker

all: main.elf

main.elf: apploader.o cache.o delay.o di.o ios.o main.o memset.o start.o strlen.o
	$(CC) $(LDFLAGS) $^ -o $@

apploader.o: apploader.c
cache.o: cache.s
delay.o: delay.s
di.o: di.c
ios.o: ios.c
main.o: main.c
memset.o: memset.c
start.o: start.s
strlen.o: strlen.c

.PHONY: clean
clean:
	$(RM) main.elf *.o
