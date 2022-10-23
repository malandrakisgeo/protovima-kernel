#old way:
#rm *.bin  *.o  *.elf  disk.img os-image

#nasm -g -f elf32 -F dwarf -o boot.o d.asm
#ld -melf_i386 -Ttext=0x7c00 -nostdlib --nmagic -o boot.elf boot.o
#objcopy -O binary boot.elf boot.bin

#gcc -g -m32 -c -ffreestanding -o kernel.o kernel.c -lgcc
#ld -melf_i386 -T linker.ld -nostdlib --nmagic -o kernel.elf kernel.o
#objcopy -O binary kernel.elf kernel.bin



#dd if=/dev/zero of=disk.img bs=512 count=2880  ##prosoxh sto dh 
#dd if=boot.bin of=disk.img bs=512 conv=notrunc
#dd if=kernel.bin of=disk.img bs=512 seek=1 conv=notrunc

#qemu-system-i386  -smp 4  -m 50M,maxmem=50M disk.img

#qemu-system-i386 -m 1G,maxmem=20030M -fda disk.img

--------------------------

#new way: 
make clean
make all 
qemu-system-i386 -m 436M -fda os.img 



## DEBUG METHOD 1: 
#gdb kernel.elf  \
#       -ex 'target remote localhost:1234' \
##        -ex 'layout src' \
#        -ex 'layout reg' \
#        -ex 'break main' \
#        -ex 'continue'


##DEBUG METHOD 2:
#telnet localhost 4444
# { "execute": "qmp_capabilities" }
# { "execute": "query-commands" }
# {"execute":"dump-guest-memory","arguments":{"paging":false,"protocol":"file:/home/guest-memory","format":"elf"}}
# ENAL. {"execute":"dump-guest-memory","arguments":{"paging":false,"protocol":"file:/home/guest-membin","format":"binary"}}
#hexedit guest-memory > readable-hex.txt
#enal.  xxd file > file.hex
#diff  hexfile1.txt hexfile2.txt > differenceF.txt
#diff  kernel-without.elf kernel.elf > differenceF.txt
##USEFUL LINKS: 

#https://forum.osdev.org/viewtopic.php?f=1&t=33512

#https://stackoverflow.com/questions/19548852/ld-error-in-eh-frame-no-eh-frame-hdr-table-will-be-create

#https://stackoverflow.com/questions/33603842/how-to-make-the-kernel-for-my-bootloader
