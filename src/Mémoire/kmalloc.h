#include "../types.h"

#define KMALLOC_MINSIZE		16

struct kmalloc_header {
	unsigned long size:31;	/* taille totale de l'enregistrement */
	unsigned long used:1;
} __attribute__ ((packed));

void *kheap_extend(int);
void *kmalloc(unsigned long);
void kfree(void *);
int pd0_add_page(char *, char *, int);