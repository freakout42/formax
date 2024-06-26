// top objects of any form
#include "runform.h"

int Block::init(Qdata *blk, int rix) {
let(table,  blk->v(rix, 1));
let(prikey, blk->v(rix, 2));
let(where,  blk->v(rix, 3));
let(order,  blk->v(rix, 4));
let(attrs, "");
bnumfs = 0;
return 0;
}

void Block::addattr(int att) {
if (*attrs) cats(t(attrs), ",");
cats(t(attrs), f.l[att].name);
bflds[bnumfs++] = att;
}

int Block::select() {
int i;
char wall[MEDSIZE];
*wall = '\0';
for (i=0; i<bnumfs; i++) {
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

