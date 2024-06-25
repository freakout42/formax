#include <string.h>
#include "runform.h"

int rBlock::init(SQLHDBC dbc0) {
int s;
stmt = NULL;
if ((s = open(dbc0))) return s;
let(table,  "blocks");
let(prikey, "id");
let(where,  "form_id = 1");
let(order,  "seq");
return 0;
}
