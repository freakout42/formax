// top objects of any form
#include "runform.h"

int Block::init(Qdata *blk, int rix) {
let(table,  blk->v(rix, 1));
let(prikey, blk->v(rix, 2));
let(where,  blk->v(rix, 3));
let(order,  blk->v(rix, 4));
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
return fieldcount;
}

int Block::select() {
int i;
char wall[MEDSIZE];
*wall = '\0';
for (i=0; i<fieldcount; i++) {
  if (f.l[blockfields[i]].qwhere[0]) {
    if (*wall) cats(t(wall), " AND ");
               cats(t(wall), "(");
               cats(t(wall), f.l[blockfields[i]].qwhere);
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
letf((char*)querystr, sizeof(querystr), "update %s set %s = ? where %s = ?", table, cn(c-1), f.l[primarykeys[0]].name);
bindv[0] = q->v(r,c);
bindv[1] = q->v(r,1); //f.l[primarykeys[0]].num);
bindv[2] = NULL;
return execute(querystr, bindv);
}

