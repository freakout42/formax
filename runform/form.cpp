#include <assert.h>
#include "runform.h"

int *Form::init() {
let(table,  "forms");
let(prikey, "id");
let(attrs,  "id,name,title");
let(order,  "id");
columni = 3;
d.ysiz = 0;
return 0;
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
rerror.q->init();
rerror.rclose();

// blocks - block 0 is for free queries/sql statements
if (rblock.init()) return 9;
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
Function u;
if (d.init()) return 6;
  for (i=0; i<numpage; i++) p[i].create();
  dirty = 0;
  lastkey = -1;
  while (!(s = u.dispatch())) {
    lastkey = f.p[0].wait();
  }
d.closedisplay();
return s==-1 ? 0 : s;
}

int Form::mapkey(int ckey) {
int ck;
ck = ispunctation(ckey);
switch(ck) {
  case KEY_F(1):       return KEF_HELP;
  case KEY_F(2):       return KEF_LIST;
  case KEY_F(3):       return KEF_COPY;
  case KEY_F(4):       return KEF_PASTE;
  case KEY_F(5):       return KEF_COPYREC;
  case KEY_F(6):       return KEF_INSREC;
  case KEY_F(7):       return KEF_QUERY;
  case KEY_F(8):       return KEF_EXIT;
  case KEY_F(9):       return KEF_CANCEL;
  case KEY_ESC:        return KEF_CANCEL;
  case KEY_CANCEL:     return KEF_CANCEL;
  case KEY_HOME:       return KEF_HOME;
  case KEY_LEFT:       return KEF_LEFT;
  case KEY_DC:         return KEF_DELETE;
  case KEY_END:        return KEF_END;
  case KEY_RIGHT:      return KEF_RIGHT;
  case KEY_BTAB:       return KEF_PREFLD;
  case KEY_BACKSPACE:  return KEF_BACKDEL;
  case KEY_TAB:        return KEF_NXTFLD;
//case KEY_F(0):       return KEF_REFRESH;
  case KEY_ENTER:      return KEF_COMMIT;
  case KEY_DOWN:       return KEF_NXTREC;
  case KEY_IC:         return KEF_INSERT;
  case KEY_UP:         return KEF_PREREC;
  case KEY_PPAGE:      return KEF_PRESETR;
  case KEY_NPAGE:      return KEF_NXTSETR;
  case KEF_NAVI0:      return KEF_MENU;
  default:             return ck;
 }
}

