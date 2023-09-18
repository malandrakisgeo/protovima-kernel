# Protovima-kernel

My first step in kernel programming. More of a playground than an actual project.

Goal: a kernel managing the CPU, interrupts, memory, processes, etc, along with a shell with some basic commands. 

Meant to run on any single-core CPU with an i386 architecture. 

==========================

**DISCLAIMER**: <br />
C is not my cup of tea, so be prepared for some weird code here.


==========================

###  Building and running: 

All you need is gcc, nasm, and ld for building, and qemu for running.

If unable to use ./run.sh, do this:
make clean
make all 
qemu-system-i386 -m 500M -fda os.img 


==========================

###  Protovima in action: 

![](./inAction1.jpg)

![](./inAction2.jpg)

![](./inAction3.jpg)

==========================

Bootloader&Makefile largely based on::

https://dev.to/frosnerd/writing-my-own-boot-loader-3mld

and 

https://github.com/vitaminac/minios

==========================

###  OS programming resources: 

osdev.org
<br />lowlevel.eu
<br />jamesmolloy.co.uk
<br />reddit.com/r/osdev/
<br />https://os.phil-opp.com
<br />
<br />Operating System Concepts - Silberschatz-Gagne-Galvin
<br />Modern Operating Systems - Tanenbaum (The MINIX book)

==========================
