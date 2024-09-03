#include <stdio.h>
/* Called by yyparse on error */
void cqerror (char *s) {
fprintf (stderr, "%s\n", s);
}
