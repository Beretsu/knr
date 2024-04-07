/*
 * Exercise 8-2. Rewrite fopen and _fillbuf with fields instead of explicit bit
 * operations. Compare code size and execution speed.
*/

#define NULL		0
#define EOF		(-1)
#define BUFSIZ		1024
#define OPEN_MAX	20

struct _flags {
	unsigned int _read	: 1;	/* file open for reading */
	unsigned int _write	: 1;	/* file open for writing */
	unsigned int _unbuf	: 1;	/* file is unbuffered */
	unsigned int _eof	: 1;	/* EOF has occurred on this file */
	unsigned int _err	: 1;	/* error occurred on this file */
};

typedef struct _iobuf {
	int cnt;		/* characters left */
	char *ptr;		/* next character position */
	char *base;		/* location of buffer */
	struct _flags flag;	/* mode of file access */
	int fd;			/* file descriptor */
} FILE;
extern FILE _iob[OPEN_MAX];

#define stdin	(&_iob[0])
#define stdout	(&_iob[1])
#define stderr	(&_iob[2])

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);

#define feof(p)		(((p)->flag._eof) != 0)
#define ferror(p)	(((p)->flag._err) != 0)
#define fileno(p)	((p)->fd)

#define getc(p)		(--(p)->cnt >= 0 \
		? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putc(x,p)		(--(p)->cnt >= 0 \
		? *(p)->ptr++ = (x) : _flushbuf((x),p))

#define getchar()	getc(stdin)
#define putchar(x)	putc((x), stdout)

#include <fcntl.h>
#undef BUFSIZ
#include "syscalls.h"
#define PERMS 0666	/* RW for owner, group, others */

/* fopen: open file, return file ptr */
FILE *fopen(char *name, char *mode)
{
	int fd;
	FILE *fp;

	if (*mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULL;
	for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
		if (!fp->flag._read && !fp->flag._write)
			break;		/* found free slot */
	if (fp >= _iob + OPEN_MAX)	/* no free slots */
		return NULL;

	if (*mode == 'w')
		fd = creat(name, PERMS);
	else if (*mode == 'a') {
		if ((fd = open(name, O_WRONLY, 0)) == -1)
			fd = creat(name, PERMS);
		lseek(fd, 0L, 2);
	} else
		fd = open(name, O_RDONLY, 0);
	if (fd == -1)	/* couldn't access name */
		return NULL;
	fp->fd = fd;
	fp->cnt = 0;
	fp->base = NULL;
	(*mode == 'r') ? (fp->flag._read = 1) : (fp->flag._write = 1);
	return fp;
}

#include <stdlib.h>

/* _fillbuf: allocate and fill input buffer */
int _fillbuf(FILE *fp)
{
	int bufsize;

	if (!fp->flag._read || fp->flag._eof || !fp->flag._err)
		return EOF;
	bufsize = (fp->flag._unbuf) ? 1 : BUFSIZ;
	if (fp->base == NULL)	/* no buffer yet */
		if ((fp->base = (char *) malloc(bufsize)) == NULL)
			return EOF;	/* can't get buffer */
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);
	if (--fp->cnt < 0) {
		if (fp->cnt == -1)
			fp->flag._eof = 1;
		else 
			fp->flag._err = 1;
		fp->cnt = 0;
		return EOF;
	}
	return (unsigned char) *fp->ptr++;
}

FILE _iob[OPEN_MAX] = {		/* stdin, stdout, stderr */
	{ 0, (char *) 0, (char *) 0, {1,0,0,0,0}, 0 },
	{ 0, (char *) 0, (char *) 0, {0,1,0,0,0}, 1 },
	{ 0, (char *) 0, (char *) 0, {0,1,1,0,0}, 2 }
};

/* testing stdio */
int main(int argc, char *argv[])
{
	int c;
	FILE *fp;
	void error(char *, ...);

	if ((fp = fopen(argv[1], "r")) == NULL)
		error("can't open %s", argv[1]);
	while ((c = getc(fp)) != EOF)
		/* putc(c, stdout); */
		; /* until _flushbuf is defined */
	if (ferror(stdout))
		error("writing stdout");

	return 0;
}

