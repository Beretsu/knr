#ifndef HDR_STREAM
#define HDR_STREAM

#define EMPTY 0
#define FULL 1
#define CIFUN struct charinfo (*)(void *)
#define INTFUN int (*)(void *)

extern struct stream strm_lw;

struct charinfo {
	int c;
	int lineno;
};

struct wordinfo {
	char *word;
	int lineno;
};

struct cifun {
	struct charinfo (*fun)(void *);
	void *args;
};

struct intfun {
	int (*fun)(void *);
	void *args;
};

struct cifuninfo {
	struct cifun *pf;
	int n;
};

struct stream {
	struct cifun *pfun;
	struct charinfo buf;
	int bufstate;
};

int getlower(int (*)(void));
struct charinfo make_ci(struct intfun *);
struct charinfo line_ci(struct cifuninfo *);

struct charinfo getci(struct stream *);
struct charinfo ungetci(struct stream *, struct charinfo);

int getword(struct stream *strm, struct wordinfo *wi, int lim);
int asalpha(int);
int asalnum(int);

#endif
