#include <string.h>
#include "runform.h"

int Page::init(Qdata pag, int rix) {
memset(map, 0, sizeof(map));
  name   = pag.c(rix, 1);
  ysiz   = pag.n(rix, 2);
  xsiz   = pag.n(rix, 3);
  vwpy0  = pag.n(rix, 4);
  vwpx0  = pag.n(rix, 5);
  border = pag.n(rix, 6);
return 0;
}

int Page::maps(rMap *rmap) {
int i, r;
for (i = 1; i <= rmap->q.rows; i++) {
  r = rmap->q.n(i, 1) - 1;
  map[r] = rmap->q.c(i, 2);
}
return 0;
}

void Page::create() {
int i;
wndw = newwin(ysiz, xsiz, vwpy0, vwpx0);
if (border) wbox();
for (i = 0; i < NLINES; i++) if (map[i]) writes(i+(border?1:0), border?1:0, map[i]);
}

int Page::wait() {
writes(0, 2,                 f.title);
writef(0, 12, 0, 4,  "%3s-", f.id);
writes(0, 16,                f.name);
writes(0, 24,                f.b[f.cb].name);
writef(0, 30, 0, 3,  "%3d",  f.lk);
writes(0, 60,                f.p[1].name);
writes(0, 67,                "runform-");
writes(0, 75,                (char*)VERSION);
move(0,0);
refresh();
return getkey();
}
