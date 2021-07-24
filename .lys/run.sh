rm *.bin *.img *.o
#assemble boot.s file
nasm -f bin stage1.asm -o boot.o

#nasm -l output.txt stage00.asm

#compile kernel.c file
#gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -m32  -ffreestanding  -static -nostartfiles -c kernel.c -o kernel.o  #2
# gcc  -ffreestanding  -c kernel.c -o kernel.o 

#linking the kernel with kernel.o and boot.o files
#ld -m elf_i386 -T linker.ld kernel.o boot.o -o MyOS.bin -nostdlib
#ld  -o MyOS.bin -Ttext  0x01000 kernel.o --oformat binary   # 2
ld -melf_i386 -N -Ttext 0x01000 -e0x01000 -o MyOS.o kernel.o

objcopy -R .note -R .comment -S -O binary MyOS.o MyOS.bin #peritto an 2

cat boot.o MyOS.bin > os-image

#run it in qemu
#qemu-system-x86_64  MyOS.bin 
qemu-system-i386  -drive format=raw,file=os-image
