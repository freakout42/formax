#ifndef __cplusplus
int cqlex(void);
int cqerror(char *s);
int parse (char *i, char *o);
int cqparse();
int cqwrap();
int cqinput(void);
int colquery(char *where, char *query, char *colname, int nonum, int type);
#else
extern "C" {
int colquery(char *where, const char *query, const char *colname, int nonum, int type);
}
#endif
