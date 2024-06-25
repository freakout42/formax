#include <string.h>
#include "runform.h"

int Block::init(Qdata blk, int rix) {
let(table,  blk.v(rix, 1));
let(prikey, blk.v(rix, 2));
let(where,  blk.v(rix, 3));
let(order,  blk.v(rix, 4));
return 0;
}
