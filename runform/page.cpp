#include <stdlib.h>
#include "runform.h"

int Page::init(Qdata *pag, int rix) {
memset(map, 0, sizeof(map));
page_id = pag->n(rix, 1);
let(name, pag->v(rix, 2));
ysiz    = pag->n(rix, 3);
xsiz    = pag->n(rix, 4);
vwpy0   = pag->n(rix, 5);
vwpx0   = pag->n(rix, 6);
border  = pag->n(rix, 7);
return 0;
}

// must be rewritten for multiple pages
int Page::maps(Qdata *qma) {
int i, r;
char *t, *p;
for (i = 1; i <= qma->rows; i++) {
  r = qma->n(i, 1) - 1;
  if (r > NLINES) return 1;
  map[r] = qma->c(i, 2);
  p = NULL;
  // white the $nn_ pos markers
  for (t=map[r]; *t; t++) {
    if (*t == '$') {
      p = t;
    } else {
      if (p && (!(*t == '_' || *t == '.' || (*t >= '0' && *t <= '9')))) {
        if (t - p > 1) while (p < t) *p++ = ' ';
        p = NULL;
      }
    }
  }
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

static char *rmodes[] = RMODENAMES;

int Page::wait() {
int i;
char commit[16];
switch (F(rmode)) {
 case MOD_QUERY:  strcpy(commit,            "  Execute-Query");                      break;
 case MOD_UPDATE: strcpy(commit,            "    Enter-Query");                      break;
 case MOD_INSERT: strcpy(commit, F(dirty) ? "  Insert-Record" : "   Clear-Record" ); break;
 case MOD_DELETE: strcpy(commit,            "  Delete-Record");                      break;
}
wera();
writef(0,  2, 0, 2,  "%2s-",      F(id));
writes(0,  5,                     F(name));
writef(0, 16, 0, 8,  "%s",        username);
writef(0, 25, 0, 8,  "%s",        CB.table);
writef(0, 34, 0, 9,  "%s",        CF.name);
writef(0, 44, 0, 9,  "%4d/%4d",   CB.currentrecord, CB.q->rows);
writef(0, 54, COL_HEADER,6,"%s",  rmodes[F(rmode)]);
writef(0, 61, COL_HEADER,3,"%s",  (char*)(insertmode ? "Ins" : "Rep"));
//ites(0, 67,                     "runform-");
//itef(0, 70, 0, 4,  "%4d",       F(lastcmd));
//ites(0, 75,                     (char*)VERSION);
writef(0, 65, COL_COMMIT,15,"%s", commit);
refr();
for (i=0; i<F(numfield); i++) F(l[i]).show(i == F(curfield));
for (i=1; i<F(numpage);  i++) F(p[i]).refr();
return LK ? LK : getkb();
}

int Page::message(int ern, char *pnt) {
int i;
static char empty[] = "";
char *pntst;
if (F(y).ysiz > 0) { // can only display with open window
if (pnt) pntst = pnt; else pntst = empty;
if (strlen(pntst) > LINE0SIZE-12) i = strlen(pntst) - LINE0SIZE + 12; else 
i = 0;
writef(0, 0, 0, LINE0SIZE, "MAX-%03d %s %s", ern, F(y).msg(ern), pntst+i);
//writef(0, 76, 0, 4, "%04d", F(lastcmd));
wmov(0,0);
refr();
return getkb();
}
return 0;
}

