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
#include "regex/re.h"

#define TRIGGRKEY(trgr) int i; if ((i = triggern(TRT_ ## trgr))) return i

int Function::dispatch() { /* returns notrunning 0..goon -1..quit <-1..error >0..form_id */
int undone;
CK = F(mapkey)(LK);
switch(CK) {
#ifdef NOTYETIMPLEMENTED
  case KEF_NAVI0:           /* menu()  */
#endif
  case -1:           LK = enter_the_form();                                   break;
  case KEF_NOOP:
  case KEF_NOOP2:    LK = 0;                                                  break;
  case KEF_REFRESH:
   if (            ( LK = triggern(TRT_REFRESH) ) )                           break;
                     LK = refresh_screen();                                   break;
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
  case KEF_NXTSETR:  LK = (CB.norec==1) ? KEF_END  : next_setrecords();       break;
  case KEF_PRESETR:  LK = (CB.norec==1 || CR==1) ? KEF_HOME : previous_setrecords(); break;
  case KEF_LIST:            /* flist() */
   if (            ( LK = triggern(TRT_LIST) ) )                              break;
                     LK = 0;                                                  break;
  case KEF_HELP:     LK = help_item();                                        break;
  case KEF_KEYHELP:
   if (            ( LK = triggern(TRT_KEY) ) )                               break;
                     LK = keys_help();                                        break;
  case KEF_PREREC:   LK = previous_record();                                  break;
  case KEF_COPYREC:  LK = fcopyrec();                                         break;
  case KEF_HOME:
  case KEF_END:
   switch(CM) {
    case MOD_UPDATE:
    case MOD_QUERY:                                                    break;
    case MOD_INSERT: LK = F(dirty) ? create_record() : clear_record(); break;
    case MOD_DELETE: LK = destroy_record();                            break;
   }
                     LK = fmove(CK==KEF_END ? 1 : -1, 0);             break;
  case KEF_COPY:     LK = fcopy();                                            break;
  case KEF_PASTE:    LK = fpaste();                                           break;
  case KEF_INSERT:
   if (            ( LK = triggern(TRT_INSERT) ) )                            break;
   switch(CM) {
    case MOD_UPDATE:
    case MOD_QUERY:  LK = insert_record();                             break;
    case MOD_INSERT: LK = F(dirty) ? create_record() : clear_record(); break;
    default:         LK = 0; y.toggle();                               break;
   }                                                                          break;
  case KEF_INS:
   if (            ( LK = triggern(TRT_INS) ) )                               break;
                     LK = 0; y.toggle();                                      break;
  case KEF_BACKDEL:         /* fbackdel() */
   if (            ( LK = triggern(TRT_BACKDEL) ) )                           break;
  case KEF_DELETE:
   if (            ( LK = triggern(TRT_DELETE) ) )                            break;
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
   if (            ( LK = triggern(TRT_COMMIT) ) )                            break;
   switch(CM) {
    case MOD_QUERY:  LK = execute_query();                             break;
    case MOD_UPDATE: LK = enter_query(&CB);                            break;
    case MOD_INSERT: LK = F(dirty) ? create_record() : clear_record(); break;
    case MOD_DELETE: LK = destroy_record();                            break;
   }                                                                          break;
  case KEF_EXIT:
   if (            ( LK = triggern(TRT_EXIT) ) )                              break;
                     LK = fexit();                                            break;
  case KEF_QUIT:
  case KEF_CANCEL:
   if (            ( LK = triggern(TRT_QUIT) ) )                              break;
   switch(CM) {
    case MOD_UPDATE:
    case MOD_QUERY:  LK = fquit();                                     break;
    case MOD_INSERT: LK = clear_record();                              break;
    case MOD_DELETE: switch_mode(MOD_UPDATE); LK = 0;                  break;
   }                                                                          break;
  case KEF_RIGHT:    LK = fedit(0);                                           break;
  case KEF_LEFT:     LK = fedit(-1);                                          break;
  case '|':          LK = MSG1(MSG_HELP, about);                              break;
  case '?':          LK = aboutwin();                                         break;

  default:
   undone = 1;
   if (CM == MOD_UPDATE) {
     undone = 0;
     switch(CK) {
#ifndef NOUSEDITOR
      case KEF_NAVI11:
      case '.':      LK = fedit(FED_FEDITOR);                          break;
      case '[':      LK = edit_map();                                  break;
      case ']':      LK = edit_file();                                 break;
      case '=':      LK = editrigger(TRT_EDITFIELD);                   break;
#endif
      case ' ':      LK = ftoggle();                                   break;
      case '+':      LK = fincrement(1);                               break;
      case '-':      LK = fincrement(-1);                              break;
      case '>':      LK = goto_cell();                                 break;
      case '~':      LK = search_cell();                               break;
      default:       undone = 1;
     }
   }
   if (undone && isprintable(LK))
                     LK = fedit(-1000 - LK);
   else              LK = 0;
                                                                              break;
}
return notrunning;
}

