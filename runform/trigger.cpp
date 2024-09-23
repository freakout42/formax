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
return fillbody(trg->n(rix, 4), map);
}
/*
CREATE TABLE triggers
  (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
   form_id INTEGER NOT NULL DEFAULT 1,
   name TEXT NOT NULL DEFAULT '',
   trgfld INTEGER NOT NULL DEFAULT 0,
   trgtyp INTEGER NOT NULL DEFAULT 0,
   page_id INTEGER NOT NULL DEFAULT 0
  );
INSERT INTO triggers VALUES(1,1,'enter_the_form',0,10001,0);
 */

/* read the body from map */
int Trigger::fillbody(int qid, rMap *map) {

int i, s;

//let(body,"next_item()");
if (map->init(qid)) return 9;
if ((s = map->query())) return s;

for (i = 1; i <= map->q->rows; i++) {
  let(body, map->q->c(i, 2));
}
map->rclose();
return 0;
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

