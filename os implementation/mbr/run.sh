#!/bin/bash
set -e

nasm -f bin -I ./ mbr.asm -o mbr.bin
nasm -f bin -I ./ loader.S -o loader.bin

dd if=/dev/zero of=boot.img bs=512 count=2048
dd if=mbr.bin of=boot.img bs=512 count=1 conv=notrunc
dd if=loader.bin of=boot.img bs=512 seek=2 conv=notrunc

qemu-system-x86_64 -drive format=raw,file=boot.img