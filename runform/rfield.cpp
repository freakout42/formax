#include "runform.h"

int rField::init(int form_id) {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,     "fields");
let(prikey,    "id");
let(attrs,     "name,blcn,pgen,dlen,line,col,key,ftype,len,btab");
letf(t(where), "form_id = %d", form_id);
let(order,     "seq");
columni = 10;
return 0;
}
