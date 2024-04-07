char *alloc(int);
void afree(char *);

char **palloc(int);
void pafree(char **);

char ***ppalloc(int);
void ppafree(char ***);

char ****pppalloc(int);
void pppafree(char ****);

int (**falloc(int))(const char *, const char *);
void fafree(int (**)(const char *, const char *));
