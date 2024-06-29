#include "runform.h"

int rField::init() {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,  "fields");
let(prikey, "id");
let(where,  "block_id = 1");
let(order,  "seq");
return 0;
}
