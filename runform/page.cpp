#include <stdio.h>
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
createwindow(ysiz, xsiz, vwpy0, vwpx0);
if (border) wbox();
for (i=0; i<NLINES; i++) if (map[i]) writes(i+(border?1:0), border?1:0, map[i]);
}

void Page::destroy() {
int i;
for (i=0; i<NLINES; i++) free(map[i]);
deletewindow();
}

static char *rmodes[] = { "Insert", "Query ", "Update", "Delete" };

int Page::wait() {
int i;
wera();
writef(0,  2, 0, 2,  "%2s-",    f.id);
writes(0,  5,                   f.name);
writef(0, 20, 0, 8,  "%s",      f.b[f.curblock].table);
writef(0, 29, 0, 9,  "%s",      f.l[f.curfield].name);
writef(0, 39, 0, 9,  "%4d/%4d", CB.currentrecord, CB.q->rows);
writef(0, 49, COL_HEADER,6,"%s",rmodes[f.rmode]);
writef(0, 56, COL_HEADER,3,"%s",(char*)(insertmode ? "Ins" : "Rep"));
writef(0, 63, 0, 3,  "%3d",     LK);
writes(0, 67,                   "runform-");
writes(0, 75,                   (char*)VERSION);
refr();
for (i=0; i<f.numfield; i++) f.l[i].show(i == f.curfield);
for (i=1; i<f.numpage;  i++) f.p[i].refr();
return LK ? LK : getkb();
}

int Page::message(int ern, char *pnt) {
int i;
static char empty[] = "";
char *pntst;
if (f.d.ysiz > 0) {
if (pnt) pntst = pnt; else pntst = empty;
if (strlen(pntst) > LINE0SIZE-12) i = strlen(pntst) - LINE0SIZE + 12; else i = 0;
writef(0, 0, 0, LINE0SIZE, "MAX-%03d %s %s", ern, f.d.msg(ern), pntst+i);
wmov(0,0);
refr();
return getkb();
}
return 0;
}

