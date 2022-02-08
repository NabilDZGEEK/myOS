nasm -f bin -o out/bootsect src/boot/bootloader.asm    #assembler le programme de boot
nasm -f elf -o out/int.o src/"Entrés Sorties"/int.asm  
gcc -fno-pie -m32 -c src/"Entrés Sorties"/screen.c   -o out/screen.o 
gcc -fno-pie -m32 -c src/"Entrés Sorties"/idt.c   -o out/idt.o 
gcc -fno-pie -m32 -c src/"Entrés Sorties"/interrupt.c   -o out/interrupt.o 
gcc -fno-pie -m32 -c src/"Entrés Sorties"/pic.c   -o out/pic.o 
gcc -fno-pie -m32 -c src/lib.c   -o out/lib.o 
gcc -fno-stack-protector -fno-pie -m32 -c src/printk.c   -o out/printk.o 
gcc -fno-pie -m32 -c src/Mémoire/mm.c   -o out/mm.o
gcc -fno-pie -m32 -c src/Mémoire/kmalloc.c   -o out/kmalloc.o 
gcc -fno-pie -m32 -c src/Fichiers/disk.c   -o out/disk.o
gcc -fno-pie -m32 -c src/Fichiers/ext2.c   -o out/ext2.o
gcc -fno-pie -m32 -c src/kernel.c  -o out/kernel.o  #compiler kernel

cd out
ld  -m elf_i386 --oformat binary -Ttext 1000 kernel.o int.o screen.o  lib.o printk.o idt.o  interrupt.o pic.o mm.o kmalloc.o disk.o ext2.o -o kernel
cat bootsect kernel /dev/zero | dd of=../disquette bs=512 count=2880 # création disquete

: 'Remarques : 
-nasm bin :(binary) fichier binaire executable
-nasm elf :(executable & linkable format) fichier objet
-gcc fno-pie:(Position Independent Executables)
-gcc m32 : produire un executable sur 32 bits
-ld elf_i386 :produire un executable sur 32 bits
-Ttext : indique l''adresse linéaire à partir de laquelle le code commence
'