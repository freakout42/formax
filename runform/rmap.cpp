/* multiline text maps
 * page boilerplates and trigger text (not yet)
 */
#include "runform.h"

int rMap::init(int page_id) {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,     "maps");
let(prikey,    "id");
let(attrs,     "line,mtext");
letf(t(where), "page_id = %d and line>0", page_id);
let(order,     "line");
columni = 2;
return 0;
}

/* extract and write to buffer */
int rMap::getbody(int page_id, char *buf, int n) {
int i, m;
init(page_id);
query();
for (i = 1; i <= q->rows; i++) {
  m = strlen(q->v(i, 2));
  n -= m + 3; /* cr and nil and reserve */
  if (n > 0) {
    strcpy(buf, q->v(i, 2));
    buf += m;
    *buf++ = '\n';
    *buf++ = '\0';
  }
}
rclose();
return n < 0;
}

/* extract and write to temp file */
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
tmpclose(0);
rclose();
return tmpath;
}

/* read from temp file and insert into map table */
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
tmpclose(1);
}