/* GENERAL */
int Function::enter_the_form() {
int i;
CBi = -1;
forall(block) if (i >= 4) enter_query(&F(b)[i]);
CBi = 4;
CFi = CB.blockfields[0];
if (!noentermac && (notrunning = triggern(TRT_ENTERFORM) > 1)) return 0;
if (updatemode) execute_query(); else if (!squerymode) insert_record();
return 0;
}

/* make a new record the current one */
void Function::enter_record(int rid) {
int i, pkfldi;
char *pkval;
char **curval;
CR = rid;
if (CR > 0) {
pkfldi = CB.primarykeys[0];
pkval = *fldi(pkfldi).valuep();
curval = &fldi(pkfldi).currentval;
/* need single primary key field and trigger for it and value has changed */
if (CB.prikeycnt == 1 && (i = qtrigger(TRT_ENTERECORD, CBi, pkfldi)) > -1 && pkval && (!*curval || strcmp(*curval, pkval))) {
  trigger(-i, NULL);
  free(*curval);
  *curval = strdup(pkval);
} } }

int Function::refresh_screen() {
F(needredraw) = 1;
return 0;
}

int Function::help_item() {
TRIGGRKEY(HELP);
MSG1(MSG_HELP,CF.helptext);
return 0;
}

int Function::keys_help() {
return F(p)[PGE_KEYHELP].showpopup();
}

int Function::aboutwin() {
return F(p)[PGE_ABOUT].showpopup();
}

#ifndef NOUSEDITOR
int Function::edit_map() {
return (CV && (!strcmp(CF.column, "seq") || !strcmp(CF.column, "page_id"))) ? F(p)[PGE_EDITOR].editmap(atoi(CV)) : 0;
}
#endif

#ifndef NOUSEDITOR
int Function::edit_file() {
char *s;
s = trigger(TRT_EDITFILE, NULL);
if (s && *s == '"') {
  s[strlen(s) - 1] = '\0';
  F(p)[PGE_EDITOR].editfile(s+1);
}
return 0;
}
#endif

/* NAVIGATION */
int Function::switch_mode(fmode mod) {
CM = mod;
if (CF.noedit()) fmove(0, 0);
return 0;
}

#define TRIGGRD(func,trgr,move,row,fld) int Function::func() {int i; return (i = triggern(TRT_ ## trgr)) ? i : move(row, fld); }
TRIGGRD(next_item,NEXTITEM,fmove,0,1)
TRIGGRD(previous_item,PREVITEM,fmove,0,-1)
TRIGGRD(next_block,NEXTBLOCK,fmove,1,0)
TRIGGRD(previous_block,PREVBLOCK,fmove,-1,0)
TRIGGRD(next_record,NEXTRECORD,fmover,0,1)
TRIGGRD(previous_record,PREVRECORD,fmover,0,-1)
TRIGGRD(next_setrecords,NEXTSETREC,fmover,0,CB.norec)
TRIGGRD(previous_setrecords,PREVSETREC,fmover,0,-CB.norec)

#define TRIGGR0(func,trgr,method) int Function::func() {int i; return (i = triggern(TRT_ ## trgr)) ? i : method(); }
TRIGGR0(exec_query,EXEQUERY,execute_query)

/* move from field to field */
int Function::fmove(int bi, int fi) {
if (bi) {
  CBi = ((CBi - 4 + F(numblock)-4 + bi) % (F(numblock)-4) + 4);
  CFi = CB.blockfields[0];
}
if (fi < NFIELD1) CFi = CB.blockfields[ (CF.sequencenum-1 + CB.fieldcount + fi) % CB.fieldcount ];
else              CFi = fi - NFIELD1 - 1;
if (CF.noedit()) fmove(0, fi<0 ? -1 : 1);
return 0;
}

/* move from record to record */
int Function::fmover(int rn, int ri) {
int newcr;
switch (CM) {
 case MOD_QUERY:  return 0;                                                         break;
 case MOD_UPDATE:                                                                   break;
 case MOD_INSERT: if (!yesno(MSG(MSG_DIRTY))) create_record(); else clear_record(); break;
 case MOD_DELETE: if (!yesno(MSG(MSG_DIRTY))) destroy_record();                     break;
}
switch_mode(MOD_UPDATE);
newcr = rn ? rn : CR;
if (newcr > 0) {
  newcr += ri;
  if (newcr > CN) {
    if (ri == 1) MSG(MSG_LAST);
    newcr = CN;
  }
  if (newcr < 1) {
    if (ri == -1) MSG(MSG_FIRST);
    newcr = 1;
  }
  if (abs(ri) > 1) {
    CB.toprec += ri;
    if (CB.toprec > CN - CB.norec) CB.toprec = CN - CB.norec;
    if (CB.toprec < 1) CB.toprec = 1;
  }
  enter_record(newcr);
  fwindow();
}
if (CF.noedit()) fmove(0, 0);
return 0;
}

