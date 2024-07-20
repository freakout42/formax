#include "runform.h"

int Function::dispatch() {
f.lastcmd = f.mapkey(LK);
switch(f.lastcmd) {
  case -1:           LK = enter_the_form();                            break;
#ifdef NOTYETIMPLEMENTED
  case KEF_HELP:            /* fhelp() */
  case KEF_COPY:            /* fhelp() */
  case KEF_PASTE:           /* fpaste() */
  case KEF_LIST:            /* flist() */
  case KEF_COPYREC:         /* fcopyrec() */
  case KEF_HOME:            /* fhome() */
  case KEF_END:             /* fend() */
  case KEF_PRESETR:         /* fpresetr() */
  case KEF_NXTSETR:         /* fnxtsetr() */
  case KEF_REFRESH:         /* frefresh() */
  case KEF_NAVI0:           /* fmenu() */
#endif
  case KEF_NAVI1:    LK = fmove(0, NFIELD1+1);                         break;
  case KEF_NAVI2:    LK = fmove(0, NFIELD1+2);                         break;
  case KEF_NAVI3:    LK = fmove(0, NFIELD1+3);                         break;
  case KEF_NAVI4:    LK = fmove(0, NFIELD1+4);                         break;
  case KEF_NAVI5:    LK = fmove(0, NFIELD1+5);                         break;
  case KEF_NAVI6:    LK = fmove(0, NFIELD1+6);                         break;
  case KEF_NAVI7:    LK = fmove(0, NFIELD1+7);                         break;
  case KEF_NAVI8:    LK = fmove(0, NFIELD1+8);                         break;
  case KEF_NAVI9:    LK = fmove(0, NFIELD1+9);                         break;
  case KEF_NXTFLD:   LK = fmove(0, 1);                                 break;
  case KEF_PREFLD:   LK = fmove(0, -1);                                break;
  case KEF_NXTREC:   LK = fmover(1);                                   break;
  case KEF_PREREC:   LK = fmover(-1);                                  break;
  case KEF_INSERT:
   switch(f.rmode) {
    case MOD_UPDATE:
    case MOD_QUERY:  LK = insert_record();                             break;
    default:         LK = 0; f.y.toggle();                             break;
   }                                                                   break;
  case KEF_BACKDEL:         /* fbackdel() */
  case KEF_DELETE:
   switch(f.rmode) {
    case MOD_QUERY:  LK = fedit(KEF_DEL);                              break;
    case MOD_UPDATE: LK = delete_record();                             break;
    case MOD_INSERT: LK = clear_record();                              break;
    default:         LK = 0;                                           break;
   }                                                                   break;
  case KEF_QUERY:    LK = enter_query();                               break;
  case KEF_NAVI10:
  case KEF_COMMIT:
   switch(f.rmode) {
    case MOD_QUERY:  LK = execute_query();                             break;
    case MOD_UPDATE: LK = enter_query();                               break;
    case MOD_INSERT: LK = create_record();                             break;
    case MOD_DELETE: LK = destroy_record();                            break;
   }                                                                   break;
  case KEF_EXIT:     LK = fexit();                                     break;
  case KEF_QUIT:
  case KEF_CANCEL:
   switch(f.rmode) {
    case MOD_UPDATE: 
    case MOD_QUERY:  LK = fquit();                                     break;
    default:         enter_query(); LK = 0;                            break;
   }                                                                   break;
  case KEF_RIGHT:    LK = fedit(0);                                    break;
  case KEF_LEFT:     LK = fedit(-1);                                   break;
  default:
   if (isprintable(LK))
                     LK = fedit(-1000 - LK);
   else              LK = 0;
                                                                       break;
}
return notrunning;
}

int Function::enter_the_form() {
f.curblock = 1;
f.curfield = CB.blockfields[0];
enter_query();
if (!squerymode) insert_record();
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
    MSG(MSG_LAST);
    CB.currentrecord = CB.q->rows;
  }
  if (CB.currentrecord < 1) {
    MSG(MSG_FIRST);
    CB.currentrecord = 1;
  }
}
return 0;
}

int Function::insert_record() {
int s;
if ((f.rmode == MOD_UPDATE || f.rmode == MOD_QUERY) && !(s = CB.q->splice(CB.currentrecord++))) {
  f.rmode = MOD_INSERT;
  DY = 0;
} else {
  MSG(MSG_QUERYM);
}
return 0;
}

int Function::create_record() {
if (CB.insert(CB.currentrecord)) f.p[0].message(MSG_SQL, CB.sqlcmd);
f.rmode = MOD_UPDATE;
return 0;
}

int Function::fedit(int pos) {
int changed;
changed = 0;
switch(f.rmode) {
 case MOD_INSERT:
 case MOD_QUERY:
  if (pos == KEF_DEL) CF.clear(); else changed = CF.edit(pos);
  break;
 case MOD_UPDATE:
  if (CF.isprimarykey) MSG(MSG_EDITKEY); else changed = CF.edit(pos);
  if (changed != KEF_CANCEL) if (CB.update(CB.currentrecord, CF.sequencenum)) f.p[0].message(MSG_SQL, CB.sqlcmd);
  break;
 case MOD_DELETE:
  break;
}
return changed==KEF_CANCEL ? 0 : changed;
}

int Function::fexit() {
if (!f.dirty) MSG(MSG_CLEAN);
notrunning = -1;
return 0;
}

int Function::fquit() {
if (f.dirty) MSG(MSG_DIRTY);
notrunning = -1;
return 0;
}

int Function::enter_query() {
f.b[1].clear();
CB.currentrecord = 0;
f.rmode = MOD_QUERY;
return 0;
}

int Function::execute_query() {
if (f.b[1].select()) f.p[0].message(MSG_SQL, CB.sqlcmd); else {
  if (CB.q->rows > 0) {
    CB.currentrecord = 1;
    f.rmode = MOD_UPDATE;
  } else {
    return insert_record();
  }
}
return 0;
}

int Function::delete_record() {
f.rmode = MOD_DELETE;
return 0;
}

int Function::destroy_record() {
int s;
s = KEY_ENTER;
if (deleprompt) s = MSG(MSG_DELASK);
switch(s) {
 case KEY_ENTER:
 case 'y':
 case 'j':
 case 'o':
 case 'Y':
 case 'J':
 case 'O':
  f.b[1].destroy(CB.currentrecord);
  clear_record();
  if (CB.currentrecord > CB.q->rows) CB.currentrecord = CB.q->rows;
  ;;
 default: f.rmode = MOD_UPDATE;
}
return 0;
}

int Function::clear_record() {
CB.q->splice(-CB.currentrecord);
return 0;
}

int Function::trigger(int trg) {
return 0;
}

