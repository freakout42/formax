#include "runform.h"

int rMap::init(int page_id) {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,     "maps");
let(prikey,    "id");
let(attrs,     "line,mtext");
letf(t(where), "page_id = %d", page_id);
let(order,     "line");
return 0;
}
