#include "runform.h"

int Function::dispatch() {
int s;
switch(f.lastkey) {
 case 0:           s = enter_the_form();                     break;
 case KEF_RIGHT:   s = fedit(0);                             break;
 case KEF_LEFT:    s = fedit(-1);                            break;
 case KEF_NXTFLD:  s = fmove(0, 1);                          break;
 case KEF_PREFLD:  s = fmove(0, -1);                         break;
 case KEF_NXTREC:  s = fmover(1);                            break;
 case KEF_PREREC:  s = fmover(-1);                           break;
 case KEF_EXIT:    s = fexit();                              break;
 case KEF_CANCEL:  s = fquit();                              break;
 case KEF_COMMIT:  s = fquery();                             break;
 case KEF_DELETE:  s = fdelete();                            break;
 case 'i':         CB.q->splice(CB.currentrecord++); break; //CB.insert(CB.currentrecord); break;
 default: ;
}
f.p[1].refr();
return notrunning;
}

int Function::enter_the_form() {
f.curblock = 1;
f.curfield = CB.blockfields[0];
f.rmode = squerymode ? MOD_QUERY : MOD_INSERT;
notrunning = trigger(PRE_FORM);
return notrunning;
}

int Function::fmove(int bi, int fi) {
//f.curblock = (f.curblock + f.numblock + bi) % f.numblock;
f.curfield = CB.blockfields[ (CF.sequencenum-1 + CB.fieldcount + fi) % CB.fieldcount ];
return f.curfield;
}

int Function::fmover(int ri) {
if (CB.currentrecord > 0) {
  CB.currentrecord += ri;
  if (CB.currentrecord > CB.q->rows) CB.currentrecord = CB.q->rows;
  if (CB.currentrecord < 1) {
    MSG(100);
    CB.currentrecord = 1;
  }
}
return CB.currentrecord;
}

int Function::fedit(int pos) {
return (f.rmode == MOD_QUERY || !CF.isprimarykey) ? CF.edit(pos) : MSG(101);
}

int Function::fexit() {
if (!f.dirty) MSG(401);
notrunning = -1;
return 0;
}

int Function::fquit() {
if (f.dirty) MSG(401);
notrunning = -1;
return 0;
}

int Function::fquery() {
if (f.b[1].select()) notrunning = -2; else {
  if (CB.q->rows > 0) {
    CB.currentrecord = 1;
    f.rmode = MOD_UPDATE;
  } else {
    CB.currentrecord = 0;
    f.rmode = MOD_QUERY;
  }
}
return f.rmode != MOD_UPDATE;
}

int Function::fdelete() {
f.b[1].destroy(CB.currentrecord);
return fquery();
}

int Function::trigger(int trg) {
return 0;
}

