/* i embedded elk into my formax project (#define JS_VERSION "3.0.0")
 * get problems when working on modern 64 bit Linux (GitHub codespace)
 */
#include <stdio.h>
#include "elk.c"

int main(void) {
  char mem[100000];
  struct js *js = js_create(mem, sizeof(mem));  // Create JS instance
  jsval_t v = js_eval(js, "let cb;let cf;let cr;let cv;let nav0 = 500;let v0;let v1;let v2;let v3;let clip = '0';", ~0U);
  printf("result: %s\n", js_str(js, v));        // result: undefined
  v = js_eval(js, "cb = 'depts'; cf = 'dname'; cr = 1; cv = 'ACCOUNTING';\nclip = cv;529;\n", ~0U);
  printf("result: %s\n", js_str(js, v));        // result: 529
  return 0;
}

#ifdef nonono

works on old 32-Linux

$ uname -a
Linux jim 3.10.0 #1 SMP Thu Dec 15 20:31:06 MSK 2022 i686 i686 i386 GNU/Linux
cc --version
cc (GCC) 3.4.6
$ cc -g -O0 testelk.c elk/elk.o -o testelk 
$ ./testelk 
result: undefined
result: 529

fails on modern 64-Linux
$ uname -a
Linux codespaces-71fa8d 6.8.0-1014-azure #16~22.04.1-Ubuntu SMP Thu Aug 15 21:31:41 UTC 2024 x86_64 x86_64 x86_64 GNU/Linux
$ cc --version
cc (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0
$ cc -g -O0 testelk.c elk/elk.o -o testelk
$ ./testelk 
result: ERROR: parse error
result: ERROR: parse error

#endif
