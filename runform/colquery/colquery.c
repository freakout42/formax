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

int colquery (i, o, c, n, t)
	char *i, *o, *c; int n, t;
{
char buffer[BUFSIZE];

cqcolumn = c;
cqnonumbers = n;
cqtype = t;
cqcoltype = 0;
cqbuf = buffer;
cqbufree = BUFSIZE;
return parse (i, o) ? 0 : (cqcoltype?cqcoltype:SQLNULL);
}

int cqwrap() { return 1; }
