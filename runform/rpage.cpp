#include <string.h>
#include "runform.h"

int rPage::init() {
int s;
stmt = NULL;
if ((s = open())) return s;
let(table,  "pages");
let(prikey, "id");
let(where,  "form_id = 1");
let(order,  "num");
return 0;
}
