#include "runform.h"

int rField::init(int form_id) {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,     "fields");
let(prikey,    "id");
let(attrs,     "name,dlen,line,col,key");
letf(t(where), "form_id = %d", form_id);
let(order,     "seq");
columni = 5;
return 0;
}
