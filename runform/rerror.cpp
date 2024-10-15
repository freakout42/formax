/* error messages table errors belongs to form zero
 * all forms use the same data
 * to use different language translate rerror.h
 */
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
empty(condition);
return 0;
}

