// workhorse object
#include "runform.h"

int Field::init(Qdata *fld, int rix) {
let(name, fld->v(rix, 1));
dlen =    fld->n(rix, 2);
line =    fld->n(rix, 3);
col  =    fld->n(rix, 4);
let(qhuman, "");
return 0;
}

void Field::fshow(int cur) {
f.p[1].writef(line, col, cur ? COL_CURRENT : COL_FIELD, dlen, "%s", f.rmode==MOD_QUERY ? qhuman : name);
if (cur) f.p[1].wmov(line, col);
}

