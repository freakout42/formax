/* The primary object of a formax application is the form.
 * A form is made up of additional objects.
 * These objects link the form to database elements,
 * such as columns and tables,
 * and provide control over the flow of execution.
 * should be a pointer and not static
 */
#include "runform.h"

Form::Form() {
let(table,  "forms");
let(prikey, "id");
let(attrs,  "id,name,title,mnugrp");
let(order,  "id");
columni = 4;
}

/* fill objects with configuation from sqlite db (.frm) */
int Form::fill(int fid) {
int i, s;
Block *blk;
Form *runningform;

runningform = f;
f = this;

/* the form configuration itself */
connect(dbconn[0]);
stmt = NULL;
if ((s = ropen())) return s;
letf(t(where), "id = %d", fid);
empty(order);
empty(condition);
if ((s = query())) return s;
if (q->rows != 1) return 7;
let(id,    q->v(1, 1));
let(name,  q->v(1, 2));
let(title, q->v(1, 3));
let(signt, q->v(1, 4));
if (pwdencrypt && strcmp(signt, runningsignature)) {
  if (!getuid()) {
    /* im root so sign form with crypted md5 hash */
    letf((char*)querystr, MEDSIZE, "update %s set %s = ? where %s", table, "mnugrp", where);
    bindv[0] = runningsignature;
    bindv[1] = NULL;
    execute();
    return 21;
  }
  return 20;
}
needredraw = 0;
rclose();

/* connect all configuration tables to the form database */
rerror.connect(dbconn[0]);
rblock.connect(dbconn[0]);
rfield.connect(dbconn[0]);
rpage.connect(dbconn[0]);
rmap.connect(dbconn[0]);
rtrigger.connect(dbconn[0]);

/* error messages */
if (rerror.init()) return 9;
if ((s = rerror.query())) return s;
if (rerror.q->rows != VERMSGS) return 19;
e = rerror.q;
rerror.q = new Qdata();
rerror.rclose();

/*               i  id seq desc
 * pages - page [0] 1  0   status/edit/message window
 *              [1] 5  1   working window
 *              [2] 2  2   key help popup
 *              [3] 3  3   editor
 *              [4] 4  4   about
 * INSERT INTO pages VALUES(1,1,'status',0,'',1,80,0,0,0,0,0);
 * INSERT INTO pages VALUES(2,0,'keyhelp',2,'',16,41,2,30,1,1,0);
 * INSERT INTO pages VALUES(3,0,'editor',3,'',21,65,2,14,1,1,0);
 * INSERT INTO pages VALUES(4,0,'about',4,'',16,41,2,30,1,1,0);
 * INSERT INTO pages VALUES(5,0,'formax',1,'',23,80,1,0,0,1,0);
 */
if (rpage.init(fid)) return 9;
if ((s = rpage.query())) return s;
numpage = rpage.q->rows;
if (numpage > NBLOCKS) return 7;
forall(page) {
  if (p[i].init(rpage.q, i+1)) return 9;
  if (rmap.init(p[i].page_id)) return 9;
  if ((s = rmap.query())) return s;
  if (p[i].maps(rmap.q)) return 9;
  rmap.rclose();
}
rpage.rclose();

/* blocks 0-3 is for free queries/sql statements */
if (rblock.init(fid)) return 9;
if ((s = rblock.query())) return s;
numblock = rblock.q->rows;
if (numblock > NBLOCKS) return 7;
forall(block) {
  blk = &b[i];
  if (blk->init(rblock.q, i+1)) return 9;
  if (i < 4) blk->connect(dbconn[i+1]);
  else       blk->connect(dbconn[(blk->sequence % 10)+1]);
  if (blk->ropen()) return 9;
}
rblock.rclose();

/* fields */
if (rfield.init(fid)) return 9;
if ((s = rfield.query())) return s;
numfield = rfield.q->rows;
if (numfield > NFIELDS) return 7;
forall(field) if (l[i].init(rfield.q, i+1, b)) return 9;
rfield.rclose();

/* triggers */
if (rtrigger.init(fid)) return 9;
if ((s = rtrigger.query())) return s;
numtrigger = rtrigger.q->rows;
if (numtrigger > NTRIGGERS) return 7;
forall(trigger) {
  if (r[i].init(rtrigger.q, i+1, &rmap)) return 9;
}
rtrigger.rclose();
/* move the maps connection to the user database
 * this way its possible to edit pages and trigger bodies
 * from a form (formax.frm)
 */
rmap.connect(dbconn[1]);

f = runningform;
return 0;
}

