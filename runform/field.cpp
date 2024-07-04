// workhorse object
#include <stdlib.h>
#include <string.h>
#include "runform.h"
#include "colquery/colquery.h"

int Field::init(Qdata *fld, int rix) {
let(name,  fld->v(rix, 1));
dlen =     fld->n(rix, 2);
line =     fld->n(rix, 3);
col  =     fld->n(rix, 4);
isprimarykey = fld->n(rix, 5);
let(qhuman, "");
let(qwhere, "");
blk = 1;
num = f.b[blk].addattribute(rix-1);
return 0;
}

void Field::fshow(int cur) {
int color;
if (cur) color = COL_CURRENT;
else switch(f.rmode) {
 case MOD_QUERY: color = COL_QUERY; break;
 default:        color = COL_FIELD;
}
f.p[1].writef(line, col, color, dlen, "%s", f.rmode==MOD_QUERY ? qhuman : f.b[blk].q->v(CB.bcur, num));
if (cur) f.p[1].wmov(line, col);
}

int Field::ledit(int pos) {
int s;
int pressed;
char buf[SMLSIZE];
char **c;
if (f.rmode == MOD_QUERY) {
  pressed = f.p[0].sedit(qhuman, pos);
  s = colquery(qhuman, qwhere, name, 1, 0);
} else if (f.b[blk].q->rows) {
  c = f.b[blk].q->w(CB.bcur, num);
  let(buf, *c);
  pressed = f.p[0].sedit(buf, pos);
  if (strlen(buf) > strlen(*c)) {
    *c = (char*)realloc(*c, strlen(buf)+1);
  }
  if (strcmp(*c, buf)) {
    strcpy(*c, buf);
    CB.update(CB.bcur, num);
  }
} else { // insert
}
return pressed;
}
