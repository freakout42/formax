/* block configuration */
#include "runform.h"

int rBlock::init(int form_id) {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,     "blocks");
let(prikey,    "id");
let(attrs,     "id,name,seq,norec,prikey,whereand,orderby");
letf(t(where), "form_id in (0, %d)", form_id);
let(order,     "seq");
columni = 7;
empty(condition);
return 0;
}

