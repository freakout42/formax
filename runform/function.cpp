#include "runform.h"

int Function::dispatch() {
int s;
switch(f.lastkey) {
 case 0:           s = enter_the_form();                     break;
 case KEF_RIGHT:   s = fedit(0);                             break;
 case KEF_LEFT:    s = fedit(1);                             break;
 case KEF_NXTFLD:  s = fmove(0, 1);                          break;
 case KEF_PREFLD:  s = fmove(0, -1);                         break;
 case KEF_NXTREC:  s = fmover(1);                            break;
 case KEF_PREREC:  s = fmover(-1);                           break;
 case KEF_EXIT:    s = fexit();                              break;
 case KEF_CANCEL:  s = fquit();                              break;
 case KEF_COMMIT:  s = fquery();                             break;
 default: ;
}
f.p[1].refr();
return notrunning;
}

int Function::enter_the_form() {
f.curblock = 1;
CB.bcurf = 0;
f.curfield = CB.bflds[CB.bcurf];
f.rmode = squerymode ? MOD_QUERY : MOD_INSERT;
notrunning = trigger(PRE_FORM);
return notrunning;
}

int Function::fmove(int bi, int fi) {
//f.curblock = (f.curblock + f.numblock + bi) % f.numblock;
CB.bcurf = (CB.bcurf + CB.bnumfs + fi) % CB.bnumfs;
f.curfield = CB.bflds[CB.bcurf];
return f.curfield;
}

int Function::fmover(int ri) {
return CB.bcur++;
}

int Function::fedit(int pos) {
return CF.ledit(pos);
}

int Function::fexit() {
if (!f.dirty) f.p[0].message(401,"");
notrunning = -1;
return 0;
}

int Function::fquit() {
if (f.dirty) f.p[0].message(401,"");
notrunning = -1;
return 0;
}

int Function::fquery() {
if (f.b[1].select()) {
  f.p[0].message(100,f.b[1].sqlcmd);
  notrunning = -2;
} else {
  CB.bcur = 1;
  f.rmode = MOD_UPDATE;
}
return f.rmode != MOD_UPDATE;
}

int Function::trigger(int trg) {
return 0;
}

