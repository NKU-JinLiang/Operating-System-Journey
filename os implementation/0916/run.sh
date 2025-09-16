nasm -f bin mbr.S -o mbr.bin
nasm -f bin loader.S -o loader.bin
dd if=/dev/zero of=os.img bs=512 count=100
dd if=mbr.bin of=os.img conv=notrunc
dd if=loader.bin of=os.img bs=512 count=2 seek=2 conv=notrunc
qemu-system-i386 -drive format=raw,file=os.img