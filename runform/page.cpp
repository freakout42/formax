#include <stdlib.h>
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
cwin(ysiz, xsiz, vwpy0, vwpx0);
if (border) wbox();
for (i=0; i<NLINES; i++) if (map[i]) writes(i+(border?1:0), border?1:0, map[i]);
}

void Page::destroy() {
int i;
for (i=0; i<NLINES; i++) free(map[i]);
dwin();
}

static char *rmodes[] = { "Insert", "Query ", "Update", "Delete" };

int Page::wait() {
int i;
weras();
writef(0,  2, 0, 2,  "%2s-", f.id);
writes(0,  5,                f.name);
writef(0, 20, 0, 9,  "%s",   f.b[f.curblock].table);
writef(0, 30, 0, 9,  "%s",   f.l[f.curfield].name);
writes(0, 49,                rmodes[f.rmode]);
writes(0, 56,                (char*)(insertmode ? "Ins" : "Rep"));
writef(0, 63, 0, 3,  "%3d",  f.lastkey);
writes(0, 67,                "runform-");
writes(0, 75,                (char*)VERSION);
refr();
for (i=0; i<f.numfield; i++) f.l[i].fshow(i == f.curfield);
for (i=1; i<f.numpage;  i++) f.p[i].refr();
return getkey();
}

int Page::message(int num, char *pnt) {
writef(0, 0, 0, 80, "MAX-%3d %s %s", num, f.d.msg(num), pnt);
wmov(0,0);
refr();
return getkey();
}

