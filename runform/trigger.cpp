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

/* examples */
/* char *js_getstr(struct js *js, jsval_t value, size_t *len); */
/* jsval_t js_mkstr(struct js *, const void *, size_t); */

/* jquery like access to a "dom"
 * $("block.field")    => current row fields value
 * $("block.field", n) => row n fields value
 */
static jsval_t j_snub(struct js *js, jsval_t *args, int nargs) {
int fldn;
char *fldvaluep;
char *selector;
double rownumber;
//let(a, "(null)");
empty(a);
selector = js_getstr(js, args[0], NULL);
fldn = F(qfield)(selector);
if (fldn >= 0) {
if (nargs == 2) {
  rownumber = js_getnum(args[1]);
  fldvaluep = *fldi(fldn).valuep((int)rownumber);
} else {
  fldvaluep = *fldi(fldn).valuep();
}
if (fldvaluep) let(a, fldvaluep);
}
return js_mkstr(js, a, strlen(a)+1);
}

/* set query conditions for field
 * setquery("block.field", "condition")
 */
static jsval_t j_setquery(struct js *js, jsval_t *args, int nargs) {
int fldn;
char *selector;
char *condition;
if (nargs == 2) {
  selector  = js_getstr(js, args[0], NULL);
  condition = js_getstr(js, args[1], NULL);
  fldn = F(qfield)(selector);
  if (fldn >= 0) fldi(fldn).setcond(condition);
}
return js_mknum(0);
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
JSEXA(next_block)
JSEXA(previous_block)
JSEXA(next_record)
JSEXA(previous_record)
JSEXA(exec_query)

/* init the engine and read from config bodys are in map */
#define JSEXE(jsfn,func) js_set(javascript, js_glob(javascript), #jsfn, js_mkfun(j_ ## func))
int Trigger::init(Qdata *trg, int rix, rMap *map) {
int i;
if (!javascript) {
  javascript = js_create(engine, HUGSIZE);
  JSEXE(next_item,next_item);
  JSEXE(previous_item,previous_item);
  JSEXE(next_block,next_block);
  JSEXE(previous_block,previous_block);
  JSEXE(next_record,next_record);
  JSEXE(previous_record,previous_record);
  JSEXE(exec_query,exec_query);
  JSEXE($,snub);
  JSEXE(setquery,setquery);
  JSEXE(String,tostring);
  JSEXE(Message,message);
  JSEXE(SQL,sql);
  letf(t(a), "let cb;let cf;let ci;let cr;let cv;let nav0 = %d;let v0;let v1;let v2;let v3;let clip = '0';", KEF_NAVI0);
  jsexecdirect(a, strlen(a));
}
trgfld = trg->n(rix, 1);
trgtyp = trg->n(rix, 2);
map_id = trg->n(rix, 3);
index = rix - 1;
fieldindex = -1;
forall(field) if (fldi(i).field_id == trgfld) {
  fieldindex = i;
  if (trgtyp == TRT_POSTCHANGE) fldi(i).trg_postchange = index;
}
i = map->getbody(map_id, t(a));
body = strdup(a);
return i;
}
void Trigger::rclose() { free(body); }

/* exec pure javascript */
char *Trigger::jsexecdirect(char *prg, int siz) {
jsval_t v;
v = js_eval(javascript, prg, siz);
return (char*)js_str(javascript, v);
}

/* exec javascript from trigger */
char *Trigger::jsexec() {
char prog[BIGSIZE];
char *fvalue;
char *escaped;
int progsize;
escaped = a;
if (CV) for (fvalue=CV; *fvalue; fvalue++) {
  if (*fvalue == '\'') *escaped++ = '\\';
  *escaped++ = *fvalue;
}
empty(escaped++);
letf(t(prog), "cb = '%s'; cf = '%s'; ci = %d; cr = %d; cv = '%s';\n", CB.table, CF.column, CF.index, CR, a);
progsize = cats(t(prog), body);
return jsexecdirect(prog, progsize);
}

