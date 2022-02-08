#include "../lib.h"
#include "mm.h"
#include "kmalloc.h"
#include "../Entrés Sorties/screen.h"
void *kheap_extend(int n)
{
	struct kmalloc_header *chunk;
	char *p_addr;
	int i;
	/*si l'ajoute des pages dépasse la limite du heap*/
	if ((kern_heap + (n * PAGESIZE)) > (char *) KERN_HEAP_LIM) {
		printk("PANIC: heap_extend(): no virtual memory left for kernel heap !\n");
		return (char *) -1;
	}

	
	/* Allocation d'une page libre */
	for (i = 0; i < n; i++) {
		p_addr = get_page_frame();
		if (p_addr < 0) {
			printk("PANIC: heap_extend(): no free page frame available !\n");
			return (char *) -1;
		}
		
		/* Ajout dans le repertoire de pages */
		pd0_add_page(kern_heap, p_addr, 0);
		kern_heap += PAGESIZE;
	}
	/* Création d'un entete */
	chunk = (struct kmalloc_header *) kern_heap;
	chunk->size = PAGESIZE * n;
	chunk->used = 0;

	return chunk;
}

void *kmalloc(unsigned long size){
	
	unsigned long realsize;	/* taille totale de l'enregistrement */
	//KMALLOC_MINSIZE=16
	//realsize = la taille des données + la taille de l'entete
	if ((realsize =sizeof(struct kmalloc_header) + size) < KMALLOC_MINSIZE)
		realsize = KMALLOC_MINSIZE;

	/* 
	 * On recherche un bloc libre de 'size' octets en parcourant le HEAP
	 * kernel a partir du debut
	 */
	struct kmalloc_header *chunk, *other;
	chunk = (struct kmalloc_header *) KERN_HEAP;
	while (chunk->used || chunk->size < realsize) {
		//l'entete est corrempu (impossible qu'un bloc aille une taille de 0)
		if (chunk->size == 0) {
			printk("PANIC: kmalloc(): corrupted chunk on %x with null size (heap %x) !\nSystem halted\n",chunk, kern_heap);
			asm("hlt");
		}
		//monter vers le bloc prochain
		chunk =(struct kmalloc_header *) ((char *) chunk +chunk->size);

		if (chunk == (struct kmalloc_header *) kern_heap) {
			//on peut pas étendre le heap
			if (kheap_extend((realsize / PAGESIZE) + 1) < 0) {
				printk("PANIC: kmalloc(): no memory left for kernel !\nSystem halted\n");
				asm("hlt");
			}
		} else if (chunk > (struct kmalloc_header *) kern_heap) {
			printk("PANIC: kmalloc(): chunk on %x while heap limit is on %x !\nSystem halted\n", chunk, kern_heap);
			asm("hlt");
		}
	}
	/* 
	 * On a trouve un bloc libre dont la taille est >= 'size'
	 * On fait de sorte que chaque bloc est une taille minimale
	 */
	/*si la taille du bloc - realsize <16 (le bloc est suffisament grand)*/
	if (chunk->size - realsize < KMALLOC_MINSIZE)
		chunk->used = 1;
	else {
		/*le bloc est trés grand (différence >16)
		 *création d'un second block qui exploite l'espace libre*/
		other =(struct kmalloc_header *) ((char *) chunk + realsize);
		other->size = chunk->size - realsize;
		other->used = 0;

		chunk->size = realsize;
		chunk->used = 1;
	}

	/* retourne un pointeur sur la zone de donnees */
	return (char *) chunk + sizeof(struct kmalloc_header);
}

void kfree(void *v_addr)
{
	struct kmalloc_header *chunk, *other;

	/* On libere le bloc alloue */
	chunk = (struct kmalloc_header *) (v_addr - sizeof(struct kmalloc_header));
	chunk->used = 0;

	/* 
	 * On merge le bloc nouvellement libere avec le bloc suivant ci celui-ci
	 * est aussi libre
	 * other pointe vers le bloc suivant
	 */
	while ((other =
		(struct kmalloc_header *) ((char *) chunk + chunk->size))
	       && other < (struct kmalloc_header *) kern_heap
	       && other->used == 0)
   
		chunk->size += other->size;
}

int pd0_add_page(char *v_addr, char *p_addr, int flags)
{
	u32 *pde;
	u32 *pte;

	if (v_addr > (char *) USER_OFFSET) {
		printk("ERROR: pd0_add_page(): %p is not in kernel space !\n", v_addr);
		return 0;
	}
	
	/* On verifie que la table de page est bien presente */
	pde = (u32 *) (0xFFFFF000 | (((u32) v_addr & 0xFFC00000) >> 20));
	printk("\npde : %x",pde);
	if ((*pde & PG_PRESENT) == 0) {
		printk("PANIC: pd0_add_page(): kernel page table not found for v_addr %p. System halted !\n", v_addr);
		asm("hlt");
	}
	/* Modification de l'entree dans la table de page */
	pte = (u32 *) (0xFFC00000 | (((u32) v_addr & 0xFFFFF000) >> 10));
	*pte = ((u32) p_addr) | (PG_PRESENT | PG_WRITE | flags);
	return 0;
}
