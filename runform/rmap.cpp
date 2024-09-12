#include <stdlib.h>
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
char *tmpath;
int i, m;
char cr[2] = "\n";
init(page_id);
query();
tmpath = tmpcreat();
m = 1;
for (i = 1; i <= q->rows; i++) {
  while(q->m(i, 1) > m++) tmput(cr);
  tmput(q->v(i, 2));
  tmput(cr);
}
tmpclose();
rclose();
return tmpath;
}

void rMap::slurp(int page_id, char *tmpf) {
int i, m;
char r[SMLSIZE];
char l[MEDSIZE];
tmpopen();
init(page_id);
letf((char*)querystr, sizeof(querystr), "delete from %s where page_id = %d", table, page_id);
bindv[0] = NULL;
execute(querystr, bindv);
letf((char*)querystr, sizeof(querystr), "insert into %s (page_id, line, mtext) VALUES (%d, ?, ?)", table, page_id);
bindv[2] = NULL;
for(m=1; tmpget(l, MEDSIZE); m++) {
  if ((i = strlen(l)) > 1) {
    l[i-1] = '\0';
    letf(t(r), "%d", m);
    bindv[0] = r;
    bindv[1] = l;
    execute(querystr, bindv);
  }
}
rclose();
tmpclose();
tmprm();
}

