// top objects of any form
#include "runform.h"

int Block::init(Qdata *blk, int rix) {
let(table,  blk->v(rix, 1));
norec =     blk->n(rix, 2);
let(prikey, blk->v(rix, 3));
let(where,  blk->v(rix, 4));
let(order,  blk->v(rix, 5));
let(attrs,  "");
fieldcount = 0;
prikeycnt = 0;
return 0;
}

int Block::addattribute(int att) {
if (*attrs) cats(t(attrs), ","); /* build the column list for query */
cats(t(attrs), f.l[att].name);
blockfields[fieldcount++] = att;
if (f.l[att].isprimarykey) primarykeys[prikeycnt++] = att;
columni = fieldcount;
return fieldcount;
}

int Block::select() {
int i;
char wall[MEDSIZE-2];
char sep[7];
*wall = '\0';
*sep = '\0';
for (i=0; i<fieldcount; i++) {
  if (f.l[blockfields[i]].querywhere[0]) {
    cats(t(wall), sep);
    strcpy(sep, " AND ");
    cats(t(wall), "(");
    cats(t(wall), f.l[blockfields[i]].querywhere);
    cats(t(wall), ")");
  }
}
if (*wall) let(where, wall);
return query();
}

char *Block::cn(int c) {
return f.l[blockfields[c]].name;
}

int Block::update(int r, int c) {
if (usebindvar) {
letf((char*)querystr, sizeof(querystr), "update %s set %s = ? where %s = ?", table, cn(c-1), f.l[primarykeys[0]].name);
bindv[0] = q->v(r, c);
bindv[1] = q->v(r, f.l[primarykeys[0]].sequencenum);
bindv[2] = NULL;
} else {
letf((char*)querystr, sizeof(querystr), "update %s set %s = '%s' where %s = '%s'", table, cn(c-1), q->v(r, c),
                                           f.l[primarykeys[0]].name, q->v(r, f.l[primarykeys[0]].sequencenum));
bindv[0] = NULL;
}
if ((ret = execute(querystr, bindv))) return ret;
return complete();
}

int Block::destroy(int r) {
if (usebindvar) {
letf((char*)querystr, sizeof(querystr), "delete from %s where %s = ?", table, f.l[primarykeys[0]].name);
bindv[0] = q->v(r, f.l[primarykeys[0]].sequencenum);
bindv[1] = NULL;
} else {
letf((char*)querystr, sizeof(querystr),
  "delete from %s where %s = '%s'", table, f.l[primarykeys[0]].name, q->v(r, f.l[primarykeys[0]].sequencenum));
bindv[0] = NULL;
}
if ((ret = execute(querystr, bindv))) return ret;
return complete();
}

int Block::insert(int r) {
int i, j;
char columnslist[MEDSIZE];
char valueslist[SMLSIZE];
char sep;
*columnslist = '\0';
*valueslist = '\0';
sep = '\0';
j = 0;
for (i=0; i<fieldcount; i++) {
  if (q->v(r, i+1)) {
    catc(t(columnslist), sep);
    cats(t(columnslist), f.l[blockfields[i]].name);
    catc(t(valueslist),  sep);
if (usebindvar) {
    cats(t(valueslist),  "?");
    bindv[j++] = q->v(r, i+1);
} else {
    letf(t(valueslist),  "'%s'", q->v(r, i+1));
}
    sep = ',';
  }
}
bindv[j] = NULL;
if (drv == ODR_ORACLE) {
//{CALL begin insert into %s (%s) values (%s) return %s into ?", table, columnslist, valueslist, }
  letf((char*)querystr, sizeof(querystr), "insert into %s (%s) values (%s)", table, columnslist, valueslist);
  if ((ret = execute(querystr, bindv))) return ret;
} else {
  letf((char*)querystr, sizeof(querystr), "insert into %s (%s) values (%s) returning %s", table, columnslist, valueslist, attrs);
  if ((ret = execute(querystr, bindv))) return ret;
  if ((ret = fetch(r))) MSG(MSG_NOREC);
}
return complete();
}

