#ifndef HDR_FORMAT
#define HDR_FORMAT

struct info {
	int col;
	int state;
};

struct intfun {
	int (*fun)(void *);
	void *args;
};

struct calldata {
	struct intfun *fun;
	struct info *nfo;
};

struct doublebuf {
	char *buf;
	int bufsize;
	int whead;
	int wtail;
	int rhead;
	int rtail;
	int breakpnt;
	int mode;
};

struct wbounder {
	struct calldata *cd;
	struct doublebuf *db;
};


int getcolumn(struct ifun *sc)
int unblank(struct ifun *p)
int wordbound(struct bufun *p)

