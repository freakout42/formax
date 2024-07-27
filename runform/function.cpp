/* all processing centers around events.
 * Put simply, events are things that occur when a form is exeecuted.
 * formax knows about events and handles them by executing functions.
 * Note that during processing, events are usually nested.
 * That is, the occurence of one event usually invokes
 * functions that invoke other events.
 * Inherent in the functions that events invoke -
 * and that are invoked by your tiggers -
 * are the processes of navigation and validation.
 */
#include "runform.h"

int Function::dispatch() {
F(lastcmd) = F(mapkey)(LK);
switch(F(lastcmd)) {
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
  case KEF_NXTFLD:   LK = next_item();                                 break;
  case KEF_PREFLD:   LK = previous_item();                             break;
  case KEF_NXTREC:   LK = next_record();                               break;
  case KEF_PREREC:   LK = previous_record();                           break;
  case KEF_INSERT:
   switch(F(rmode)) {
    case MOD_UPDATE:
    case MOD_QUERY:  LK = insert_record();                             break;
    default:         LK = 0; F(y).toggle();                            break;
   }                                                                   break;
  case KEF_BACKDEL:         /* fbackdel() */
  case KEF_DELETE:
   switch(F(rmode)) {
    case MOD_QUERY:  LK = fedit(KEF_DEL);                              break;
    case MOD_UPDATE: LK = delete_record();                             break;
    case MOD_INSERT: LK = clear_record();                              break;
    default:         LK = 0;                                           break;
   }                                                                   break;
  case KEF_QUERY:    LK = enter_query();                               break;
  case KEF_NAVI10:
  case KEF_COMMIT:
   switch(F(rmode)) {
    case MOD_QUERY:  LK = execute_query();                             break;
    case MOD_UPDATE: LK = enter_query();                               break;
    case MOD_INSERT: LK = create_record();                             break;
    case MOD_DELETE: LK = destroy_record();                            break;
   }                                                                   break;
  case KEF_EXIT:     LK = fexit();                                     break;
  case KEF_QUIT:
  case KEF_CANCEL:
   switch(F(rmode)) {
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

/* GENERAL */
int Function::enter_the_form() {
F(curblock) = 1;
F(curfield) = CB.blockfields[0];
enter_query();
if (!squerymode) insert_record();
notrunning = trigger("enter_the_form");
return 0;
}

/* NAVIGATION */
int Function::next_item()       { return fmove(0,  1); }
int Function::previous_item()   { return fmove(0, -1); }
int Function::next_record()     { return fmover(1);    }
int Function::previous_record() { return fmover(-1);   }

#define JSEXA(func) jsval_t j_ ## func (struct js *js, jsval_t *args, int nargs) { return js_mknum(u.func()); }
//jsval_t j_next_item(struct js *js, jsval_t *args, int nargs) { return js_mknum(u.next_item()); }
JSEXA(next_item)
JSEXA(previous_item)
JSEXA(next_record)
JSEXA(previous_record)

int Function::fmove(int bi, int fi) {
//F(curblock) = (F(curblock) + F(numblock) + bi) % F(numblock);
if (fi < NFIELD1) F(curfield) = CB.blockfields[ (CF.sequencenum-1 + CB.fieldcount + fi) % CB.fieldcount ];
else              F(curfield) = fi - NFIELD1 - 1;
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

/* EDITING */
int Function::insert_record() {
if (F(rmode) == MOD_UPDATE || F(rmode) == MOD_QUERY) {
  CB.q->splice(CB.currentrecord++);
  F(rmode) = MOD_INSERT;
  DY = 0;
} else {
  MSG(MSG_QUERYM);
}
return 0;
}

int Function::create_record() {
if (CB.insert(CB.currentrecord)) MSG1(MSG_SQL, CB.sqlcmd);
F(rmode) = MOD_UPDATE;
return 0;
}

int Function::fedit(int pos) {
int changed;
changed = 0;
switch(F(rmode)) {
 case MOD_INSERT:
 case MOD_QUERY:
  if (pos == KEF_DEL) CF.clear(); else changed = CF.edit(pos);
  break;
 case MOD_UPDATE:
  changed = CF.edit(pos);
  if (changed != KEF_CANCEL) if (CB.update(CB.currentrecord, CF.sequencenum)) MSG1(MSG_SQL, CB.sqlcmd);
  break;
 case MOD_DELETE:
  break;
}
return changed==KEF_CANCEL ? 0 : changed;
}

int Function::fexit() {
if (!F(dirty)) MSG(MSG_CLEAN);
notrunning = -1;
return 0;
}

int Function::fquit() {
if (F(dirty)) MSG(MSG_DIRTY);
notrunning = -1;
return 0;
}

int Function::enter_query() {
F(b[1]).clear();
CB.currentrecord = 0;
F(rmode) = MOD_QUERY;
return 0;
}

int Function::execute_query() {
if (F(b[1]).select()) MSG1(MSG_SQL, CB.sqlcmd); else {
  if (CB.q->rows > 0) {
    CB.currentrecord = 1;
    F(rmode) = MOD_UPDATE;
  } else {
    return insert_record();
  }
}
return 0;
}

int Function::delete_record() {
F(rmode) = MOD_DELETE;
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
  F(b[1]).destroy(CB.currentrecord);
  clear_record();
  if (CB.currentrecord > CB.q->rows) CB.currentrecord = CB.q->rows;
  ;;
 default: F(rmode) = MOD_UPDATE;
}
return 0;
}

int Function::clear_record() {
CB.q->splice(-CB.currentrecord);
return 0;
}

int Function::trigger(char *trg) {
int i, s;
s = 0;
char trgid[SMLSIZE];
letf(t(trgid), "%d.%d.%s", F(curblock), F(curfield), trg);
for (i=0; i<F(numtrigger); i++)
  if (strcmp(F(r[i]).triggerid(), trgid))
    s = F(r[i]).jsexec();
return s;
}

