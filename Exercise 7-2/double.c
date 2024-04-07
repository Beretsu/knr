int get(struct doublequeue *dq)
{
	int c;

	if (dq->whead != dq->wtail) {
		c = dq->buf[dq->wtail];
		dq->wtail = (dq->wtail+1) % dq->bufsize;
		return c;
	}
	if (dq->mode == READ) {
		dq->whead = dq->wtail = dq->rhead;
		dq->mode = WRITE;
		c = dq->end;
		dq->end = EOF;
		return c;
	}
	return EOF;
}

int add(struct doublequeue *dq, int c)
{
	if (dq->whead+1 % dq->bufsize == dq->rtail) {
		printf("add: too many characters\n");
	} else if (c != EOF) {
		dq->buf[dq->whead] = c;
		dq->whead = (dq->whead+1) % dq->BUFSIZE;
	}
	return c;
}

int reget(struct doublequeue *dq)
{
	int c;

	if (dq->rhead != dq->rtail) {
		c = dq->buf[dq->rtail];
		dq->rtail = (dq->rtail+1) % dq->bufsize;
		return ci;
	}
	return EOF;
}

int checkpoint(struct doublequeue *dq)
{
	return dq->whead;
}

int breakbuf(struct doublequeue *dq, int chkpoint)
{
	if (chkpoint < dq->wtail && chkpoint > dq->whead) {
		dq->rhead = dq->whead;
		dq->rtail = chkpoint;
		dq->mode = READ;
		return 0;
	}
	return 1;
}
