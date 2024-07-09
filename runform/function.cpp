#include "runform.h"

int Function::dispatch() {
switch(f.mapkey(LK)) {
 case -1:           LK = enter_the_form();                            break;
 case KEF_RIGHT:    LK = fedit(0);                                    break;
 case KEF_LEFT:     LK = fedit(-1);                                   break;
 case KEF_NXTFLD:   LK = fmove(0, 1);                                 break;
 case KEF_PREFLD:   LK = fmove(0, -1);                                break;
 case KEF_NAVI1:    LK = fmove(0, NFIELD1+1);                         break;
 case KEF_NAVI2:    LK = fmove(0, NFIELD1+2);                         break;
 case KEF_NAVI3:    LK = fmove(0, NFIELD1+3);                         break;
 case KEF_NAVI4:    LK = fmove(0, NFIELD1+4);                         break;
 case KEF_NAVI5:    LK = fmove(0, NFIELD1+5);                         break;
 case KEF_NAVI6:    LK = fmove(0, NFIELD1+6);                         break;
 case KEF_NAVI7:    LK = fmove(0, NFIELD1+7);                         break;
 case KEF_NAVI8:    LK = fmove(0, NFIELD1+8);                         break;
 case KEF_NAVI9:    LK = fmove(0, NFIELD1+9);                         break;
 case KEF_NXTREC:   LK = fmover(1);                                   break;
 case KEF_PREREC:   LK = fmover(-1);                                  break;
 case KEF_EXIT:     LK = fexit();                                     break;
 case KEF_CANCEL:   LK = fquit();                                     break;
 case KEF_COMMIT:
  switch(f.rmode) {
   case MOD_UPDATE:
   case MOD_QUERY:  LK = fquery();                                    break;
   case MOD_INSERT: LK = fcreate(); fmquery();                        break;
  }                                                                   break;
 case KEF_QUERY:    LK = fmquery();                                   break;
 case KEF_DELETE:   LK = fdelete(); fmquery();                        break;
 case KEF_INSREC:   LK = finsert();                                   break;
 default:           LK = fedit(-1000 - LK);
}
return notrunning;
}

int Function::enter_the_form() {
f.curblock = 1;
f.curfield = CB.blockfields[0];
f.rmode = squerymode ? MOD_QUERY : MOD_INSERT;
notrunning = trigger(PRE_FORM);
return 0;
}

int Function::fmove(int bi, int fi) {
//f.curblock = (f.curblock + f.numblock + bi) % f.numblock;
if (fi < NFIELD1) f.curfield = CB.blockfields[ (CF.sequencenum-1 + CB.fieldcount + fi) % CB.fieldcount ];
else              f.curfield = fi - NFIELD1 - 1;
return 0;
}

int Function::fmover(int ri) {
if (CB.currentrecord > 0) {
  CB.currentrecord += ri;
  if (CB.currentrecord > CB.q->rows) {
    MSG(103);
    CB.currentrecord = CB.q->rows;
  }
  if (CB.currentrecord < 1) {
    MSG(100);
    CB.currentrecord = 1;
  }
}
return 0;
}

int Function::finsert() {
int s;
s = CB.q->splice(CB.currentrecord++);
f.rmode = MOD_INSERT;
return 0;
}

int Function::fcreate() {
CB.insert(CB.currentrecord);
return 0;
}

int Function::fedit(int pos) {
int changed;
changed = 0;
switch(f.rmode) {
 case MOD_INSERT:
 case MOD_QUERY:
  changed = CF.edit(pos);
  break;
 case MOD_UPDATE:
  if (CF.isprimarykey) MSG(101); else changed = CF.edit(pos);
  if (changed != KEF_CANCEL) CB.update(CB.currentrecord, CF.sequencenum);
  break;
}
return changed==KEF_CANCEL ? 0 : changed;
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

int Function::fmquery() {
f.b[1].clear();
CB.currentrecord = 0;
f.rmode = MOD_QUERY;
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
return 0;
}

int Function::fdelete() {
f.b[1].destroy(CB.currentrecord);
fquery();
return 0;
}

int Function::trigger(int trg) {
return 0;
}

