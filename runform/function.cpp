#include <string.h>
#include "runform.h"

int Function::dispatch() {
int run;
run = 1;
switch(f.lastkey) {
 case 0: f.p[1].refresh(); break;
 case KEY_F(8):
  if (!f.dirty) f.p[0].message(40401);
  run = 0;
  break;
 default: ;
}
return run;
}
