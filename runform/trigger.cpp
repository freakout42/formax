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
int Trigger::init(Qdata *trg, int rix) {
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
return fillbody(trg->n(rix, 4));
}

/* read the body from map */
int Trigger::fillbody(int qid) {
let(body,"0;");
/*
  if (p[i].init(rpage.q, i+1)) return 9;
  if (rmap.init(p[i].page_id)) return 9;
  if ((s = rmap.query())) return s;
  if (p[i].maps(rmap.q)) return 9;
  rmap.rclose();
int i, r, y;
char *t, *p;
for (i = 1; i <= qma->rows; i++) {
  r = qma->n(i, 1) - 1;
  if (r > NLINES) return 1;
  map[r] = qma->c(i, 2);
  y = 1;
  p = NULL;
  // white the $nn_ pos markers
  for (t=map[r]; y; t++) {
    if (*t == '$') {
      p = t;
    } else {
      if (p && (!(*t == '_' || *t == '.' || (*t >= '0' && *t <= '9')))) {
        if (t - p > 1) while (p < t) *p++ = ' ';
        p = NULL;
      }
    }
    if (!*t) y = 0;
  }
}
 */
return 0;
}

int Trigger::triggerid() {
return (trgtyp<100 ? 0 : trgfld) * 1000 + trgtyp;
}

int Trigger::jsexec() {
jsval_t v;
//v = js_eval(javascript, "next_item()", ~0);
v = js_eval(javascript, body, ~0);
return atoi(js_str(javascript, v));
}

