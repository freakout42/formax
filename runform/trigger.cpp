/* triggers are sets of processing commands associated with event points,
 * such as when a particular function key is pressed by the operator
 * or when specific events occur within functions
 * should always return a status int =1 success =0 failure
 * which cause the event to commit or cancel respectively
 */
#include "runform.h"
#include "elk/elk.h"

static char engine[HUGSIZE];
static struct js *javascript = NULL;

/* char *js_getstr(struct js *js, jsval_t value, size_t *len); */
/* jsval_t js_mkstr(struct js *, const void *, size_t); */

/* jquery like access to a "dom"
 * $(block.field)    => current row fields value
 * $(block.field, n) => row n fields value
 */
static jsval_t j_snub(struct js *js, jsval_t *args, int nargs) {
char *fldvaluep;
char *selector;
double rownumber;
selector = js_getstr(js, args[0], NULL);
if (nargs == 2) {
  rownumber = js_getnum(args[1]);
  fldvaluep = *F(l)[F(qfield)(selector)].valuepr((int)rownumber);
} else {
  fldvaluep = *F(l)[F(qfield)(selector)].valuep();
}
if (fldvaluep) let(a, fldvaluep); else *a = '\0';
return js_mkstr(js, a, strlen(a)+1);
}

/* String() number to string */
static jsval_t j_tostring(struct js *js, jsval_t *args, int nargs) {
double number;
number = js_getnum(args[0]);
letf(t(a), "%.0f", number);
return js_mkstr(js, a, strlen(a)+1);
}

/* Message() */
static jsval_t j_message(struct js *js, jsval_t *args, int nargs) {
char *msgtext;
msgtext = js_getstr(js, args[0], NULL);
return js_mknum(MSG1(MSG_JS, msgtext));
}

/* SQL() returns singlerow singlecol result */
static jsval_t j_sql(struct js *js, jsval_t *args, int nargs) {
char *query;
query = js_getstr(js, args[0], NULL);
dbconn[1].execdirect(query);
letf(t(a), "%s", dbconn[1].q->v(1,1));
return js_mkstr(js, a, strlen(a)+1);
}

#define JSEXA(func) jsval_t j_ ## func (struct js *js, jsval_t *args, int nargs) { return js_mknum(u.func()); }
JSEXA(next_item)
JSEXA(previous_item)
JSEXA(next_record)
JSEXA(previous_record)

/* init the engine and read from config bodys are in map */
int Trigger::init(Qdata *trg, int rix, rMap *map) {
if (!javascript) {
  javascript = js_create(engine, HUGSIZE);
#define JSEXE(jsfn,func) js_set(javascript, js_glob(javascript), #jsfn, js_mkfun(j_ ## func))
  JSEXE(next_item,next_item);
  JSEXE(previous_item,previous_item);
  JSEXE(next_record,next_record);
  JSEXE(previous_record,previous_record);
  JSEXE($,snub);
  JSEXE(String,tostring);
  JSEXE(Message,message);
  JSEXE(SQL,sql);
  letf(t(a), "let cb;let cf;let cr;let nav0 = %d;let v0;let v1;let v2;let v3;", KEF_NAVI0);
  jsexecdirect(a);
}
trgfld = trg->n(rix, 1);
trgtyp = trg->n(rix, 2);
map_id = trg->n(rix, 3);
return map->getbody(map_id, body, sizeof(body));
}

char *Trigger::jsexecdirect(char *prog) {
jsval_t v;
v = js_eval(javascript, prog, ~0);
return (char*)js_str(javascript, v);
}

char *Trigger::jsexec() {
char prog[BIGSIZE];
letf(t(prog), "cb = '%s'; cf = '%s'; cr = %d;\n", CB.table, CF.name, CR);
cats(t(prog), body);
return jsexecdirect(prog);
}

