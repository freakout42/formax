/* triggers are sets of processing commands associated with event points,
 * such as when a particular function key is pressed by the operator
 * or when specific events occur within functions
 * should always return a status int =1 success =0 failure
 * which cause the event to commit or cancel respectively
 */
#include "runform.h"

static char engine[HUGSIZE];
static struct js *javascript = NULL;

/* char *js_getstr(struct js *js, jsval_t value, size_t *len) */
jsval_t j_snub(struct js *js, jsval_t *args, int nargs) {
char fieldindex[6];
char *selector;
selector = js_getstr(js, args[0], NULL);
letf(t(fieldindex), "%5d", F(qfield)(selector));
/*return js_mknum(js_getnum(args[0]));*/
/*return js_mkstr(js, js_getstr(js, args[0], NULL), SMLSIZE);*/
return js_mkstr(js, fieldindex, sizeof(fieldindex));
}

#define JSEXT(func) jsval_t j_ ## func (struct js *js, jsval_t *args, int nargs);
JSEXT(snub)

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
}
trgfld = trg->n(rix, 1);
trgtyp = trg->n(rix, 2);
map_id = trg->n(rix, 3);
return map->getbody(map_id, body, sizeof(body));
}

char *Trigger::jsexec() {
jsval_t v;
v = js_eval(javascript, body, ~0);
return (char*)js_str(javascript, v);
}

#ifdef EXAMPLE
jsval_t myDelay(struct js *js, jsval_t *args, int nargs) {
  delay(js_getnum(args[0]));
  return js_mknum(0);
}
jsval_t myWrite(struct js *js, jsval_t *args, int nargs) {
  digitalWrite(js_getnum(args[0]), js_getnum(args[1]));
  return js_mknum(0);
}
jsval_t myMode(struct js *js, jsval_t *args, int nargs) {
  pinMode(js_getnum(args[0]), js_getnum(args[1]));
  return js_mknum(0);
}

char buf[300];  // Runtime JS memory
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
