#ifndef HDR_STREAM
#define HDR_STREAM

#define EMPTY 0
#define FULL 1
#define CIFUN struct charinfo (*)(void *)
#define INTFUN int (*)(void *)

extern struct stream strm_in;
extern struct stream strm_code;

enum filtermode {
        CODE, OPCOMMENT, CLCOMMENT, COMMENT,
        PREPROC, QUOTE, ESC
};

struct charinfo {
	int c;
	enum filtermode mode;
};

struct cifun {
	struct charinfo (*fun)(void *);
	void *args;
};

struct intfun {
	int (*fun)(void *);
	void *args;
};

struct stream {
	struct cifun *pfun;
	struct charinfo buf;
	int bufstate;
};

struct strminfo {
	struct stream *strm;
	enum filtermode mode;
};

struct charinfo make_ci(struct intfun *);
struct charinfo mode_ci(struct strminfo *);
struct charinfo code_ci(struct cifun *);

struct charinfo getci(struct stream *);
struct charinfo ungetci(struct stream *, struct charinfo);

int getword(struct stream *strm, char *w, int lim);
int asalpha(int);
int asalnum(int);

#endif
