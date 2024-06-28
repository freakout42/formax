#include <stdlib.h>
#include <string.h>
#include "runform.h"

int Page::init(Qdata *pag, int rix) {
memset(map, 0, sizeof(map));
let(name,pag->v(rix, 1));
ysiz   = pag->n(rix, 2);
xsiz   = pag->n(rix, 3);
vwpy0  = pag->n(rix, 4);
vwpx0  = pag->n(rix, 5);
border = pag->n(rix, 6);
return 0;
}

int Page::maps(Qdata *qma) {
int i, r;
for (i = 1; i <= qma->rows; i++) {
  r = qma->n(i, 1) - 1;
  if (r > NLINES) return 1;
  map[r] = qma->c(i, 2);
}
return 0;
}

void Page::create() {
int i;
wndw = newwin(ysiz, xsiz, vwpy0, vwpx0);
wattrset(wndw, A_NORMAL);
wattron(wndw, COLOR_PAIR(0));
if (border) wbox();
for (i=0; i<NLINES; i++) if (map[i]) writes(i+(border?1:0), border?1:0, map[i]);
}

void Page::destroy() {
int i;
for (i=0; i<NLINES; i++) free(map[i]);
delwin(wndw);
}

int Page::wait() {
int i;
writes(0, 2,                 f.title);
writef(0, 12, 0, 4,  "%3s-", f.id);
writes(0, 16,                f.name);
writes(0, 24,                f.b[f.curblock].table);
writef(0, 30, 0, 3,  "%3d",  f.lastkey);
writes(0, 60,                f.p[1].name);
writes(0, 67,                "runform-");
writes(0, 75,                (char*)VERSION);
refr();
for (i=0; i<f.numfield; i++) f.p[1].wfield(i);
for (i=1; i<f.numpage; i++) f.p[i].refr();
return getkey();
}

int Page::message(int num) {
writef(0, 0, 0, 80, "%s", f.d.msg(num));
//move(0,0);
refr();
return getkey();
}

void Page::wfield(int fno) {
Field *fld;
fld = f.l+fno;
writef(fld->line, fld->col, fno==f.curfield ? COL_CURRENT : COL_FIELD, fld->dlen, "%s", fld->name);
if (fno == f.curfield) wmov(fld->line, fld->col);
}
