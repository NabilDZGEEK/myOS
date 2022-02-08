#include "../types.h"

#define	PAGESIZE 	4096
#define	RAM_MAXPAGE	64000 //64 000 pages => pagination de 250 MO de RAM
#define	KERN_PDIR		0x20000
#define	KERN_STACK		0x2000
#define	KERN_BASE		0x1000
#define KERN_HEAP		0x100000
#define KERN_HEAP_LIM		0x6400000
#define PAGE(addr)		(addr) >> 12  //dévision par 4096

#define USER_OFFSET 0x6400000
#define USER_STACK  0xE0000000
#define	PAGING_FLAG 	0x80000000	/* CR0 - bit 31 */
#define PSE_FLAG	0x00000010	/* CR4 - bit 4  */

#define PG_PRESENT	0x00000001	/* page directory / table */
#define PG_WRITE	0x00000002
#define PG_USER		0x00000004
#define PG_4MB		0x00000080

#ifdef __MM__
	u32 *pt[25];			/* kernel page table */
	u32 *pd = (u32 *) KERN_PDIR;	/* kernel page directory */
	u8 mem_bitmap[RAM_MAXPAGE / 8];	/* bitmap allocation de pages (1 Go) */

#endif
char *kern_heap;
struct pd_entry {
	u32 present:1;
	u32 writable:1;
	u32 user:1;
	u32 pwt:1;
	u32 pcd:1;
	u32 accessed:1;
	u32 _unused:1;
	u32 page_size:1;
	u32 global:1;
	u32 avail:3;

	u32 page_table_base:20;
} __attribute__ ((packed));

struct pt_entry {
	u32 present:1;
	u32 writable:1;
	u32 user:1;
	u32 pwt:1;
	u32 pcd:1;
	u32 accessed:1;
	u32 dirty:1;
	u32 pat:1;
	u32 global:1;
	u32 avail:3;

	u32 page_base:20;
} __attribute__ ((packed));


/* Marque une page comme utilisee / libre dans le bitmap */
#define set_page_frame_used(page)	mem_bitmap[((u32) page)/8] |= (1 << (((u32) page)%8))
#define release_page_frame(p_addr)	mem_bitmap[((u32) p_addr/PAGESIZE)/8] &= ~(1 << (((u32) p_addr/PAGESIZE)%8))

/* Selectionne une page libre dans le bitmap */
char *get_page_frame(void);

/* Initialise les structures de donnees de gestion de la memoire */
void init_mm(void);

