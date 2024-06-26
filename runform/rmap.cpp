#include <string.h>
#include "runform.h"

int rMap::init(int page_id) {
int s;
stmt = NULL;
if ((s = open())) return s;
let(table,  "maps");
let(prikey, "id");
letf(t(where), "page_id = %d", page_id);
let(order,  "line");
return 0;
}
