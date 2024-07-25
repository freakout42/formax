/* triggers are sets of processing commands associated with event points,
 * such as when a particular function key is pressed by the operator
 */
#include <stdlib.h>
#include "runform.h"

static char engine[HUGSIZE];
static struct js *javascript = NULL;

int Trigger::init(Qdata *trg, int rix) {
if (!javascript) javascript = js_create(engine, HUGSIZE);
let(name,trg->v(rix, 1));
trgblk = trg->n(rix, 2);
trgfld = trg->n(rix, 3);
trgtyp = trg->n(rix, 4);
let(body,trg->v(rix, 5));
return 0;
}

int Trigger::jsexec(char *cod) {
jsval_t v;
v = js_eval(javascript, cod, ~0);
return atoi(js_str(javascript, v));
}

