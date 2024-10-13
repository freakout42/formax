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
  case KEF_LIST:            /* flist() */
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
  case KEF_NXTSETR:  LK = next_setrecords();                                  break;
  case KEF_PRESETR:  LK = previous_setrecords();                              break;
  case KEF_HELP:     LK = help_item();                                        break;
  case KEF_KEYHELP:  LK = keys_help();                                        break;
  case KEF_PREREC:   LK = previous_record();                                  break;
  case KEF_COPYREC:  LK = fcopyrec();                                         break;
  case KEF_HOME:     LK = fmove(-1, 0);                                       break;
  case KEF_END:      LK = fmove( 1, 0);                                       break;
  case KEF_COPY:     LK = fcopy();                                            break;
  case KEF_PASTE:    LK = fpaste();                                           break;
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
  case KEF_QUERY:    LK = enter_query(&CB);                                   break;
  case KEF_NAVI10:
  case KEF_COMMIT:
   switch(CM) {
    case MOD_QUERY:  LK = execute_query();                             break;
    case MOD_UPDATE: LK = enter_query(&CB);                            break;
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
  case KEF_NAVI0:    letf(t(a), "formax v%s charset %s - https://formax.freakout.de/", VERSION, CHARSET);
                     LK = MSG1(MSG_HELP,a);                                   break;
  case '~':          LK = editrigger(TRT_EDITFIELD);                          break;
  case '[':          LK = edit_map();                                         break;
  case ' ':          LK = ftoggle();                                          break;
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
int i;
F(curblock) = 4;
F(curfield) = CB.blockfields[0];
forall(block) if (i >= 4) enter_query(&F(b)[i]);
if (updatemode) execute_query(); else if (!squerymode) insert_record();
notrunning = triggern(TRT_ENTERFORM);
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
return F(p)[PGE_KEYHELP].showpopup();
}

int Function::edit_map() {
return CV ? F(p)[PGE_EDITOR].editmap(atoi(CV)) : 0;
}

/* NAVIGATION */
int Function::switch_mode(fmode mod) {
CM = mod;
if (CF.noedit()) fmove(0, 0);
return 0;
}

#define TRIGGRD(func,trgr,move,row,fld) int Function::func() {int i; return (i = triggern(TRT_ ## trgr)) ? i : move(row, fld); }
TRIGGRD(next_item,NEXTITEM,fmove,0,1)
TRIGGRD(previous_item,PREVITEM,fmove,0,-1)
TRIGGRD(next_record,NEXTRECORD,fmover,0,1)
TRIGGRD(previous_record,PREVRECORD,fmover,0,-1)
TRIGGRD(next_setrecords,NEXTSETREC,fmover,0,CB.norec)
TRIGGRD(previous_setrecords,PREVSETREC,fmover,0,-CB.norec)

/* move from field to field */
int Function::fmove(int bi, int fi) {
if (bi) {
  F(curblock) = ((F(curblock)-4 + F(numblock)-4 + bi) % (F(numblock)-4) + 4);
  F(curfield) = CB.blockfields[0];
}
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

/* double pressed should copy record */
int Function::fcopyrec() {
if (CM != MOD_UPDATE) return 0;
if (CR > 1) {
  edittrgtyp = TRT_COPYREC;
  changed = fedit(FED_TRIGGER);
  return changed==KEF_CANCEL ? 0 : changed;
} else {
  MSG(MSG_NOREC2);
  return 0;
}
}

int Function::fcopy() {int i; return CV && (i = triggern(TRT_COPY)) ? i : 0; }

int Function::fpaste() {
if (CM != MOD_UPDATE) return 0;
  edittrgtyp = TRT_PASTE;
  changed = fedit(FED_TRIGGER);
  return changed==KEF_CANCEL ? 0 : changed;
}

int Function::ftoggle() {
int editmode;
edittrgtyp = TRT_EDITFIELD;
editmode = qtrigger(edittrgtyp) > -1 ? FED_TRIGGER : FED_TOGGLE;
changed = fedit(editmode);
return changed==KEF_CANCEL ? 0 : changed;
}

int Function::fincrement(int ival) {
int editmode;
editmode = ival > 0 ? FED_INCR : FED_DECR;
changed = fedit(editmode);
return changed==KEF_CANCEL ? 0 : changed;
}

/* change the field with various methods determined by pos */
int Function::fedit(int pos) {
changed = 0;
switch(CM) {
 case MOD_INSERT:
 case MOD_QUERY:
  if (pos == KEF_DEL) CF.clear(); else changed = CF.edit(pos<FED_SPECIAL ? -1 : pos);
  break;
 case MOD_UPDATE:
  changed = CF.edit(pos);
  if (changed != KEF_CANCEL)
    if (CF.basetable)
      if (CB.update(CB.currentrecord, CF.sequencenum))
        MSG1(MSG_SQL, CB.sqlcmd);
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

int Function::enter_query(Block *blk) {
blk->clear();
blk->currentrecord = 0;
if (blk == &CB) {
  switch_mode(MOD_QUERY);
  F(dirty) = 0;
} else blk->rmode = MOD_QUERY;
return 0;
}

/* get record data into the block based on the query columns entered */
int Function::execute_query() {
int i, j, k;
int cf;
int triggerdfields[NFIELD1];
int tfn;
if (CB.select()) MSG1(MSG_SQL, CB.sqlcmd); else {
  if (CB.q->rows > 0) {
    edittrgtyp = TRT_POSTQUERY;
    tfn = 0;
    forall(trigger)
      if (trgi(i).trgtyp == edittrgtyp)
        if ((k = trgi(i).fieldindex) >= 0)
          if (fldi(k).blockindex == F(curblock))
            triggerdfields[tfn++] = trgi(i).fieldindex;
    if (tfn > 0) {
      CM = MOD_UPDATE;
      cf = F(curfield);
      for (j=0; j<CB.q->rows; j++) {
        for (k=0; k<tfn; k++) {
          F(curfield) = triggerdfields[k];
          CB.currentrecord = j + 1;
          fedit(FED_TRIGGER);
        }
      }
      F(curfield) = cf;
    }
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
if (CB.q->rows) switch_mode(MOD_UPDATE); else enter_query(&CB); 
return 0;
}

/* TRIGGER */
int Function::editrigger(int tid) {
int i;
if ((i = qtrigger(tid)) > -1) F(p)[PGE_EDITOR].editbuf(F(r)[i].body);
return 0;
}

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
      strcpy(s, "-1");
      notrunning = -1;
    }
  injstrigger = 0;
}
return s;
}

int Function::triggern(int tid) {
char *jsresult;
jsresult = trigger(tid);
return jsresult ? atoi(jsresult) : 0;
}

/* edit field with trigger */
int Function::edittrg(char *buf) {
int i;
char *jsresult; /* BIGSIZE never overflows with elk's output */
jsresult = trigger(edittrgtyp);
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

