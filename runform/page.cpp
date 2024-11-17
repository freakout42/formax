/* page configuration */
#include "runform.h"

int Page::init(Qdata *pag, int rix) {
memset(map, 0, sizeof(map));
page_id = pag->n(rix, 2);
let(name, pag->v(rix, 3));
ysiz    = pag->n(rix, 4);
xsiz    = pag->n(rix, 5);
vwpy0   = pag->n(rix, 6);
vwpx0   = pag->n(rix, 7);
popup   = pag->n(rix, 8);
border  = pag->n(rix, 9);
index   = rix - 1;
return 0;
}

/* read the boilerplate from map and clean the field placeholders */
int Page::maps(Qdata *qma) {
int i, r, y;
char *t, *p;
char a[NORSIZE];
if (page_id == PGE_ABOUT) {
  r = 0;
#define ABOUTLINE map[r++] = strdup(a);
  letf(t(a), "%s  [%d]", COMPANY, globalpkid);                       ABOUTLINE
  letf(t(a), "https://formax.freakout.de v%s %d", VERSION, VERMSGS); ABOUTLINE
  letf(t(a), "charset:    %s", CHARSET);                             ABOUTLINE
  letf(t(a), "compiler:   %s (%d)", CCOMPILER, (int)sizeof(Form));   ABOUTLINE
  letf(t(a), "compiled:   %s %5.5s", __DATE__, __TIME__);            ABOUTLINE
  letf(t(a), "sqlite inc: %s", sqliteversion);                       ABOUTLINE
  letf(t(a), "sqlite run: %s %d", sqliterun, sqlitevernumber);       ABOUTLINE
  letf(t(a), "odbc inc:   %s", odbcversion+2);                       ABOUTLINE
  letf(t(a), "odbc lib:   %s", odbcrun);                             ABOUTLINE
  letf(t(a), "form odbc:  %s", dbconn[0].driver_odbc_ver);           ABOUTLINE
  letf(t(a), "form drvr:  %s", dbconn[0].driver_ver);                ABOUTLINE
  letf(t(a), "form dver:  %s", dbconn[0].dbmsver);                   ABOUTLINE
  letf(t(a), "data base:  %s", dbconn[1].dbmsname);                  ABOUTLINE
  letf(t(a), "data dver:  %s", dbconn[1].dbmsver);                   ABOUTLINE
  letf(t(a), "data odbc:  %s", dbconn[1].driver_odbc_ver);           ABOUTLINE
  letf(t(a), "data drvr:  %s", dbconn[1].driver_ver);                ABOUTLINE
  letf(t(a), "curses inc: %s", cursesversion);                       ABOUTLINE
  letf(t(a), "curses run: %s", cursesrun);                           ABOUTLINE
} else {
for (i = 1; i <= qma->rows; i++) {
  r = qma->n(i, 1);
  if (r > NLINES) return 1;
  map[r] = qma->c(i, 2);
  y = 1;
  p = NULL;
  // white the $nn_ pos markers
  for (t=map[r]; y; t++) {
    if (*t == '$') {
      p = t;
    } else {
      if (p && (!(*t == '_' || *t == '.' || (*t >= '0' && *t <= '9')))) {
        if (t - p > 1) while (p < t) *p++ = ' ';
        p = NULL;
      }
    }
    if (!(*t)) y = 0;
  }
} }
return 0;
}

/* associate the page with a curses window and write the boilerplate */
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

void Page::repaint()   { if (!popup) redraw(); }
void Page::refrnopop() { if (!popup) if (!macropointer || watchmacro) refr(); }

/* display popup page and close after key pressed */
int Page::showpopup() {
int i;
redraw();
refr();
i = getkb();
F(needredraw) = 1;
return i==KEY_ENTER ? 0 : i;
}

#ifndef NOUSEDITOR
/* edit a multiline buffer with the full screen editor
 * truncate carriage return from single line content
 */
int Page::editbuf(char *buf) {
char cr[2] = "\n";
char *tmpf;
char *eol;
int i;
int s;
tmpf = tmpcreat();
tmput(buf);
if (!strchr(buf, '\n')) tmput(cr);
tmpclose(0);
s = editfile(tmpf);
tmpopen();
i = tmpread(buf, BIGSIZE);
buf[i] = '\0';
if ((eol = strchr(buf, '\n')) && *(eol+1) == '\0') empty(eol);
tmpclose(1);
return s ? KEF_NXTFLD : KEF_CANCEL;
}

/* edit a file */
int Page::editfile(char *pth) {
redraw();
refr();
F(needredraw) = 1;
return mainloop(pth, wndw);
}

/* edit a map with the full screen editor */
int Page::editmap(int pid) {
char *tmpf;
int hasborder;
hasborder = 1; /* only real pages can not have a border and start with =0 */
redraw();
refr();
if (pid < NBLOCKS) {
  hasborder = F(p)[pid].border * 2;
  pid = F(p)[pid].page_id;
}
tmpf = F(rmap).extract(pid);
F(rmap).slurp(mainloop(tmpf, wndw) ? pid : 0, tmpf, hasborder);
F(needredraw) = 1;
return 0;
}
#endif

/* update the status line and the fields content
 * clear closed popups and refresh the screen
 */
static const char *rmodes[] = RMODENAMES;
int Page::wait() {
int i;
char commit[16];
switch (CM) {
 case MOD_QUERY:  strcpy(commit,            "Execute-Query");                    break;
 case MOD_UPDATE: strcpy(commit,            "  Enter-Query");                    break;
 case MOD_INSERT: strcpy(commit, F(dirty) ? "Insert-Record" : " Clear-Record" ); break;
 case MOD_DELETE: strcpy(commit,            "Delete-Record");                    break;
}
wera();
writef(0,  2, 0, 2,  "%2s-",      F(id));
writes(0,  5,                     F(name));
writef(0, 16, 0, 8,  "%s",        username);
writef(0, 25, 0, 8,  "%s",        CB.table);
writef(0, 33, 0, 8,  "%s",        CF.column);
writef(0, 42, 0,13,  "%6d/%6d",   CR, CN);
writef(0, 56, COL_HEADER,6,"%s",  rmodes[CM]);
writef(0, 63, COL_HEADER,3,"%s",  (char*)(insertmode ? "Ins" : "Rep"));
if (watchmacro) writef(0, 67, 0,   13,"%04o %5d", lastgetch, lastgetch);
else            writef(0, 67, COL_COMMIT,13,"%s", commit);
if (!macropointer || watchmacro) refr();
forall(field) F(l)[i].show();
for (i=PGE_MAIN; i<F(numpage); i++) {
  if (F(needredraw)) F(p)[i].repaint();
  F(p)[i].refrnopop();
}
if (macropointer && watchmacro) sleep(1);
F(needredraw) = 0;
return LK ? LK : getkb();
}

void Page::working() {
writef(0, 67, COL_BUSY,13,"%s", "!! Working...");
if (!macropointer) refr();
}

/* display message in status line and wait for key pressed */
int Page::message(int ern, const char *pnt) {
int i;
static char empty[] = "";
const char *pntst;
if (!intrigger && !macropointer && !screenclos) { /* not in trigger and display has open window */
if (pnt) pntst = pnt; else pntst = empty;
i = (strlen(pntst) > LINE0SIZE-9) ? strlen(pntst) - LINE0SIZE + 9 : 0;
writef(0, 0, 0, LINE0SIZE, "MAX-%03d %s %s", ern, msg(ern), pntst+i);
//writef(0, 76, 0, 4, "%04d", CK);
wmov(0,0);
refr();
return (i = getkb())==KEY_ESC ? 0 : i;
}
return 0;
}

