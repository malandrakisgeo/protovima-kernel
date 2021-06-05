 rm *.bin  *.o os-image
#assemble boot.s file
nasm a.asm -f bin  -o boot.o
#nasm mem.asm -f bin  -o boott.o #to binary einai polu pio euanagnwsto meta apo objdump se sxesh me to elf (150 vs 550 grammes)

#nasm memsimplified.asm   -o e8202.o

nasm  jmptokernel.asm -f elf -o kerneljump.o

#nasm  E820mem.asm -f elf -o e820.o
#gcc TEST.c -S to view .S
#objdump -D -Mintel,i386 -b binary -m i386 kernel.o > kernel.txt
#objdump -D -Mintel,i386 -b binary -m i386 os-image > os2.txt

gcc -m32 -c  -fno-builtin -nostdinc -nostdlib -ffreestanding  kernel.c  -o kernel.o
#gcc -Wall -Wcast-align -m32 -c -ffreestanding  kernel.c  -o kernel.o

#gcc  fno-plt -m32  -ffreestanding   kernel.c e8202.o -o kernel.o #to -c den endeiknuetai gia linking me .o

ld -melf_i386 -Ttext 0x1000  -o kernel.bin  kerneljump.o kernel.o --oformat binary

#ld -melf_i386 -N -Ttext 0x1000  -o kernel.bin  kerneljump.o kernel.o --oformat binary
# ld -melf_i386 -N -Ttext 0x1000  -o freestanding.bin  freestanding.o --oformat binary

cat boot.o kernel.bin > os-image

#qemu-system-i386  -fda os-image
#qemu-system-i386 -m 1G  os-image  -serial file:serial.log
qemu-system-i386 -D log.log -d cpu_reset -qmp tcp:localhost:4444,server,nowait -m 500M  os-image 

#telnet localhost 4444
# { "execute": "qmp_capabilities" }
# { "execute": "query-commands" }
# {"execute":"dump-guest-memory","arguments":{"paging":false,"protocol":"file:/home/guest-memory","format":"elf"}}
# {"execute":"dump-guest-memory","arguments":{"paging":false,"protocol":"file:/home/guest-membin","format":"binary"}}
#hexedit guest-memory > readable.txt
#xxd file > file.hex
#diff  best820-yes.txt best820-no.txt > differenceF.txt