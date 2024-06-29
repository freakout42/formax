#include "runform.h"
#include "colquery/colquery.h"

int Function::dispatch() {
int s;
int run;
run = 0;
switch(f.lastkey) {
 case 0:                               // startup
  f.rmode = MOD_QUERY;
  break;

 case KEY_HOME:
  f.p[0].getst(0, 0, 80, 0, f.rmode==MOD_QUERY ? f.l[f.curfield].qhuman : f.l[f.curfield].name, 0, "", 80, NULL);
  s = colquery(f.l[f.curfield].qhuman, f.l[f.curfield].qwhere, f.l[f.curfield].name, 0, 0);
  f.p[0].getst(0, 0, 80, 0, f.l[f.curfield].qwhere, 0, "", 80, NULL);
  break;

 case KEY_LEFT:   fmove(0, -1); break;

 case KEY_RIGHT:  fmove(0, 1);  break;

 case KEY_F(8):                        // commit and exit
  if (!f.dirty) f.p[0].message(40401);
  run = 1;
  break;

 case KEY_F(9):                        // rollback and exit
  if (f.dirty) f.p[0].message(40401);
  run = 1;
  break;

 default: ;
}
f.p[1].refr();
return run;
}

void Function::fmove(int bi, int fi) {
// totally broken with multi-block
f.curblock = (f.curblock + f.numblock + bi) % f.numblock;
f.curfield = (f.curfield + f.numfield + fi) % f.numfield;
}

#ifdef nonono
int Screen::getst(int y, int x, int width, int att, char *s, int pos, char *legal, int max, int *chg) {
int done;           /* end-of-loop flag */
int changed;        /* changed the string */
int first;          /* first input flag */
int c;              /* input key */
int sx;             /* current position x */
int len;            /* currrent string len  */
char *tp;           /* position of tab */
char se[MEDSIZE];   /* my copy of string  */
char *so;           /* position in string */
char tmp[MEDSIZE];  /* output string  */
int endx;           /* end position   */
#endif
