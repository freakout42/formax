// top objects of any form
#include "runform.h"

int Block::init(Qdata *blk, int rix) {
let(table,  blk->v(rix, 1));
let(prikey, blk->v(rix, 2));
let(where,  blk->v(rix, 3));
let(order,  blk->v(rix, 4));
let(battrs, "");
return 0;
}

void Block::addattr(char *att) {
if (!*battrs) cats(t(battrs), ",");
cats(t(battrs), att);
}

