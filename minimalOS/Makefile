# Author: UWE
# Desc: Makefile to build Minimal Operating System
# Goal: Provide students with the bare minimum to boot their first OS

OSNAME=minimal.bin
CDROM=minimal.iso

AS=i686-elf-as
CC=i686-elf-gcc
CFLAGS=-ffreestanding -O2 -std=gnu99 -Wall -Wextra 
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc

OBJS= boot.o kernel.o

%.o : %.s 
	$(AS) -o $@ $<

%.o : %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

$(CDROM) : $(OSNAME) grub.cfg
	mkdir -p isodir/boot/grub
	cp $(OSNAME) isodir/boot/$(OSNAME)
	cp grub.cfg isodir/boot/grub
	grub-mkrescue -o $@ isodir

$(OSNAME) : $(OBJS)
	$(CC) -T linker.ld -o $@ $(LFLAGS) $^


.PHONY: clean

run :
	qemu-system-i386 --curses --serial mon:stdio --kernel minimal.bin

clean :
	rm -rf isodir/boot/$(OSNAME)
	rm -rf isodir/boot/grub/grub.cfg
	rm -rf *.o
	rm -rf $(OSNAME)
	rm -rf $(CDROM)
