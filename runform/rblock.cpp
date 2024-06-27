#include <string.h>
#include "runform.h"

int rBlock::init() {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,  "blocks");
let(prikey, "id");
let(where,  "form_id = 1");
let(order,  "seq");
return 0;
}
