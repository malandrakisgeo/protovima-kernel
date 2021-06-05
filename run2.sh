 rm *.bin  *.o  
nasm b.asm -f elf  -o boot.o
##nasm b.asm -f bin  -o bootb.o
#nasm b.asm -f elf  -o boot.o

#objcopy -I binary  -O elf32-i386  boot.o

gcc -m32 -c  -fno-builtin -nostdinc -nostdlib -ffreestanding  kernel.c  -o kernel.o -lgcc
#gcc  -m32 -o kernelwithjump.bin kernel.o kerneljump.o

ld  -m elf_i386 -T linker.ld  boot.o  kernel.o  -o kernel.bin

#qemu-system-i386   kernel.bin



#ld -melf_i386 -Tlinked.ld -nostdlib --nmagic -o kernel.elf kernelwithjump.bin

#objcopy -O binary kernel.elf kernel.bin boot.o




#https://forum.osdev.org/viewtopic.php?f=1&t=33512

#https://stackoverflow.com/questions/19548852/ld-error-in-eh-frame-no-eh-frame-hdr-table-will-be-create

#https://stackoverflow.com/questions/33603842/how-to-make-the-kernel-for-my-bootloader