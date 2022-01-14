#include "screen.h"
#include "io.h"
#include "idt.h"

void main(void);
void init_pic(void);
void _start(void){
        scrollup(2);
        kX = 35;
        kY = 0;
        kattr = 0x0F;
        print("Bienvenu dans nabilux\n");
        kY = 10;
        kX=0;kY=5;
        print("kernel : chargement IDT");
	init_idt();
        kattr = 0x0A;
	kX=60;
        print("OK\n");
        kattr = 0x0F;

        print("kernel : configuration des PIC");
	init_pic();/*initialisation des controlleurs*/
	kattr = 0x0A;
	kX=60;
        print("OK\n");
        kattr = 0x0F;

        print("kernel : activation des interruptions");
        sti;
        kattr = 0x0A;
	kX=60;
        print("OK\n");
        kattr = 0x0F;
	/* Initialisation du pointeur de pile %esp */
	/*asm("	movw $0x18, %ax \n \
		movw %ax, %ss \n \
		movl $0x20000, %esp");*/
        main();
} 
void main(){
        show_cursor();

        /*kattr = 0x0A;
        print("nabilux>");*/
        
        while (1);
}
