#include <string.h>
#include "version.h"
#include "qdata.h"
#include "record.h"
#include "rmap.h"

int rMap::init(SQLHDBC dbc0, int page_id) {
int s;
stmt = NULL;
if ((s = open(dbc0))) return s;
let(table,  "maps");
let(prikey, "id");
letf(t(where), "page_id = %d", page_id);
let(order,  "line");
return 0;
}