/* cleanup the form with all blocks and pages */
void Form::clear() {
int i;
e->freed();
delete(e);
forall(block)   b[i].rclose();
forall(field)   l[i].destroy();
forall(trigger) r[i].destroy();
forall(page)    p[i].destroy();
}

/* set-up screen and pages and execute through the event dispatcher */
int Form::run() {
int i, s;
 f = this;
  for (i=0; i<numpage; i++) p[i].create();
  lastkey = -1;
  while (!(s = u.dispatch())) { /* returns notrunning 0..goon -1..quit <-1..error >0..form_id */
    lastkey = F(p[PGE_STATUS]).wait();
  }
return s;
}

/* search field */
int Form::qfield(char *sel) {
int i;
char selector[SMLSIZE];
for (i=0; i<numfield; i++) {
  letf(t(selector), "%s.%s", b[l[i].blockindex].table, l[i].column);
  if (!strcmp(sel, selector)) break;
}
return i<numfield ? i : -1;
}

/* application key mapping physical to function */
int Form::mapkey(int ckey) {
int ck;
ck = ispunctation(ckey);
switch(ck) {                                  /* C */
  case KEY_F(1):       return KEF_HELP;       /* @      fhelp */
  case KEY_F(2):       return KEF_COPY;       /* c      fcopy */
  case KEY_F(3):       return KEF_PASTE;      /* v      fpaste */
  case KEY_F(4):       return KEF_COPYREC;    /* t      fcopyrec */
  case KEY_F(5):       return KEF_LIST;       /* u      flist */
  case KEY_F(6):       return KEF_INSERT;     /* o      create_record */
  case KEY_F(7):       return KEF_DELETE;     /* d      delete_record */
  case KEY_F(8):       return KEF_EXIT;       /* z      exit */
  case KEY_F(9):       return KEF_QUIT;       /* y      cancel/quit */
  case KEY_F(10):      return KEF_QUERY;      /* x      enter_query */
  case KEY_F(11):      return KEF_KEYHELP;    /* k      keys_help */
  case KEY_F(12):      return KEF_REFRESH;    /* l      frefresh */
  case KEY_ESC:        return KEF_CANCEL;     /* esc    cancel/quit */
  case KEY_CANCEL:     return KEF_CANCEL;     /* cancel cancel/quit */
  case KEY_IC:         return KEF_INSERT;     /* j      create_record */
  case KEY_HOME:       return KEF_HOME;       /* a      fhome */
  case KEY_PPAGE:      return KEF_PRESETR;    /* r      fpresetr */
  case KEY_DC:         return KEF_DELETE;     /* d      delete_record */
  case KEY_LL:
  case KEY_END:        return KEF_END;        /* e      fend */
  case KEY_NPAGE:      return KEF_NXTSETR;    /* w      fnxtsetr */
  case KEY_UP:         return KEF_PREREC;     /* p      fmover */
  case KEY_LEFT:       return KEF_LEFT;       /* b      fedit */
  case KEY_DOWN:       return KEF_NXTREC;     /* n      fmover */
  case KEY_RIGHT:      return KEF_RIGHT;      /* f      fedit */
  case KEY_TAB:        return KEF_NXTFLD;     /* i      fmove */
  case KEY_BTAB:       return KEF_PREFLD;     /* g      fmove */
  case KEY_BACKSPACE:  return KEF_BACKDEL;    /* h      fbackspace */
  case KEY_ENTER:      return KEF_COMMIT;     /* m      execute */
  default:             return ck;
 }
}

