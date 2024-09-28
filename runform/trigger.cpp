/* triggers are sets of processing commands associated with event points,
 * such as when a particular function key is pressed by the operator
 * or when specific events occur within functions
 * should always return a status int =1 success =0 failure
 * which cause the event to commit or cancel respectively
 */
#include "runform.h"

static char engine[HUGSIZE];
static struct js *javascript = NULL;

/* char *js_getstr(struct js *js, jsval_t value, size_t *len); */
/* jsval_t js_mkstr(struct js *, const void *, size_t); */

/* jquery like access to a "dom"
 * $(.block.field)    => current row fields value
 * $(.block.field, n) => row n fields value
 */
jsval_t j_snub(struct js *js, jsval_t *args, int nargs) {
char *fldvaluep;
char *selector;
selector = js_getstr(js, args[0], NULL);
fldvaluep = *F(l)[F(qfield)(selector)].valuep();
if (fldvaluep) let(a, fldvaluep); else *a = '\0';
return js_mkstr(js, a, strlen(a)+1);
}

/* String() */
jsval_t j_tostring(struct js *js, jsval_t *args, int nargs) {
double number;
number = js_getnum(args[0]);
letf(t(a), "%.0f", number);
return js_mkstr(js, a, strlen(a)+1);
}

#define JSEXT(func) jsval_t j_ ## func (struct js *js, jsval_t *args, int nargs);
JSEXT(snub)
JSEXT(tostring)

/* init the engine and read from config bodys are in map */
int Trigger::init(Qdata *trg, int rix, rMap *map) {
if (!javascript) {
  javascript = js_create(engine, HUGSIZE);
#define JSEXE(func) js_set(javascript, js_glob(javascript), #func, js_mkfun(j_ ## func))
  JSEXE(next_item);
  JSEXE(previous_item);
  JSEXE(next_record);
  JSEXE(previous_record);
  js_set(javascript, js_glob(javascript), "$", js_mkfun(j_snub));
  js_set(javascript, js_glob(javascript), "String", js_mkfun(j_tostring));
  jsexecdirect("let cb; let cf; let cr;");
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

#ifdef EXAMPLE

jsval_t j_snub(struct js *js, jsval_t *args, int nargs) {
char *fldvaluep;
//jsval_t fieldvalue;
char *selector;
selector = js_getstr(js, args[0], NULL);
fldvaluep = *F(l)[F(qfield)(selector)].valuep();
if (fldvaluep) let(a, fldvaluep); else *a = '\0';
//fieldvalue = js_mkstr(js, a, strlen(a)+1);
//let(a, "corrupt");
//fieldvalue = fldvaluep ? js_mkstr(js, fldvaluep, strlen(fldvaluep)+1) : js_mkstr(js, "", 1); // dirty
//return fieldvalue;
return js_mkstr(js, a, strlen(a)+1);
//return args[0];
}

jsval_t myWrite(struct js *js, jsval_t *args, int nargs) {
  digitalWrite(js_getnum(args[0]), js_getnum(args[1]));
  return js_mknum(0);
}

void setup() {
  struct js *js = js_create(buf, sizeof(buf));
  jsval_t global = js_glob(js), gpio = js_mkobj(js);  // Equivalent to:
#if 0
  js_set(js, global, "x", js_mkfun(x));
#endif
  js_set(js, global, "delay", js_mkfun(myDelay));  // Import delay()
  js_set(js, global, "gpio", gpio);                // let gpio = {};
  js_set(js, gpio, "mode", js_mkfun(myMode));      // Import gpio.mode()
  js_set(js, gpio, "write", js_mkfun(myWrite));    // Import gpio.write()

  // Serial.begin(115200);
  // for (;;) Serial.println(js_str(js, js_eval(js, "'abc'", ~0U))),
  // delay(1000);

  js_eval(js,
          "let pin = 13;"       // LED pin. Usually 13, but double-check
          "gpio.mode(pin, 1);"  // Set OUTPUT mode on a LED pin
          "for (;;) {"
          "  delay(300);"
          "  gpio.write(pin, 1);"
          "  delay(300);"
          "  gpio.write(pin, 0);"
          "}",
          ~0U);
}
#endif
