/* multiline text maps
 * page boilerplates and trigger text
 */
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
empty(condition);
return 0;
}

/* extract and write to buffer */
int rMap::getbody(int page_id, char *buf, int n, int acr) {
int i, m;
init(page_id);
empty(buf);
query();
for (i = 1; i <= q->rows; i++) {
  m = strlen(q->v(i, 2));
  n -= m + 3; /* cr and nil and reserve */
  if (n > 0) {
    strcpy(buf, q->v(i, 2));
    buf += m;
    if (acr) *buf++ = '\n';
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

/* read from temp file and insert into map table
 * adjust the field positions and lengths according to the $nn[._] placeholders
 */
void rMap::slurp(int page_id, char *tmpf, int brdr) {
int i, j;
char rows[SMLSIZE];
char cols[SMLSIZE];
char lens[SMLSIZE];
char lins[MEDSIZE];
Block *fields;
int col=0, act, len;
char k;
char fldstr[3];
fields = NULL;
fldstr[2] = '\0';
if (brdr == 0 || brdr == 2) { /* we are maintaining a screen page */
  /* search for the fields block */
  forall(block) if (!strcmp(blki(i).table, "fields")) fields = &blki(i);
  if (fields) {
    letstrncpy((char*)fields->querystr, "update fields set line = ?, col = ?, dlen = ? where id = ?", MEDSIZE-2);
    fields->bindv[4] = NULL;
  }
}
tmpopen();
init(page_id);
letf((char*)querystr, MEDSIZE, "delete from %s where page_id = %d", table, page_id);
bindv[0] = NULL;
execute();
letf((char*)querystr, MEDSIZE, "insert into %s (page_id, line, mtext) VALUES (%d, ?, ?)", table, page_id);
bindv[2] = NULL;
for (j = brdr / 2; tmpget(lins, MEDSIZE); j++) {
  if ((i = strlen(lins)) > 1) {
    lins[i-1] = '\0';
    letf(t(rows), "%d", j+1);
    bindv[0] = rows;
    bindv[1] = lins;
    execute();
    if (fields) { /* we want to maintain the fields pos and length */
      letf(t(rows), "%d", j + brdr/2);
      fields->bindv[0] = rows;
      for (col=0; lins[col]; col++) {
        k = lins[col];
        if (k == '$') {
          act = col;
          fldstr[0] = lins[++col];
          fldstr[1] = k = lins[++col];
          len = 2;
          while(k == '_' || k == '.' || (k >= '0' && k <= '9')) { len = k=='.' ? 1 : len+1; k = lins[++col]; }
          letf(t(cols), "%d", act + brdr/2);
          fields->bindv[1] = cols;
          letf(t(lens), "%d", len);
          fields->bindv[2] = lens;
          fields->bindv[3] = fldstr;
          fields->execute();
        }
      }
    }
  }
}
rclose();
tmpclose(1);
}

