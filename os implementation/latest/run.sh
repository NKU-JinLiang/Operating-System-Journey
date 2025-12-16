nasm -f bin loader/mbr.S -o mbr.bin
nasm -f bin loader/loader.S -o loader.bin
gcc -m32 -c kernel/main.c -o kernel/main.o

dd if=/dev/zero of=os.img bs=512 count=200
dd if=mbr.bin of=os.img conv=notrunc
dd if=loader.bin of=os.img bs=512 seek=2 conv=notrunc

qemu-system-i386 -drive format=raw,file=os.img  -m 4096