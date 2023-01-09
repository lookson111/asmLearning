#!/bin/bash
echo "Compile asm!"
if [ -n "$1" ]
then
	if [ ! -f $1.asm ]; then
		echo "File $1.asm not found!"
		exit 1
	fi
	nasm -f elf -dOS_LINUX $1.asm
	ld -m elf_i386 $1.o -o $1
	rm $1.o
else
	echo "Need write file asm name!"
fi
