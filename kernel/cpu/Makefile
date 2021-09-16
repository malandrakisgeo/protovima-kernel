# Automatically generate lists of sources using wildcards
C_SOURCES = $(wildcard kernel/*.c kernel/*/*.c)
CPU_SOURCES = $(wildcard *.c)
H_SOURCES = $(wildcard ../../include/*.h)

INC_DIR = .
# The option -ffreestanding directs the compiler 
# to not assume that standard functions 
# have their usual definition
CFLAGS= -fno-pic -fno-pie -fno-exceptions -Wno-implicit-function-declaration  -ffreestanding -m32 -g   -I../../include
#CFLAGS= -fno-pic -fno-pie -fno-exceptions  -ffreestanding -m32 -std=c17 -g  -I ./kernel/include-I../../include/

# Convert the *.c filenames to *.o to give a list of object files to build
CPU_OBJ = ${CPU_SOURCES:.c=.o }
H_OBJ = ${H_SOURCES:.h=.o }

# Generic rule for compiling C code to an object file
# The compiler outputs annotated machine code,
# where meta information, such as textual labels, 
# that are redundant for execution, 
# remain present to enable more flexibilty 
# in how the code is eventually put together.
# One big advantage of this intermediary format 
# is that the code may be easily relocated into a larger binary file
# when linked in with routines from other routines in other libraries, 
# since code in the object file uses relative 
# rather than absolute internel memory references.
# $< is the first dependancy and $@ is the target file
%.o: %.c
	gcc ${CFLAGS} -c $< -o $@

# The option -f elf tells the assembler 
# to output an object file of the particular format Executable 
# and Linking Format (ELF), 
# which is the default format output by out C compiler
%.o: %.asm
	nasm $< -f elf -o $@

cpu.bin: ${CPU_OBJ} 
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary
	

# Default build target .
all: ${CPU_OBJ} 

debug: kernel.elf

# Disassemble our kernel - might be useful for debugging
kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@

# Clear away all generated files .
clean:
	#rm ${OBJ}
	#rm -fr *.bin *.dis *.o os.img *.map *.elf *.dis *.buf 
	find  . -name \*.o -type f -delete
	find  . -name \*.bin -type f -delete
	find  . -name \*.elf -type f -delete
	find  . -name \*.buf -type f -delete
	rm os-img

# Run bochs to simulate booting of our code
debug3: all kernel.elf
	qemu-system-i386 -s -vnc :0 -fda os.img -d guest_errors,int & gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"
