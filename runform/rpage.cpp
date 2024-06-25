#include <string.h>
#include "version.h"

int rPage::init(SQLHDBC dbc0) {
int s;
stmt = NULL;
if ((s = open(dbc0))) return s;
let(table,  "pages");
let(prikey, "id");
let(where,  "form_id = 1");
let(order,  "num");
return 0;
}
