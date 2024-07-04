// top objects of any form
#include "runform.h"

int Block::init(Qdata *blk, int rix) {
let(table,  blk->v(rix, 1));
let(prikey, blk->v(rix, 2));
let(where,  blk->v(rix, 3));
let(order,  blk->v(rix, 4));
let(attrs,  "");
fieldcount = 0;
return 0;
}

int Block::addattribute(int att) {
if (*attrs) cats(t(attrs), ",");
cats(t(attrs), f.l[att].name);
if (!(strcmp(f.l[att].name, prikey))) bprikf = fieldcount;
bflds[fieldcount++] = att;
return fieldcount;
}

int Block::select() {
int i;
char wall[MEDSIZE];
*wall = '\0';
for (i=0; i<fieldcount; i++) {
  if (f.l[bflds[i]].qwhere[0]) {
    if (*wall) cats(t(wall), " AND ");
               cats(t(wall), "(");
               cats(t(wall), f.l[bflds[i]].qwhere);
               cats(t(wall), ")");
  }
}
if (*wall) let(where, wall);
return query();
}

char *Block::cn(int c) {
return f.l[bflds[c]].name;
}

int Block::update(int r, int c) {
letf((char*)querystr, sizeof(querystr), "update %s set %s = ? where %s = ?", table, cn(c-1), prikey);
bindv[0] = q->v(r,c);
bindv[1] = q->v(r,bprikf+1);
bindv[2] = NULL;
return execute(querystr, bindv);
}

