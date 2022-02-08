#include "Entrés Sorties/screen.h"
#include "Entrés Sorties/io.h"
#include "Entrés Sorties/idt.h"
#include "Mémoire/mm.h"
#include "Mémoire/kmalloc.h"
#include "Fichiers/disk.h"
#include "Fichiers/ext2.h"
#include "lib.h"

void main(void);
void print_ok(void);
void init_pic(void);

void _start(void){
        scrollup(2);
        kX = 35;
        kY = 0;
        kattr = WHITE;
        print("Bienvenu dans PERSO\n");
        kY = 10;
        kX=0;kY=5;
        print("kernel : chargement IDT");
	init_idt();
        print_ok();

        print("kernel : configuration des PIC");
	init_pic();/*initialisation des controlleurs*/
        print_ok();

        print("kernel : activation des interruptions");
        sti;
        print_ok();
	/* Initialisation du pointeur de pile %esp */
	/*asm("	movw $0x18, %ax \n \
		movw %ax, %ss \n \
		movl $0x2000, %esp");*/

        print("kernel : Initialisation de la memoire");
        //init_mm();
        print_ok();
        main();
} 
void main(){
        printk("shell>");
        while (1);
}
void print_ok(){
        kattr = GREEN;
	kX=60;
        print("OK\n");
        kattr = WHITE;
}
