#include "runform.h"

int Function::dispatch() {
switch(f.lastkey) {
 case 0:           run = enter_the_form();                            break;
 case KEY_RIGHT:
 case KEY_LEFT:    fedit();                                           break;
 case KEY_TAB:     fmove(0, 1);                                       break;
 case KEY_BTAB:    fmove(0, -1);                                      break;
 case KEY_F(8):    run = fexit();                                     break;
 case KEY_F(9):    run = fquit();                                     break;
 case KEY_ENTER:   fquery();                                          break;
 default: ;
}
f.p[1].refr();
return run;
}

int Function::enter_the_form() {
f.curblock = 1;
CB.bcurf = 0;
f.curfield = CB.bflds[CB.bcurf];
f.rmode = squerymode ? MOD_QUERY : MOD_INSERT;
return trigger(PRE_FORM);
}

void Function::fmove(int bi, int fi) {
//f.curblock = (f.curblock + f.numblock + bi) % f.numblock;
CB.bcurf = (CB.bcurf + CB.bnumfs + fi) % CB.bnumfs;
f.curfield = CB.bflds[CB.bcurf];
}

int Function::fedit() {
return CF.ledit();
}

int Function::fexit() {
if (!f.dirty) f.p[0].message(40401,"");
return 1;
}

int Function::fquit() {
if (f.dirty) f.p[0].message(40401,"");
return 1;
}

void Function::fquery() {
if (f.b[1].select()) {
  f.p[0].message(40100,f.b[1].sqlcmd);
} else {
  f.rmode = MOD_UPDATE;
}
//return 1;
}

int Function::trigger(int trg) {
return 0;
}

