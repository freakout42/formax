/* body should be in a map */
#include "runform.h"

int rTrigger::init(int form_id) {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,     "triggers");
let(prikey,    "id");
let(attrs,     "trgfld,trgtyp,page_id");
letf(t(where), "form_id = %d", form_id);
let(order,     "");
columni = 3;
return 0;
}
