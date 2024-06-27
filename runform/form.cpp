#include <assert.h>
#include <string.h>
#include "runform.h"

int *Form::init() {
stmt = NULL;
if (ropen()) return NULL;
let(table,  "forms");
let(prikey, "id");
let(where,  "");
let(order,  "id");
return 0;
}

// fill objects with configuation
int Form::fill(int fid) {
int i, s;

// the form itself
letf(t(where), "id = %d", fid);
if ((s = query("id,name,title"))) return s;
if (q->rows != 1) return 7;
let(id,    q->v(1, 1));
let(name,  q->v(1, 2));
let(title, q->v(1, 3));
rclose();

// error messages
if (rerror.init()) return 9;
if ((s = rerror.query("num,severity,etext"))) return s;
e = rerror.q;
rerror.q = new(Qdata);
rerror.q->init();
rerror.rclose();

// blocks - block 0 is for free queries/sql statements
if (rblock.init()) return 9;
if ((s = rblock.query("name,prikey,whereand,orderby"))) return s;
numblock = rblock.q->rows;
if (numblock > NBLOCKS) return 7;
for (i=0; i<numblock; i++) {
  if (b[i].ropen()) return 9;
  if (b[i].init(rblock.q, i+1)) return 9;
}
rblock.rclose();
curblock = 1;

// fields
if (rfield.init()) return 9;
if ((s = rfield.query("name"))) return s;
numfield = rfield.q->rows;
if (numfield > NFIELDS) return 7;
for (i=0; i<numfield; i++) {
  if (l[i].init(rfield.q, i+1)) return 9;
}
rfield.rclose();
curfield = 1;

// pages - page 0 is status/edit/message window
if (rpage.init()) return 9;
if ((s = rpage.query("name,ysiz,xsiz,vwpy0,vwpx0,border"))) return s;
numpage = rpage.q->rows;
if (numpage > NBLOCKS) return 7;
for (i=0; i<numpage; i++) if (p[i].init(rpage.q, i+1)) return 9;
rpage.rclose();
if (rmap.init(1)) return 9;
if ((s = rmap.query("line,mtext"))) return s;
if (p[1].maps(rmap.q)) return 9;
rmap.rclose();

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
  dirty = lastkey = 0;
  while (!(s = u.dispatch()))
    lastkey = p[0].wait();
d.dclose();
return s==1 ? 0 : s;
}
