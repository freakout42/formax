#include "runform.h"
#include "colquery/colquery.h"

int Function::dispatch() {
int run;
run = 0;
switch(f.lastkey) {
 case 0:           fstartup();                                        break;
 case KEY_RIGHT:
 case KEY_LEFT:    fedit();                                           break;
 case KEY_TAB:     fmove(0, 1);                                       break;
 case KEY_BTAB:    fmove(0, -1);                                      break;
 case KEY_F(8):    run = fexit();                                     break;
 case KEY_F(9):    run = fquit();                                     break;
 case KEY_ENTER:   f.b[1].query(f.b[1].battrs); f.rmode = MOD_UPDATE; break;
 default: ;
}
f.p[1].refr();
return run;
}

void Function::fmove(int bi, int fi) {
// totally broken with multi-block
f.curblock = (f.curblock + f.numblock + bi) % f.numblock;
f.curfield = (f.curfield + f.numfield + fi) % f.numfield;
}

void Function::fstartup() {
  f.rmode = MOD_QUERY;
}

void Function::fedit() {
int s;
char *toedit;
toedit = f.rmode==MOD_QUERY ? CF.qhuman : CF.name;
f.p[0].sedit(toedit);
s = colquery(CF.qhuman, CF.qwhere, CF.name, 1, 0);
}

int Function::fexit() {
if (!f.dirty) f.p[0].message(40401,"");
return 1;
}

int Function::fquit() {
if (f.dirty) f.p[0].message(40401,"");
return 1;
}

