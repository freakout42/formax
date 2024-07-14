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
let(queryhuman, "");
let(querywhere, "");
blockindex = 1;
sequencenum = f.b[blockindex].addattribute(rix-1);
return 0;
}

void Field::show(int cur) {
int color;
if (cur) color = COL_CURRENT;
else switch(f.rmode) {
 case MOD_QUERY:  color = COL_QUERY;  break;
 case MOD_INSERT: color = COL_NEWREC; break;
 default:         color = COL_FIELD;
}
f.p[1].writef(line, col, color, dlen, "%s", f.rmode==MOD_QUERY ? queryhuman : f.b[blockindex].q->v(CB.currentrecord, sequencenum));
if (cur) f.p[1].wmov(line, col);
}

int Field::edit(int pos) {
int s;
int pressed;
char buf[SMLSIZE];
char **c;
pressed = 0;
switch(f.rmode) {
 case MOD_INSERT:
 case MOD_UPDATE:
  if (f.b[blockindex].q->rows) {
    c = f.b[blockindex].q->w(CB.currentrecord, sequencenum);
    if (*c) let(buf, *c); else *buf = '\0';
    pressed = f.p[0].sedit(buf, pos);
    if (*c==NULL && *buf) *c = strdup(buf);
    else {
      if (strlen(buf) > strlen(*c)) *c = (char*)realloc(*c, strlen(buf)+1);
      if (strcmp(*c, buf)) strcpy(*c, buf);
    }
  }
  break;
 case MOD_QUERY:
  pressed = f.p[0].sedit(queryhuman, pos);
  s = colquery(queryhuman, querywhere, name, 1, 0);
  break;
}
pressed = pressed==KEY_ENTER ? KEF_NXTFLD : f.mapkey(pressed);
return pressed;
}
