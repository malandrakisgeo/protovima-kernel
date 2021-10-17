C_SOURCES = $(wildcard kernel/*.c kernel/*/*.c)
CPU_SOURCES = $(wildcard *.c)
H_SOURCES = $(wildcard ../../include/*.h)

INC_DIR = .
CFLAGS= -fno-pic -fno-pie -fno-exceptions -Wno-implicit-function-declaration  -ffreestanding -m32 -g   -I../../include

CPU_OBJ = ${CPU_SOURCES:.c=.o }
H_OBJ = ${H_SOURCES:.h=.o }

%.o: %.c
	gcc ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

cpu.bin: ${CPU_OBJ} 
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

#TODO: Modify the disgrace below.
all: ${CPU_OBJ} 
	#rm interrupt_service_routines.o 
	#gcc -c interrupt_service_routines.c interrupt_routines.o -o interrupt_service_routines.o


debug: kernel.elf

kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@

clean:
	find  . -name \*.o -type f -delete
	find  . -name \*.bin -type f -delete
	find  . -name \*.elf -type f -delete
	find  . -name \*.buf -type f -delete
	rm os-img

# Run bochs to simulate booting of our code
debug3: all kernel.elf
	qemu-system-i386 -s -vnc :0 -fda os.img -d guest_errors,int & gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"
