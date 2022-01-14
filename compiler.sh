nasm -f bin -o out/bootsect src/bootloader.asm    #assembler le programme de boot
nasm -f elf -o out/int.o src/int.asm
gcc -fno-pie -m32 -c src/screen.c   -o out/screen.o #compiler screen
gcc -fno-pie -m32 -c src/lib.c   -o out/lib.o 
gcc -fno-pie -m32 -c src/idt.c   -o out/idt.o 
gcc -fno-pie -m32 -c src/interrupt.c   -o out/interrupt.o 
gcc -fno-pie -m32 -c src/pic.c   -o out/pic.o 
gcc -fno-pie -m32 -c src/kernel.c  -o out/kernel.o  #compiler kernel

cd out
ld -m elf_i386 --oformat binary -Ttext 1000 kernel.o int.o screen.o lib.o idt.o  interrupt.o pic.o  -o kernel  #edition des liens
#ld -m elf_i386 --oformat binary -Ttext 1000  kernel.o screen.o -o kernel  #edition des liens
cat bootsect kernel /dev/zero | dd of=testdisk bs=512 count=2880 # création disquete