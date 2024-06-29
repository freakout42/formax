#include <stdio.h>
void cqerror (s)  /* Called by yyparse on error */
	char *s;
{
fprintf (stderr, "%s\n", s);
}
