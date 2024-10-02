#include <stdio.h>
#include "elk.c"

int main(void) {
  char mem[200];
  struct js *js = js_create(mem, sizeof(mem));  // Create JS instance
  jsval_t v = js_eval(js, "1 + 2 * 3", ~0U);     // Execute JS code
  printf("result: %s\n", js_str(js, v));        // result: 7
  printf("0=%d 0L=%d 0LL=%d\n", sizeof(0), sizeof(0L), sizeof(0LL));
  return 0;
}
