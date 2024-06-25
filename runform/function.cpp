#include <string.h>
#include "runform.h"

int Function::dispatch() {
int run;
run = 1;
switch(f.lk) {
 case 0: f.p[1].refresh(); break;
 case KEY_F(8): run = 0; break;
 default: ;
}
return run;
}
