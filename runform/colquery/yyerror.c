#include <stdio.h>
cqerror (s)  /* Called by yyparse on error */
	char *s;
{
fprintf (stderr, "%s\n", s);
}
