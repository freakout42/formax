#ifndef __cplusplus
int cqlex();
int cqerror();
int parse();
int cqparse();
int cqwrap();
int cqinput();
#else
extern "C" {
#endif
int colquery(char *where, char *query, char *colname, int nonum, int type);
#ifdef __cplusplus
}
#endif
