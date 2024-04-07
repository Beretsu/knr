#include <stdio.h>
#define NBYTES sizeof(Header) * 2
#define NBYTESPLUS NBYTES + sizeof(Header)

typedef long Align;     /* for alignment to long boundary */

union header {          /* block header */
        struct {
                union header *ptr;      /* next block if on free list */
                unsigned size;          /* size of this block */
        } s;
        Align x;        /* force alignment of blocks */
};

typedef union header Header;

int main()
{
	void *mymalloc(unsigned), bfree(void *, unsigned);
	char v[NBYTESPLUS], *pv;

	bfree((void *)v, NBYTESPLUS);	
	pv = (char *) mymalloc(NBYTES);
	if (v+sizeof(Header) == pv)
		printf("bfreed successfully\n");
	return 0;
}
