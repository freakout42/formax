/* triggers are sets of processing commands associated with event points,
 * such as when a particular function key is pressed by the operator
 * or when specific events occur within functions
 * should always return a status int =1 success =0 failure
 * which cause the event to commit or cancel respectively
 */
#include <stdlib.h>
#include "runform.h"

static char engine[HUGSIZE];
static struct js *javascript = NULL;

/* init the engine and read from config bodys are in map */
#define JSEXE(func) js_set(javascript, js_glob(javascript), #func, js_mkfun(j_ ## func))
int Trigger::init(Qdata *trg, int rix, rMap *map) {
if (!javascript) {
  javascript = js_create(engine, HUGSIZE);
//  js_set(javascript, js_glob(javascript), "next_item", js_mkfun(j_next_item));
  JSEXE(next_item);
  JSEXE(previous_item);
  JSEXE(next_record);
  JSEXE(previous_record);
}
let(name,trg->v(rix, 1));
trgfld = trg->n(rix, 2);
trgtyp = trg->n(rix, 3);
map_id = trg->n(rix, 4);
return map->getbody(map_id, body, sizeof(body));
}

int Trigger::triggerid() {
//(trgtyp<100 ? 0 : trgfld) * 1000 + trgtyp;
return trgtyp;
}

int Trigger::jsexec() {
jsval_t v;
v = js_eval(javascript, body, ~0);
return atoi(js_str(javascript, v));
}

