C_SOURCES = $(wildcard *.c)

CFLAGS= -fno-pic -fno-pie -fno-exceptions -Wno-implicit-function-declaration  -ffreestanding -m32 -g  -I../include

OBJ = ${C_SOURCES:.c=.o }
 
%.o: %.c
	gcc ${CFLAGS} -c $< -o $@
	
all: ${OBJ}

clean: 
	find  . -name \*.o -type f -delete
