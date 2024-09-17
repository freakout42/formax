/* block configuration */
#include "runform.h"

int rBlock::init(int form_id) {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,     "blocks");
let(prikey,    "id");
let(attrs,     "name,norec,prikey,whereand,orderby");
letf(t(where), "form_id = %d", form_id);
let(order,     "seq");
columni = 5;
return 0;
}
