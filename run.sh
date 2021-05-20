 rm *.bin  *.o os-image
#assemble boot.s file
nasm a.asm -f bin  -o boot.o

nasm  jmptokernel.asm -f elf -o kerneljump.o

gcc -m32  -ffreestanding  -c kernel.c -o kernel.o 

ld -melf_i386 -N -Ttext 0x1000  -o kernel.bin  kerneljump.o kernel.o --oformat binary

cat boot.o kernel.bin > os-image

#qemu-system-i386  -fda os-image
qemu-system-i386  os-image

