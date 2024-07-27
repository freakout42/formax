/* triggers are sets of processing commands associated with event points,
 * such as when a particular function key is pressed by the operator
 */
#include <stdlib.h>
#include "runform.h"

static char engine[HUGSIZE];
static struct js *javascript = NULL;

int Trigger::init(Qdata *trg, int rix) {
if (!javascript) {
  javascript = js_create(engine, HUGSIZE);
  js_set(javascript, js_glob(javascript), "next_item", js_mkfun(j_next_item));
}
let(name,trg->v(rix, 1));
trgblk = trg->n(rix, 2);
trgfld = trg->n(rix, 3);
trgtyp = trg->n(rix, 4);
let(body,trg->v(rix, 5));
return 0;
}

char *Trigger::triggerid() {
static char tid[SMLSIZE];
letf(t(tid), "%d.%d.%s", trgblk, trgfld, name);
return tid;
}

int Trigger::jsexec() {
jsval_t v;
//v = js_eval(javascript, "next_item()", ~0);
v = js_eval(javascript, body, ~0);
return atoi(js_str(javascript, v));
}

