
all: clean link  bootFinal.bin

stage0.o: stage0.asm
	nasm -felf32 $< -o $@

	#nasm -felf32  $< -o $@

kernel.o: kernel.c
	#gcc -o $@ $<
	# gcc   -I/usr/include  -m64  -c  -o  $@ $<
	gcc  -m32  -I/usr/include   -c  -o  $@ $<
	#objcopy -O binary -j .text $@ kernel.o

link: stage0.o kernel.o
	#gcc -m32 stage0.bin kernel.o -o test

	#ld -o kernel.bin -Ttext 0x08000 -e0x08000 kernel.o --oformat binary
	ld -I/usr/lib/libc.so  -m elf_i386 -Tlinker.ld  

bootFinal.bin: os-img.bin
	objcopy --binary-architecture=i386  --remove-section=.comment $< $@	
	
	

clean:
	rm -f stage0.o kernel.o bernel.bin kernel.bin os-image.img
