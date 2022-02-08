#define __SCREEN__
#include "io.h"
#include "screen.h"

/* 
 * 'scrollup' scrolle l'écran (la console mappée en ram) vers le haut
 * de n lignes (de 0 a 25).
 */
void scrollup(unsigned int n)
{
        unsigned char *video, *tmp;

        for (video = (unsigned char *) RAMSCREEN;
             video < (unsigned char *) SCREENLIM; video += 2) {
                tmp = (unsigned char *) (video + n * 160);

                if (tmp < (unsigned char *) SCREENLIM) {
                        *video = *tmp;
                        *(video + 1) = *(tmp + 1);
                } else {
                        *video = 0;
                        *(video + 1) = 0x07;
                }
        }

        kY -= n;
        if (kY < 0)
                kY = 0;
}

void putcar(uchar c)
{
        unsigned char *carAdd;
        int i;
        switch (c){
            case 9 : kX = kX + 8 - (kX % 8);break; /* TAB */
            case 10: kX = 0;kY++;break; /* CR-NL */
            case 13: kX = 0;break; /* CR */

        	default:carAdd = (unsigned char *) (RAMSCREEN + 2 * kX + 160 * kY);
                	*carAdd = c;
                	*(carAdd + 1) = kattr;
               		kX++;
					if (kX > 79) {
							kX = 0;
							kY++;
					}
					break;
        }
        if (kY > 24) scrollup(kY - 24);
}

/*
 * 'print' affiche à l'écran, à la position courante du curseur, une chaîne
 * de caractères terminée par \0.
 */
void print(char *string)
{
        while (*string != 0) {  /* tant que le caractère est différent de 0x0 */
                putcar(*string);
                string++;
        }
}
void supcar(){
	unsigned char *carAdd;
	kX--;
	carAdd = (unsigned char *) (RAMSCREEN + 2 * kX + 160 * kY);
	*carAdd=' ';
	*(carAdd + 1) = kattr;
}
void move_cursor(u8 x, u8 y)
{
	u16 c_pos;

	c_pos = y * 80 + x;

	outb(0x3d4, 0x0f);
	outb(0x3d5, (u8) c_pos);
	outb(0x3d4, 0x0e);
	outb(0x3d5, (u8) (c_pos >> 8));
}

void show_cursor(void)
{
	move_cursor(kX, kY);
}

void hide_cursor(void)
{
	move_cursor(-1, -1);
}
