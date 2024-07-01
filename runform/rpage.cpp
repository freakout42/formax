#include "runform.h"

int rPage::init() {
int s;
stmt = NULL;
if ((s = ropen())) return s;
let(table,  "pages");
let(prikey, "id");
let(attrs,  "name,ysiz,xsiz,vwpy0,vwpx0,border");
let(where,  "form_id = 1");
let(order,  "seq");
return 0;
}
