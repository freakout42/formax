/* field configuration */
#include "runform.h"

int rField::init(int form_id) {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,     "fields");
let(prikey,    "id");
let(attrs,     "id,name,blcn,pgen,dlen,align,line,col,key,ftype,len,decl,btab,dflt,query,upd,updnul,mand,lovtit,lov_id,lovi_id,low,high,valpatn,help,id");
letf(t(where), "form_id = %d", form_id);
let(order,     "seq");
columni = 26;
empty(condition);
return 0;
}

