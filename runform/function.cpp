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

int Function::dispatch() { /* returns notrunning 0..goon -1..quit <-1..error >0..form_id */
F(lastcmd) = F(mapkey)(LK);
switch(F(lastcmd)) {
#ifdef NOTYETIMPLEMENTED
  case KEF_COPY:            /* fcopy() */
  case KEF_PASTE:           /* fpaste() */
  case KEF_COPYREC:         /* fcopyrec() */
  case KEF_LIST:            /* flist() */
  case KEF_HOME:            /* fhome() */
  case KEF_END:             /* fend() */
  case KEF_PRESETR:         /* fpresetr() */
  case KEF_NXTSETR:         /* fnxtsetr() */
  case KEF_NAVI0:           /* fmenu() */
#endif
  case -1:           LK = enter_the_form();                                   break;
  case KEF_NOOP:
  case KEF_NOOP2:    LK = 0;                                                  break;
  case KEF_REFRESH:  LK = refresh_screen();                                   break;
  case KEF_NAVI1:    LK = fmove(0, NFIELD1+1);                                break;
  case KEF_NAVI2:    LK = fmove(0, NFIELD1+2);                                break;
  case KEF_NAVI3:    LK = fmove(0, NFIELD1+3);                                break;
  case KEF_NAVI4:    LK = fmove(0, NFIELD1+4);                                break;
  case KEF_NAVI5:    LK = fmove(0, NFIELD1+5);                                break;
  case KEF_NAVI6:    LK = fmove(0, NFIELD1+6);                                break;
  case KEF_NAVI7:    LK = fmove(0, NFIELD1+7);                                break;
  case KEF_NAVI8:    LK = fmove(0, NFIELD1+8);                                break;
  case KEF_NAVI9:    LK = fmove(0, NFIELD1+9);                                break;
  case KEF_NXTFLD:   LK = next_item();                                        break;
  case KEF_PREFLD:   LK = previous_item();                                    break;
  case KEF_NXTREC:   LK = next_record();                                      break;
  case KEF_HELP:     LK = help_item();                                        break;
  case KEF_KEYHELP:  LK = keys_help();                                        break;
  case KEF_PREREC:   LK = previous_record();                                  break;
  case KEF_INSERT:
   switch(CM) {
    case MOD_UPDATE:
    case MOD_QUERY:  LK = insert_record();                             break;
    case MOD_INSERT: LK = F(dirty) ? create_record() : clear_record(); break;
    default:         LK = 0; y.toggle();                               break;
   }                                                                          break;
  case KEF_BACKDEL:         /* fbackdel() */
  case KEF_DELETE:
   switch(CM) {
    case MOD_QUERY:  LK = fedit(KEF_DEL);                              break;
    case MOD_UPDATE: LK = delete_record();                             break;
    case MOD_INSERT: LK = clear_record();                              break;
    case MOD_DELETE: LK = destroy_record();                            break;
    default:         LK = 0;                                           break;
   }                                                                          break;
  case KEF_QUERY:    LK = enter_query();                                      break;
  case KEF_NAVI10:
  case KEF_COMMIT:
   switch(CM) {
    case MOD_QUERY:  LK = execute_query();                             break;
    case MOD_UPDATE: LK = enter_query();                               break;
    case MOD_INSERT: LK = F(dirty) ? create_record() : clear_record(); break;
    case MOD_DELETE: LK = destroy_record();                            break;
   }                                                                          break;
  case KEF_EXIT:     LK = fexit();                                            break;
  case KEF_QUIT:
  case KEF_CANCEL:
   switch(CM) {
    case MOD_UPDATE:
    case MOD_QUERY:  LK = fquit();                                     break;
    case MOD_INSERT: LK = clear_record();                              break;
    case MOD_DELETE: switch_mode(MOD_UPDATE); LK = 0;                  break;
   }                                                                          break;
  case KEF_RIGHT:    LK = fedit(0);                                           break;
  case KEF_LEFT:     LK = fedit(-1);                                          break;
  case KEF_NAVI11:   LK = fedit(FED_FEDITOR);                                 break;
  case ' ':          LK = fedit(FED_TRIGGER);                                 break;
  case '=':          LK = editrigger(TRT_EDITFIELD);                          break;
  case '~':          LK = ftoggle();                                          break;
  case '+':          LK = fincrement(1);                                      break;
  case '-':          LK = fincrement(-1);                                     break;
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
F(curblock) = 4;
F(curfield) = CB.blockfields[0];
enter_query();
if (updatemode) execute_query(); else if (!squerymode) insert_record();
notrunning = triggern(TRT_ENTERFORM) != KEF_CANCEL;
return 0;
}

int Function::refresh_screen() {
F(needredraw) = 1;
return 0;
}

int Function::help_item() {
MSG1(MSG_HELP,CF.helptext);
return 0;
}

int Function::keys_help() {
return F(p[PGE_KEYHELP]).showpopup();
}

int Function::edit_map() {
return F(p[PGE_EDITOR]).editmap(2);
}

/* NAVIGATION */
int Function::switch_mode(fmode mod) {
CM = mod;
if (CF.noedit()) fmove(0, 0);
return 0;
}

#define MOVER(func,trgr,move,row,fld) int Function::func() {int i; if ((i = triggern(TRT_ ## trgr)) == KEF_CANCEL) return move(row, fld); return i;}
MOVER(next_item,NEXTITEM,fmove,0,1)
MOVER(previous_item,PREVITEM,fmove,0,-1)
MOVER(next_record,NEXTRECORD,fmover,0,1)
MOVER(previous_record,PREVRECORD,fmover,0,-1)

/* move from field to field */
int Function::fmove(int bi, int fi) {
//F(curblock) = (F(curblock) + F(numblock) + bi) % F(numblock);
if (fi < NFIELD1) F(curfield) = CB.blockfields[ (CF.sequencenum-1 + CB.fieldcount + fi) % CB.fieldcount ];
else              F(curfield) = fi - NFIELD1 - 1;
if (CF.noedit()) fmove(0, fi<0 ? -1 : 1);
return 0;
}

/* move from record to record */
int Function::fmover(int bi, int ri) {
switch (CM) {
 case MOD_QUERY:  return 0;                                                         break;
 case MOD_UPDATE:                                                                   break;
 case MOD_INSERT: if (!yesno(MSG(MSG_DIRTY))) create_record(); else clear_record(); break;
 case MOD_DELETE: if (!yesno(MSG(MSG_DIRTY))) destroy_record();                     break;
}
switch_mode(MOD_UPDATE);
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
if (CM == MOD_UPDATE || CM == MOD_QUERY) {
  CB.q->splice(CB.currentrecord++);
  switch_mode(MOD_INSERT);
} else {
  MSG(MSG_QUERYM);
}
return 0;
}

int Function::create_record() {
if (CB.insert(CB.currentrecord)) MSG1(MSG_SQL, CB.sqlcmd);
switch_mode(MOD_UPDATE);
return 0;
}

int Function::ftoggle() {
changed = CF.toggle();
if (changed == KEF_CANCEL) return 0;
if (CB.update(CB.currentrecord, CF.sequencenum)) MSG1(MSG_SQL, CB.sqlcmd);
return changed;
}

int Function::fincrement(int ival) {
changed = CF.increment(ival);
if (changed == KEF_CANCEL) return 0;
if (CB.update(CB.currentrecord, CF.sequencenum)) MSG1(MSG_SQL, CB.sqlcmd);
return changed;
}

int Function::fedit(int pos) {
changed = 0;
switch(CM) {
 case MOD_INSERT:
 case MOD_QUERY:
  if (pos == KEF_DEL) CF.clear(); else changed = CF.edit(pos<FED_SPECIAL ? -1 : pos);
  break;
 case MOD_UPDATE:
  changed = CF.edit(pos);
  if (changed != KEF_CANCEL) if (CB.update(CB.currentrecord, CF.sequencenum)) MSG1(MSG_SQL, CB.sqlcmd);
  break;
 case MOD_DELETE:
  break;
}
if (CM != MOD_QUERY && changed) F(dirty) = 1;
return changed==KEF_CANCEL ? 0 : changed;
}

int Function::fexit() {
   switch(CM) {
    case MOD_QUERY:
    case MOD_UPDATE: /*MSG(MSG_CLEAN);*/                               break;
    case MOD_INSERT: if (F(dirty)) create_record();                    break;
    case MOD_DELETE: LK = destroy_record();                            break;
   }
notrunning = -1;
return 0;
}

int Function::fquit() {
   switch(CM) {
    case MOD_QUERY:
    case MOD_UPDATE:                                                   break;
    case MOD_INSERT:
    case MOD_DELETE: MSG(MSG_DIRTY);                                   break;
   }
notrunning = -1;
return 0;
}

int Function::enter_query() {
CB.clear();
CB.currentrecord = 0;
F(dirty) = 0;
switch_mode(MOD_QUERY);
return 0;
}

int Function::execute_query() {
if (CB.select()) MSG1(MSG_SQL, CB.sqlcmd); else {
  if (CB.q->rows > 0) {
    CB.currentrecord = 1;
    switch_mode(MOD_UPDATE);
  } else {
    return insert_record();
  }
}
return 0;
}

int Function::delete_record() {
switch_mode(MOD_DELETE);
return 0;
}

int Function::destroy_record() {
int s;
s = KEY_ENTER;
if (deleprompt) s = MSG(MSG_DELASK);
if (yesno(s)) {
  CB.destroy(CB.currentrecord);
  clear_record();
} else switch_mode(MOD_UPDATE);
return 0;
}

int Function::clear_record() {
CB.q->splice(-CB.currentrecord);
if (CB.currentrecord > CB.q->rows) CB.currentrecord = CB.q->rows;
if (CB.q->rows) switch_mode(MOD_UPDATE); else enter_query(); 
return 0;
}

/* TRIGGER */
int Function::editrigger(int tid) {
int i;
int j;
j = 0;
if ((i = qtrigger(tid)) > -1) j = F(p)[PGE_EDITOR].editbuf(F(r)[i].body);
return j;
}

/* export functions to javascript */
#define JSEXA(func) jsval_t j_ ## func (struct js *js, jsval_t *args, int nargs) { return js_mknum(u.func()); }
JSEXA(next_item)
JSEXA(previous_item)
JSEXA(next_record)
JSEXA(previous_record)

int Function::qtrigger(int tid) {
int i;
for (i=0; i<F(numtrigger); i++)
  if ((F(r)[i].trgfld == 0 || F(r)[i].trgfld == CF.field_id) && F(r)[i].trgtyp == tid)
    return i;
return -1;
}

/* raw trigger call NULL..notfound "..string [0-9]..number [^"0-9]..error
 * javascript should always return number see below
 */
char *Function::trigger(int tid) {
static int injstrigger = 0;
int i;
char *s;
s = NULL;
if (injstrigger) return s;
if ((i = qtrigger(tid)) > -1) {
  injstrigger = 1;
    s = F(r)[i].jsexec();
    if (*s != '"' && !isdigit(*s)) {
      g.logfmt("[%d]%s", tid, s);
      MSG1(MSG_JS, s);
      s = "-1";
    }
  injstrigger = 0;
}
return s;
}

int Function::triggern(int tid) {
int i;
char *jsresult;
jsresult = trigger(tid);
i = jsresult ? atoi(jsresult) : 0;
switch(i) {
 case -1: notrunning = -1; return KEF_CANCEL; /* js error quit */
 case 0:                   return KEF_CANCEL; /* cancel no action or no trigger */
 case 1:                   return KEF_NOOP;   /* ok without new key */
 case 2:                   return KEF_NOOP2;  /* no without new key */
 case 3:                   return KEF_NXTFLD; /* ok with next item */
 default:                  return i;          /* ok with key */
}
}

/* edit field with trigger */
int Function::edittrg(char *buf) {
int i;
char *jsresult; /* BIGSIZE never overflows with elk's output */
jsresult = trigger(TRT_EDITFIELD);
if (!jsresult) { ;             /* check for trigger */
} else if (*jsresult == '"') { /* check for output is js string */
  strcpy(buf, jsresult+1);
  i = strlen(buf) - 1;
  assert(*(buf+i) == '\"');
  *(buf+i) = '\0';
  return KEF_NXTFLD;
} else if (isdigit(*jsresult)) { /* check for output is js int */
  strcpy(buf, jsresult);
  return KEF_NXTFLD;
}
return KEF_CANCEL;
}