/* adjust the top row in multiple rows blocks */
void Function::fwindow() {
if (CB.norec > 1) {
       if (CR <  CB.toprec)            CB.toprec = CR;
  else if (CR >= CB.toprec + CB.norec) CB.toprec = CR - CB.norec + 1;
} else {
  CB.toprec = CR;
} }

/* goto entered cell */
int Function::goto_cell() {
int pressed;
char a[NORSIZE];
let(a, "block.field:row");
pressed = F(p)[PGE_STATUS].sedit(a, 0, FTY_ALL, 30);
if (pressed == KEY_ENTER) fgoto(a);
return 0; //pressed;
}

void Function::fgoto(char *sel) {
int fieldn, rown;
char *colon;
rown = 0;
if ((colon = strchr(sel, ':'))) {
  *colon = '\0';
  rown = atoi(colon + 1);
}
fieldn = F(qfield)(sel);
if (fieldn != -1) {
  CBi = fldi(fieldn).blockindex;
  CFi = fieldn;
  if (CF.noedit()) fmove(0, 1);
  fmover(rown, 0);
} }

/* search with regex */
int Function::search_cell() {
int pressed;
if (CM == MOD_UPDATE) {
  pressed = F(p)[PGE_STATUS].sedit(CB.searchre, 0, FTY_ALL, 60);
  if (pressed == KEY_ENTER) fsearch(CB.searchre);
}
return 0; //pressed;
}

void Function::fsearch(char *rex) {
int fldn, rown, s;
char **val;
char *src, *tgt;
char a[HUGSIZE];
re_t re;
re = re_compile(rex);
fldn = CF.sequencenum + 1;
for(rown=CR; rown<=CN; rown++) {
  for(; fldn<=CB.fieldcount; fldn++) {
    val = CB.q->w(rown, fldn);
    if (*val) {
      if (matchnocas) {
        tgt = a;
        for (src=*val; *src; src++) *tgt++ = tolower(*src);
        tgt = a;
        val = &tgt;
      }
      if (re_matchp(re, *val, &s) != -1) {
        CFi = CB.blockfields[fldn-1];
        CR = rown;
        if (CF.noedit()) fmove(0, 1);
        fmover(0, 0);
        return;
      }
    }
  }
  fldn = 1;
} }

/* EDITING */
int Function::insert_record() {
if (CM == MOD_UPDATE || CM == MOD_QUERY) {
  CB.q->splice(CR++);
  CB.filldefault(CR);
  switch_mode(MOD_INSERT);
} else {
  MSG(MSG_QUERYM);
}
fwindow();
return 0;
}

int Function::create_record() {
if (CB.insert(CR)) MSG1(MSG_SQL, (char*)CB.querystr);
switch_mode(MOD_UPDATE);
enter_record(CR);
return 0;
}

/* double pressed should copy record */
int Function::fcopyrec() {
if (CM != MOD_UPDATE && CM != MOD_INSERT) return 0;
if (CR > 1) {
  changed = fedit(FED_TRIGGER - TRT_COPYREC);
  return changed==KEF_CANCEL ? 0 : changed;
} else {
  MSG(MSG_NOREC2);
  return 0;
} }

int Function::fcopy() {
if (CM == MOD_UPDATE && CV) return triggern(TRT_COPY);
return 0;
}

int Function::fpaste() {
if (CM != MOD_UPDATE) return 0;
  changed = fedit(FED_TRIGGER - TRT_PASTE);
  return changed==KEF_CANCEL ? 0 : changed;
}

