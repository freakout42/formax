#include <string.h>
#include "runform.h"

int Function::dispatch() {
int run;
run = 0;
switch(f.lastkey) {

 // startup event
 case 0: f.p[1].refr(); break;

 // commit and exit
 case KEY_F(8):
  if (!f.dirty) f.p[0].message(40401);
  run = 1;
  break;

 // rollback and exit
 case KEY_F(12):
  if (f.dirty) f.p[0].message(40401);
  run = 1;
  break;

 default: ;
}
return run;
}
