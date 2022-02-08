int block_read(int, int, int, char *);	/* drive, block, count, buf */
int block_write(int, int, int, char *);
int disk_read(int, int, char *, int);	/* drive, offset, buf, count */