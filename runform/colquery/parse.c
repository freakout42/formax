#ifdef DEBUG
#include <stdio.h>
#endif
#include "colq.h"

char *cqop;
static char *ip;

int cqinput ()
{
#ifdef DEBUG
fprintf(stderr,"i:0%02x\n", *ip);
#endif
if (*ip=='\0') {ip++; return '\n';}
return *ip++;
}

void unput ()
{
ip--;
}

int parse (i, o)
	char *i, *o;
{
#ifdef DEBUG
int status;
#endif

ip = i;
cqop = o;

#ifdef DEBUG
fprintf(stderr,"i:%s\n", i);
status = cqparse();
fprintf(stderr,"o:%s\n", o);
return status;
#else
return cqparse();
#endif

}
