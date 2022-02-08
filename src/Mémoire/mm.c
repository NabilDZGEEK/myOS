#define __MM__
#include "mm.h"
#include "../lib.h"
#include "kmalloc.h"
/*
 * Parcours le bitmap a la recherche d'une page libre et la marque
 * comme utilisee avant de retourner son adresse physique.
 */
char* get_page_frame(void)
{
	int byte, bit;
	int page = -1;

	for (byte = 0; byte < RAM_MAXPAGE / 8; byte++)
		if (mem_bitmap[byte] != 0xFF)
			for (bit = 0; bit < 8; bit++)
				if (!(mem_bitmap[byte] & (1 << bit))) {
					page = 8 * byte + bit;
					set_page_frame_used(page);
					return (char *) (page * PAGESIZE);
				}
	return (char *) -1;
}

/* Cree un mapping tel que vaddr = paddr sur 4Mo */
void init_mm(void)
{
	u32 page_addr;
	int i, pg;
	
	/* Initialisation du bitmap de pages physiques */
	for (pg = 0; pg < RAM_MAXPAGE / 8; pg++)
		mem_bitmap[pg] = 0;

	/* Pages reservees pour le noyau */
	for (pg = PAGE(0x0); pg < PAGE(0x20000); pg++) 
		set_page_frame_used(pg);

	/* Pages reservees pour le hardware */
	for (pg = PAGE(0xA0000); pg < PAGE(0x100000); pg++) 
		set_page_frame_used(pg);
	/* Prend une page pour le Page Directory et une pour la Page Table[0] */
	pd = (u32*) get_page_frame();
	printk("\nadresse pd = %x",pd);
	//printk("\n2eme page adresse = %x",get_page_frame());
	/* Initialisation du Page Directory */
	
	for(i=0;i<25;i++) {
		pt[i] = (u32*) get_page_frame();
		printk("\n pt[%d] : %x",i,pt[i]);
		pd[i] = (u32) pt[i];
		pd[i] |= 3;
	}
	for (i = 25; i < 1024; i++)
		pd[i] = 0;

	/* Initialisation des Page Table */
	page_addr = 0;
	for (pg = 0; pg < 25*1024; pg++) {
		pt[0][pg] = page_addr;
		pt[0][pg] |= 3;
		page_addr += 4096;
	}
	printk("lala");
	printk("\nl'adresse : %x",get_page_frame());

		/*activation de la pagination*/
	asm("	mov %0, %%eax \n \
		mov %%eax, %%cr3 \n \
		mov %%cr0, %%eax \n \
		or %1, %%eax \n \
		mov %%eax, %%cr0"::"m"(pd), "i"(PAGING_FLAG));
		/* Initialisation du heap du noyau utilise par kmalloc */
	kern_heap = (char *) KERN_HEAP;
	//printk("%x",kern_heap);
	kheap_extend(1);
}