int Function::ftoggle() {
int editmode;
editmode = qtrigger(TRT_EDITFIELD) > -1 ? FED_TRIGGER - TRT_EDITFIELD : FED_TOGGLE;
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
if (pos ==  0) {TRIGGRKEY(RIGHT);}
if (pos == -1) {TRIGGRKEY(LEFT);}
changed = 0;
switch(CM) {
 case MOD_INSERT:
 case MOD_QUERY:
  if (pos == KEF_DEL) {
    CF.clear();
    changed = 0;
    break;
  }
 case MOD_UPDATE:
  if (CM == MOD_UPDATE && !*fldi(CB.primarykeys[0]).valuep()) {
    MSG(MSG_NOPRIKEY);
    break;
  }
  changed = CF.edit(pos);
  if (changed != KEF_CANCEL)
    if (CM == MOD_UPDATE && CF.basetable)
      if (CB.update(CR, CF.sequencenum))
        MSG1(MSG_SQL, (char*)CB.querystr);
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
if (noentermac || triggern(TRT_EXITFORM) <= 1) notrunning = -1;
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
blk->currentrec = 0;
blk->toprec = 1;
F(dirty) = 0;
if (blk == &CB) switch_mode(MOD_QUERY);
else            blk->rmode = MOD_QUERY;
return 0;
}

/* get record data into the block based on the query columns entered */
int Function::execute_query() {
int i, j, k;
int cf;
int triggerdfields[NFIELD1];
int tfn;
F(p)[PGE_STATUS].working();
if (CM != MOD_QUERY) enter_query(&CB);
if (CB.select()) MSG1(MSG_SQL, (char*)CB.querystr); else {
  if (CN > 0) {
    /* optimized - first check for triggers
     * collect them in triggerdfields
     * and run them within one loop through the records
     */
    tfn = 0;
    forall(trigger)
      if (trgi(i).trgtyp == TRT_POSTQUERY)
        if ((k = trgi(i).fieldindex) > -1)
          if (fldi(k).blockindex == CBi)
            triggerdfields[tfn++] = trgi(i).fieldindex;
    if (tfn > 0) {
      CM = MOD_UPDATE;
      cf = CFi;
      for (j=0; j<CN; j++) {
        for (k=0; k<tfn; k++) {
          CFi = triggerdfields[k];
          CR = j + 1;
          fedit(FED_TRIGGER);
        }
      }
      CFi = cf;
    }
    enter_record(1);
    switch_mode(MOD_UPDATE);
  } else {
    MSG(MSG_COUNT0);
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
  CB.destroy(CR);
  clear_record();
} else switch_mode(MOD_UPDATE);
fwindow();
return 0;
}

int Function::clear_record() {
CB.q->splice(-CR);
if (CN) {
  switch_mode(MOD_UPDATE);
  if (CR > CN) CR = CN;
  enter_record(CR);
} else enter_query(&CB);
return 0;
}

/* TRIGGER */
#ifndef NOUSEDITOR
int Function::editrigger(int tid) {
int i;
if ((i = qtrigger(tid)) > -1) F(p)[PGE_EDITOR].editbuf(trgi(i).body);
return 0;
}
#endif

/* search for trigger */
int Function::qtrigger(int tid) { return qtrigger(tid, CBi, CFi); }
int Function::qtrigger(int tid, int bid, int fid) {
int i;
int blk_id, fld_id;
blk_id = blki(bid).block_id;
fld_id = fldi(fid).field_id;
forall(trigger) {
  if ( trgi(i).trgtyp == tid &&
      (trgi(i).trgblk + trgi(i).trgfld == 0 || trgi(i).trgblk == blk_id || trgi(i).trgfld == fld_id)) return i;
  else if (trgi(i).trgblk > blk_id || trgi(i).trgfld > fld_id) break;
}
return -1;
}

/* raw trigger call returns        *char   when
 * -                               NULL    trigger not found or disabled
 * 0;                              "0"     background executed go default action
 * 1;                              "1"     foreground or key macro no default action
 * nn;                             "nn"    next key action triggers only return numbers
 * "string";                       ""str"" for setting triggers
 * raise error;                    "-1"    javascript error
 */
char *Function::trigger(int tid, char *av0) {
int i;
char *s;
s = NULL;
if (!intrigger) { /* not nice but fast */
  i = tid < 0 ? -tid : qtrigger(tid);
  if (i != -1) {
    s = trgi(i).execute(av0);
    if (*s != '"' && !isdigit(*s)) {
      g.logfmt("[%d]%s", tid, s);
      MSG1(MSG_JS, s);
      strcpy(s, "-1");
      notrunning = -1;
    }
  }
}
return s;
}

/* execute action trigger
 * no difference whether not or background executed =0
 */
int Function::triggern(int tid) {
char *jsresult;
jsresult = trigger(tid, NULL);
return jsresult ? atoi(jsresult) : 0;
}

/* execute setting trigger */
int Function::triggers(int tid, char *buf) {
int i;
char *jsresult; /* BIGSIZE never overflows with elk's output */
jsresult = trigger(tid, buf);
if (!jsresult)                 { return 0; }
else if (*jsresult == '"')     {
  strcpy(buf, jsresult+1);
  i = strlen(buf) - 1;
  assert(*(buf+i) == '\"');
  *(buf+i) = '\0';
                                 return KEF_NXTFLD;
}
else if (isdigit(*jsresult))   { return atoi(jsresult); }
else                           { return KEF_CANCEL; }
}

/* edit field with trigger */
int Function::edittrg(char *buf, int typ) {
return triggers(typ, buf);
}

