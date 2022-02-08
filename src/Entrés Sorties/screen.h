#include "../types.h"

#ifdef __SCREEN__

#define RAMSCREEN 0xB8000	/* debut de la memoire video */
#define SIZESCREEN 0xFA0	/* 4000, nombres d'octets d'une page texte */
#define SCREENLIM 0xB8FA0

char kX = 0;			/* position courante du curseur a l'ecran */
char kY = 0;
char kattr = 0x0F;		/* attributs video des caracteres a afficher */


#else
#define GREEN 0x0A
#define WHITE 0X0F
extern char kX;
extern char kY;
extern char kattr;

#endif				/* __SCREEN__ */

void scrollup(unsigned int);
void putcar(uchar);
void move_cursor(u8, u8);
void show_cursor(void);
void hide_cursor(void);
void print(char*);
void supcar(void);