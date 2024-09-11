#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "runform.h"

int rMap::init(int page_id) {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,     "maps");
let(prikey,    "id");
let(attrs,     "line,mtext");
letf(t(where), "page_id = %d", page_id);
let(order,     "line");
columni = 2;
return 0;
}

char *rMap::extract(int page_id) {
static char tfp[14];
int i, m;
FILE *tf;
init(page_id);
query();
strcpy(tfp, "/tmp/fmXXXXXX");
tf = fdopen(mkstemp(tfp), "w");
m = 1;
for (i = 1; i <= q->rows; i++) {
  while(q->m(i, 1) > m++) fputc('\n', tf);
  fputs(q->v(i, 2), tf);
  fputc('\n', tf);
}
fclose(tf);
rclose();
return tfp;
}

void rMap::slurp(int page_id, char *tmpf) {
int i, m;
FILE *tf;
char r[SMLSIZE];
char l[MEDSIZE];
if (page_id) {
init(page_id);
letf((char*)querystr, sizeof(querystr), "delete from %s where page_id = %d", table, page_id);
bindv[0] = NULL;
execute(querystr, bindv);
letf((char*)querystr, sizeof(querystr), "insert into %s (page_id, line, mtext) VALUES (%d, ?, ?)", table, page_id);
bindv[2] = NULL;
tf = fopen(tmpf, "r");
for(m=1; fgets(l, MEDSIZE, tf); m++) {
  if ((i = strlen(l)) > 1) {
    l[i-1] = '\0';
    letf(t(r), "%d", m);
    bindv[0] = r;
    bindv[1] = l;
    execute(querystr, bindv);
  }
}
fclose(tf);
rclose();
}
unlink(tmpf);
}

