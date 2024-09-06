#include "runform.h"

int rPage::init(int form_id) {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,     "pages");
let(prikey,    "id");
let(attrs,     "id,name,ysiz,xsiz,vwpy0,vwpx0,popup,border");
letf(t(where), "form_id in (0, %d)", form_id);
let(order,     "seq");
columni = 8;
return 0;
}
