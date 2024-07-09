#include "runform.h"

int rError::init() {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,  "errors");
let(prikey, "id");
let(attrs,  "num,severity,etext");
let(where,  "");
let(order,  "num");
columni = 3;
return 0;
}
