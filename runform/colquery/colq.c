#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "colquery.h"

int main (int argc, char *argv[]) {
int rc = 0;            /* parser return code			*/
char query[2048];      /* query output buffer			*/
char where[1024] = ""; /* input buffer				*/
char *colname;         /* column name				*/
int nonum = 0;         /* no numbers mode   -q option		*/
int type = 0;          /* type mode         -a option		*/

colname = strdup("id");
strcpy(where, argv[1]);
rc = colquery (where, query, colname, nonum, type);
if (rc) {
  printf ("q=\"%s\" rc=%d\n", query, rc);
} else {
  return 1;
}
strcpy(where, argv[2]);
rc = colquery (where, query, colname, nonum, type);
if (rc) {
  printf ("q=\"%s\" rc=%d\n", query, rc);
  return 0;
} else {
  return 1;
}
}
