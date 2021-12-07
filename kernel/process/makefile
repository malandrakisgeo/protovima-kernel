C_SOURCES = $(wildcard kernel/*.c kernel/*/*.c)
PROCESS_SOURCES = $(wildcard *.c)

CFLAGS= -fno-pic -fno-pie -fno-exceptions -Wno-implicit-function-declaration  -ffreestanding -m32 -g   -I../../include

PROC_OBJ = ${PROCESS_SOURCES:.c=.o }

%.o: %.c
	gcc ${CFLAGS} -c $< -o $@
	
all: ${PROC_OBJ} 

clean:
	find  . -name \*.o -type f -delete
	find  . -name \*.bin -type f -delete
