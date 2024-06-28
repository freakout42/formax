// workhorse object
#include <string.h>
#include "runform.h"

int Field::init(Qdata *fld, int rix) {
let(name, fld->v(1, 1));
dlen =    fld->n(rix, 2);
line =    fld->n(rix, 3);
col  =    fld->n(rix, 4);
return 0;
}
