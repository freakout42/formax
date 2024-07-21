#include "runform.h"

int rBlock::init() {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,  "blocks");
let(prikey, "id");
let(attrs,  "name,norec,prikey,whereand,orderby");
let(where,  "form_id = 1");
let(order,  "seq");
columni = 5;
return 0;
}
