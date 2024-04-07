/*
 * Exercise 8-4. The standard library function
 * 	int fseek(FILE *fp, long offset, int origin)
 * is identical to lseek except that fp is a file pointer instead of a file
 * descriptor and the return value is an int status, not a position. Write
 * fseek. Make sure that your fseek coordinates properly with the buffering
 * done for the other functions of the library.
*/


#define NULL		0
#define EOF		(-1)
#define BUFSIZ		1024
#define OPEN_MAX	20

typedef struct _iobuf {
	int cnt;	/* characters left */
	char *ptr;	/* next character position */
	char *base;	/* location of buffer */
	int flag;	/* mode of file access */
	int fd;		/* file descriptor */
} FILE;
extern FILE _iob[OPEN_MAX];

#define stdin	(&_iob[0])
#define stdout	(&_iob[1])
#define stderr	(&_iob[2])


enum _flags {
	_READ	= 01,	/* file open for reading */
	_WRITE	= 02,	/* file open for writing */
	_UNBUF	= 04,	/* file is unbuffered */
	_EOF	= 010,	/* EOF has occurred on this file */
	_ERR	= 020,	/* error occurred on this file */
};

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);

#define feof(p)		(((p)->flag & _EOF) != 0)
#define ferror(p)	(((p)->flag & _ERR) != 0)
#define fileno(p)	((p)->fd)

#define getc(p)		(--(p)->cnt >= 0 \
		? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putc(x,p)	(--(p)->cnt >= 0 \
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
		if ((fp->flag & (_READ | _WRITE)) == 0)
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
	fp->flag = (*mode == 'r') ? _READ : _WRITE;
	return fp;
}

#include <stdlib.h>

/* fclose: flush file and close underlying file descriptor */
int fclose(FILE *fp)
{
	int err = 0, fflush(FILE *);
	if ((fp->flag & _WRITE) && fflush(fp))
		err = 1;	/* error flushing output buffer */
	free(fp->base);
	fp->base = fp->ptr = NULL;
	fp->flag = 0;
	fp->cnt = 0;
	close(fp->fd);
	return err;
}

/* _fillbuf: allocate and fill input buffer */
int _fillbuf(FILE *fp)
{
	int bufsize;

	if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
		return EOF;
	bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
	if (fp->base == NULL)	/* no buffer yet */
		if ((fp->base = (char *) malloc(bufsize)) == NULL)
			return EOF;	/* can't get buffer */
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);
	if (--fp->cnt < 0) {
		if (fp->cnt == -1)
			fp->flag |= _EOF;
		else 
			fp->flag |= _ERR;
		fp->cnt = 0;
		return EOF;
	}
	return (unsigned char) *fp->ptr++;
}

/* _flushbuf: write and empty output buffer */
int _flushbuf(int c, FILE *fp)
{
	int bufsize;

	if ((fp->flag & (_WRITE | _ERR)) != _WRITE)
		return EOF;
	bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
	if (fp->base == NULL) {	/* no buffer yet */
		if ((fp->base = (char *) malloc(bufsize)) == NULL)
			return EOF;	/* can't get buffer */
	} else {		/* buffer is full */
		if (write(fp->fd, fp->base, bufsize) < bufsize) {
			fp->flag |= _ERR;
			return EOF;
		}
	}
	fp->ptr = fp->base;
	*fp->ptr++ = c;			/* add c to buffer */
	fp->cnt = bufsize - 1;
	return (unsigned int) c;
}

/* fflush: write buffered data */
int fflush(FILE *fp)
{
	int n, bufsize, err = 0;

	if (fp == NULL) {		/* flush all open output streams */
		for (n = 0; n < OPEN_MAX; n++)
			if (_iob[n].flag & _WRITE && fflush(_iob+n))
				err = EOF;
		return err;
	}
	if ((fp->flag & (_WRITE | _ERR)) != _WRITE)	/* can't write */
		return EOF;
	bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
	n = bufsize - fp->cnt;		/* chars in buf */
	fp->cnt = bufsize;
	fp->ptr = fp->base;
	if (write(fp->fd, fp->ptr, n) < n) {
		fp->flag |= _ERR;
		return EOF;
	}
	return 0;
}

/* fseek: sets the position of fp to offset from origin */
int fseek(FILE *fp, long offset, int origin)
{
	if (fp == NULL)
		return 1;
	if ((fp->flag & _UNBUF) || fp->base == NULL) /* no buf to adjust */
		return lseek(fp->fd, offset, origin) < 0;

	if (fp->flag & _READ) {
		if (origin == 1)		/* from current position */
			offset -= fp->cnt; 	/* retract _fillbuf */
		fp->cnt = 0;			/* _fillbuf on next getchar*/
		fp->ptr = fp->base;
	} else if (fp->flag & _WRITE) {
		if (fflush(fp))		/* write buf before seeking */
			return 1;
	}
	if (lseek(fp->fd, offset, origin) == -1)
		return 1;
	fp->flag &= ~_EOF;		/* retract EOF */
	return 0;
}


FILE _iob[OPEN_MAX] = {		/* stdin, stdout, stderr */
	{ 0, (char *) 0, (char *) 0, _READ, 0 },
	{ 0, (char *) 0, (char *) 0, _WRITE, 1 },
	{ 0, (char *) 0, (char *) 0, _WRITE | _UNBUF, 2 }
};

/* testing stdio */
int main(int argc, char *argv[])
{
	int c, i;
	FILE *fp;
	void error(char *, ...);

	if ((fp = fopen(argv[1], "r")) == NULL)
		error("can't open %s", argv[1]);
	for (i = 0; i < 10; ++i)
		putchar(getc(fp));
	if (fseek(fp, -5, 1))
		error("seeking");
	while ((c = getc(fp)) != EOF)
		putchar(c);
	if (ferror(stdout))
		error("writing stdout");
	fclose(stdout);
	fclose(fp);
	return 0;
}
