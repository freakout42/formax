/* The primary object of a formax application is the form.
 * A form is made up of additional objects.
 * These objects link the form to database elements,
 * such as columns and tables,
 * and provide control over the flow of execution.
 * should be a pointer and not static
 */
#include <assert.h>
#include "runform.h"

Form::Form() {
let(table,  "forms");
let(prikey, "id");
let(attrs,  "id,name,title");
let(order,  "id");
columni = 3;
}

void Form::rconnect() {
rerror.connect(*this);
rblock.connect(*this);
rfield.connect(*this);
rpage.connect(*this);
rmap.connect(*this);
rtrigger.connect(*this);
}

// fill objects with configuation
int Form::fill(int fid) {
int i, s;

// the form itself
stmt = NULL;
if ((s = ropen())) return s;
letf(t(where), "id = %d", fid);
if ((s = query())) return s;
if (q->rows != 1) return 7;
let(id,    q->v(1, 1));
let(name,  q->v(1, 2));
let(title, q->v(1, 3));
rclose();

// triggers
if (rtrigger.init(fid)) return 9;
if ((s = rtrigger.query())) return s;
numtrigger = rtrigger.q->rows;
if (numtrigger > NTRIGGERS) return 7;
for (i=0; i<numtrigger; i++) {
  if (r[i].init(rtrigger.q, i+1)) return 9;
}
rtrigger.rclose();

// pages - page 0 is status/edit/message window
if (rpage.init(fid)) return 9;
if ((s = rpage.query())) return s;
numpage = rpage.q->rows;
if (numpage > NBLOCKS) return 7;
for (i=0; i<numpage; i++) if (p[i].init(rpage.q, i+1)) return 9;
rpage.rclose();
if (rmap.init(1)) return 9;
if ((s = rmap.query())) return s;
if (p[1].maps(rmap.q)) return 9;
rmap.rclose();

// error messages
if (rerror.init()) return 9;
if ((s = rerror.query())) return s;
e = rerror.q;
rerror.q = new(Qdata);
rerror.rclose();

// blocks - block 0 is for free queries/sql statements
if (rblock.init(fid)) return 9;
if ((s = rblock.query())) return s;
numblock = rblock.q->rows;
if (numblock > NBLOCKS) return 7;
for (i=0; i<numblock; i++) {
  if (b[i].ropen()) return 9;
  if (b[i].init(rblock.q, i+1)) return 9;
}
rblock.rclose();

// fields
if (rfield.init(fid)) return 9;
if ((s = rfield.query())) return s;
numfield = rfield.q->rows;
if (numfield > NFIELDS) return 7;
for (i=0; i<numfield; i++) {
  if (l[i].init(rfield.q, i+1)) return 9;
}
rfield.rclose();

return 0;
}

void Form::clear() {
int i;
e->freed();
for (i=0; i<numblock; i++) b[i].rclose();
for (i=0; i<numpage;  i++) p[i].destroy();
}

// set-up screen and pages and execute through the event dispatcher
int Form::run() {
int i, s;
if (y.init()) return 6;
  for (i=0; i<numpage; i++) p[i].create();
  lastkey = -1;
  while (!(s = u.dispatch())) {
    lastkey = F(p[0]).wait();
  }
y.closedisplay();
return s==-1 ? 0 : s;
}

int Form::mapkey(int ckey) {
int ck;
ck = ispunctation(ckey);
switch(ck) {                                  /* C */
  case KEY_F(0):       return KEF_REFRESH;    /* l      frefresh */
  case KEY_F(1):       return KEF_HELP;       /* @      fhelp */
  case KEY_F(2):       return KEF_COPY;       /* c      fcopy */
  case KEY_F(3):       return KEF_PASTE;      /* v      fpaste */
  case KEY_F(4):       return KEF_LIST;       /* u      flist */
  case KEY_F(5):       return KEF_COPYREC;    /* t      fcopyrec */
  case KEY_F(6):       return KEF_INSERT;     /* o      create_record */
  case KEY_F(7):       return KEF_DELETE;     /* d      delete_record */
  case KEY_F(8):       return KEF_EXIT;       /* z      exit */
  case KEY_F(9):       return KEF_QUIT;       /* y      cancel/quit */
  case KEY_F(10):      return KEF_QUERY;      /* x      enter_query */
  case KEY_F(11):      return KEF_KEYHELP;    /* k      keys_help */
  case KEY_ESC:        return KEF_CANCEL;     /* esc    cancel/quit */
  case KEY_CANCEL:     return KEF_CANCEL;     /* cancel cancel/quit */
  case KEY_IC:         return KEF_INSERT;     /* j      create_record */
  case KEY_HOME:       return KEF_HOME;       /* a      fhome */
  case KEY_PPAGE:      return KEF_PRESETR;    /* r      fpresetr */
  case KEY_DC:         return KEF_DELETE;     /* d      delete_record */
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

