// workhorse object
#include "runform.h"

int Field::init(Qdata *fld, int rix) {
let(name,  fld->v(rix, 1));
dlen =     fld->n(rix, 2);
line =     fld->n(rix, 3);
col  =     fld->n(rix, 4);
//blk = &f.b[fld->n(rix, 5)];
let(qhuman, "");
let(qwhere, "");
//blk->addattr(name);
return 0;
}

void Field::fshow(int cur) {
char *data;
     if (f.rmode == MOD_QUERY) data = qhuman;
//else if (blk->q)               data = blk->q->v(1, 1);
else                           data = name;
f.p[1].writef(line, col, cur ? COL_CURRENT : COL_FIELD, dlen, "%s", data);
if (cur) f.p[1].wmov(line, col);
}

