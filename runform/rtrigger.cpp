#include "runform.h"

int rTrigger::init(int form_id) {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,     "triggers");
let(prikey,    "id");
let(attrs,     "name,trgblk,trgfld,trgtyp,body");
letf(t(where), "form_id = %d", form_id);
let(order,     "");
columni = 5;
return 0;
}
