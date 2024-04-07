int getcolumn(struct col *sc)
{
	int c;
	int cols = sc->strmfo->cols;

	switch (c = sc->fun(sc->args)) {
	case '\t':
		cols += sc->tabstop - cols % sc->tabstop;
		break;
	case '\b':
		cols = (cols > 0) ? --cols : cols;
		break;
	case '\n':
		cols = 0;
		break;
	default:
		++cols;
		break;
	}
	sc->strmfo->cols = cols;
	return c;
}

int unblank(struct pfun *p)
{
	int c, col;

	col = p->strmfo->col;
	while (isblank(c = p->fun(p->args)) && p->strmfo->state == OUT)
		p->strmfo->col;
	p->strmfo->state = isblank(c) ? OUT : IN;
	return c;
}

int wordbound(struct bufun *p)
{
	if (p->found_eof == YES && isempty(p->qout)) {
		return EOF;
	} else if (p->newline == YES && isempty(p->qout)) {
		p->newline = NO;
		p->strmfo->col = 0;
		return '\n';
	} else if (isempty(p->qout)) {
		end = NULL;
		while ((c=p->fun(p->args))!='\n' && c!=EOF
		&& p->strmfo->col <= p->maxline) {
			if (isblank(c))
				end = p->qout->pos;
			enqueue(p->qout, c);
		}
		if (c == '\n') {
			enqueue(p->qout, c);
		} else if (c == EOF) {
			p->found_eof = YES;
			if (isempty(p->qout))
				return EOF;
		} else {
			p->newline = YES;
			if (end == NULL) {
				p->qin->start = p->qout->end;
				p->qin->end = p->qout->end;
			} else {
				p->qin->end = p->qout->end;
				p->qin->start = p->qout->end = end;
			}
			enqueue(p->qin, c);
		}
	}
	return dequeue(p->qout);
}

int wordbound(struct doublebuf *p)
{
	if (isempty(p)) {
		while ((c=getnext(p))!='\n' && c!=EOF
		&& p->strmfo->col <= p->maxline) {
			if (isblank(c))
				end = p->qout->pos;
			enqueue(p->qout, c);
		}
		if (c == '\n') {
			enqueue(p->qout, c);
		} else if (c == EOF) {
			p->found_eof = YES;
			if (isempty(p->qout))
				return EOF;
		} else {
			p->newline = YES;
			if (end == NULL) {
				p->qin->start = p->qout->end;
				p->qin->end = p->qout->end;
			} else {
				p->qin->end = p->qout->end;
				p->qin->start = p->qout->end = end;
			}
			enqueue(p->qin, c);
		}
	}
	return dequeue(p->qout);
}
