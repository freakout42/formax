/* trigger configuration */
#include "runform.h"

int rTrigger::init(int form_id) {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,     "triggers");
let(prikey,    "id");
let(attrs,     "trgfld,trgtyp,page_id");
letf(t(where), "form_id = 0 or form_id = %d", form_id);
let(order,     "abs(trgfld)");
columni = 3;
empty(condition);
return 0;
}
