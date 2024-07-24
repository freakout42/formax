#include "runform.h"

int rPage::init(int form_id) {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,     "pages");
let(prikey,    "id");
let(attrs,     "id,name,ysiz,xsiz,vwpy0,vwpx0,border");
letf(t(where), "form_id = %d", form_id);
let(order,     "seq");
columni = 7;
return 0;
}
