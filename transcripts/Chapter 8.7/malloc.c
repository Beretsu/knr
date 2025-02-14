#include <stddef.h>	/* for NULL */

typedef long Align;	/* for alignment to long boundary */

union header {		/* block header */
	struct {
		union header *ptr;	/* next block if on free list */
		unsigned size;		/* size of this block */
	} s;
	Align x;	/* force alignment of blocks */
};

typedef union header Header;

static Header base;		/* empty list to get started */
static Header *freep = NULL;	/* start of free list */

/* malloc: general-purpose storage allocator */
void *mymalloc(unsigned nbytes)
{
	Header *p, *prevp;
	Header *morecore(unsigned);
	unsigned nunits;

	nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;
	if ((prevp = freep) == NULL) {	/* no free list yet */
		base.s.ptr = freep = prevp = &base;
		base.s.size = 0;
	}
	for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
		if (p->s.size >= nunits) {		/* big enough */
			if (p->s.size == nunits)	/* exactly */
				prevp->s.ptr = p->s.ptr;
			else {			/* allocate tail end */
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size = nunits;
			}
			freep = prevp;
			return (void *)(p+1);
		}
		if (p == freep)		/* wrapped around free list */
			if ((p = morecore(nunits)) == NULL)
				return NULL;	/* non left */
	}
}

#define NALLOC 1024	/* minimun #units to request */

/* morecore: ask system for more memory */
Header *morecore(unsigned nu)	/* removed static declaration; error */
{
	void myfree(void *);
	char *cp, *sbrk(int);
	Header *up;

	if (nu < NALLOC)
		nu = NALLOC;
	cp = sbrk(nu * sizeof(Header));
	if (cp == (char *) -1)	/* no space at all */
		return NULL;
	up = (Header *) cp;
	up->s.size = nu;
	myfree((void *)(up+1));
	return freep;
}

/* free: put block ap in free list */
void myfree(void *ap)
{
	Header *bp, *p;

	bp = (Header *)ap - 1;	/* point to block header */
	for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
			break;	/* freed block at start or end of arena */
	if (bp + bp->s.size == p->s.ptr) {	/* joint to upper nbr */
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	} else
		bp->s.ptr = p->s.ptr;
	if (p + p->s.size == bp) {		/* join to lower nbr */
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	} else
		p->s.ptr = bp;
	freep = p;
}
