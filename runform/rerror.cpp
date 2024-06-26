#include <string.h>
#include "runform.h"

int rError::init() {
int s;
stmt = NULL;
if ((s = open())) return s;
let(table,  "errors");
let(prikey, "id");
let(where,  "");
let(order,  "num");
return 0;
}
