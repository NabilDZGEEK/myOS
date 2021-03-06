#include "io.h"

void init_pic(void)
{
	/* Initialisation de ICW1 */
	/*0x11 signifie:
	 *-avec ICW4
	 *-présence d'un controlleur esclave
	 */
	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	/* Initialisation de ICW2 */
	outb(0x21, 0x20);	/* vecteur de depart = 32 */
	outb(0xA1, 0x70);	/* vecteur de depart = 96 */

	/* Initialisation de ICW3 */
	/*le controlleur esclave est rataché a la broche 2 du controlleur maitre*/
	outb(0x21, 0x04);
	outb(0xA1, 0x02);

	/* Initialisation de ICW4 */
	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	/* désactivation du masquage des interruptions 
	 * mise a 0 du  registre IMR*/ 
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}
