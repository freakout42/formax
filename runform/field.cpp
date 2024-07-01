// workhorse object
#include "runform.h"

int Field::init(Qdata *fld, int rix) {
let(name,  fld->v(rix, 1));
dlen =     fld->n(rix, 2);
line =     fld->n(rix, 3);
col  =     fld->n(rix, 4);
let(qhuman, "");
let(qwhere, "");
blk = 1;
f.b[blk].addattr(this);
num = f.b[blk].bnumfs;
return 0;
}

void Field::fshow(int cur) {
char *data;
     if (f.rmode == MOD_QUERY) data = qhuman;
else if (f.b[blk].q->rows)     data = f.b[blk].q->v(1, num);
else                           data = name;
f.p[1].writef(line, col, cur ? COL_CURRENT : COL_FIELD, dlen, "%s", data);
if (cur) f.p[1].wmov(line, col);
}

