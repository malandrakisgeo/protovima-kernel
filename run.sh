
make clean
make all 
qemu-system-i386  -m 100M -fda os.img




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
