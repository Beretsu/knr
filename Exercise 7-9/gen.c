#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>

#define MAXLINE 1000

#define ALNUM 2
#define ALPHA 4
#define CNTRL 8
#define DIGIT 16
#define GRAPH 32
#define LOWER 64
#define PRINT 128
#define PUNCT 256
#define SPACE 512
#define UPPER 1024
#define XDIGIT 2048

/* generate code with a map for is<atribute>(char c) family of funtions. */
int main(int argc, char *argv[])
{	
	void genmap(FILE *), gendefn(FILE *);
	if (argc == 1) {
		gendefn(stdout);
		genmap(stdout);
	} else {
		void appendfiles(FILE *, FILE *);
		FILE *infp, *outfp;
		char *outname = argv[--argc];
		if ((outfp = fopen(outname, "a")) == NULL) {
			fprintf(stderr, "can't open %s\n", outname);
			exit(1);
		}
		fprintf(outfp, "/* defines and map generated with %s */\n\n",
				argv[0]);
		gendefn(outfp);
		genmap(outfp);
		while (--argc > 0) {
			if ((infp = fopen(*++argv, "r")) == NULL) {
				fprintf(stderr, "can't open %s\n", *argv);
				exit(1);
			}
			appendfiles(infp, outfp);
			fclose(infp);
		}
	}
	exit(0);
}

/* appendfiles: copy infp to the end of outfp */
void appendfiles(FILE *infp, FILE *outfp)
{
	char line[MAXLINE];
	while (fgets(line, MAXLINE, infp) != NULL) {
		fprintf(outfp, "%s", line);
		if (ferror(outfp)) {
			fprintf(stderr, "error writing to output\n");
			exit(2);
		}
	}
	putc('\n', outfp);
}

#define NVALS 10	/* values per row */

/* genmap: write a declaration of map and initialize it's values */
void genmap(FILE *outfp)
{
	fprintf(outfp, "short map[] = {\n");	/* declaration */
	int rows = (UCHAR_MAX%NVALS == 0) ? UCHAR_MAX-NVALS :
		(UCHAR_MAX/NVALS)*NVALS; /* exclude last row */
	int i, j, getval(unsigned char);

	for (i = 0; i < rows; i += NVALS) {
		fprintf(outfp, "\t%4d, ", getval(i));		/* start */
		for (j = i+1; j < i+NVALS-1; ++j)
			fprintf(outfp, "%4d, ", getval(j));	/* middle */
		fprintf(outfp, "%4d,\n", getval(j));		/* end */
	}
	fprintf(outfp, "\t%4d, ", getval(i));	/* last row start */
	for (; i < UCHAR_MAX-1; ++i)
		fprintf(outfp, "%4d, ", getval(i));	/* last row middle */
	fprintf(outfp, "%4d\n};\n\n", getval(i)); /* end table with \n\n */
	
	if (ferror(outfp)) {
		fprintf(stderr,	"error writing to output\n");
		exit(2);
	}
}

/* gendefn: write defines of bitflag masks */
void gendefn(FILE *outfp)
{
	fprintf(outfp, "#define ALNUM 2\n");
	fprintf(outfp, "#define ALPHA 4\n");
	fprintf(outfp, "#define CNTRL 8\n");
	fprintf(outfp, "#define DIGIT 16\n");
	fprintf(outfp, "#define GRAPH 32\n");
	fprintf(outfp, "#define LOWER 64\n");
	fprintf(outfp, "#define PRINT 128\n");
	fprintf(outfp, "#define PUNCT 256\n");
	fprintf(outfp, "#define SPACE 512\n");
	fprintf(outfp, "#define UPPER 1024\n");
	fprintf(outfp, "#define XDIGIT 2048\n\n");
}

/* getval: return bit flags for char i */
int getval(unsigned char c)
{
	int n = 0;
	n = isalnum(c) ? n | ALNUM : n;
	n = isalpha(c) ? n | ALPHA : n;
	n = iscntrl(c) ? n | CNTRL : n;
	n = isdigit(c) ? n | DIGIT : n;
	n = isgraph(c) ? n | GRAPH : n;
	n = islower(c) ? n | LOWER : n;
	n = isprint(c) ? n | PRINT : n;
	n = ispunct(c) ? n | PUNCT : n;
	n = isspace(c) ? n | SPACE : n;
	n = isupper(c) ? n | UPPER : n;
	n = isxdigit(c) ? n | XDIGIT : n;
	return n;
}

