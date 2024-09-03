#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "colquery.h"
#include "colq.h"

#define BUFSIZE 65536

int cqnonumbers;
int cqtype;
int cqcoltype;
char *cqcolumn;
char *cqbuf;
int cqbufree;

int colquery (char *i, char *o, char *c, int n, int t) {
char buffer[BUFSIZE];

cqcolumn = c;
cqnonumbers = n;
cqtype = t;
cqcoltype = 0;
cqbuf = buffer;
cqbufree = BUFSIZE;
return parse (i, o) ? 0 : (cqcoltype?cqcoltype:SQLNULL);
}

int cqwrap(void) { return 1; }
