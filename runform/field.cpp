/* workhorse object most of the action is here
 * Represent columns or data entry areas and describe
 * how the data should be displayed and validated
 * and how an operator should interact with the data while it is entered
 */
#include <stdlib.h>
#include <string.h>
#include "runform.h"
#include "colquery/colquery.h"

int Field::init(Qdata *fld, int rix) {
let(name,       fld->v(rix, 1));
blockindex    = fld->n(rix, 2);
pageindex     = fld->n(rix, 3);
displaylen    = fld->n(rix, 4);
line          = fld->n(rix, 5);
col           = fld->n(rix, 6);
isprimarykey  = fld->n(rix, 7);
fieldtype     = (ftype)fld->n(rix, 8);
fieldlen      = fld->n(rix, 9);
basetable     = fld->n(rix,10);
let(defaultval, fld->v(rix,11));
enterable     = fld->n(rix,12);
queryable     = fld->n(rix,13);
updateable    = fld->n(rix,14);
updnulable    = fld->n(rix,15);
mandatory     = fld->n(rix,16);
uppercase     = fld->n(rix,17);
let(lovtitle,   fld->v(rix,18));
lov_id        = fld->n(rix,19);
lovi_id       = fld->n(rix,20);
lowvalue      = fld->n(rix,21);
highvalue     = fld->n(rix,22);
let(validreg,   fld->v(rix,23));
let(helptext,   fld->v(rix,24));
field_id      = fld->n(rix,25);
let(queryhuman, "");
let(querywhere, "");
sequencenum = F(b[blockindex].addattribute)(rix-1);
return 0;
}

int Field::noedit() {
switch(CM) {
 case MOD_UPDATE: if (isprimarykey || !(updateable || (updnulable && *valuep()==NULL))) return 1; break;
 case MOD_QUERY:  if (!queryable)                                                       return 1; break;
 case MOD_INSERT: if (!enterable)                                                       return 1; break;
 case MOD_DELETE:                                                                                 break;
}
return 0;
}

ftype Field::fldtype() {
return (fieldtype==FTY_INT && lowvalue==0 && highvalue==1) ? FTY_BOOL : fieldtype;
}

void Field::show(int cur) {
int color;
switch(CM) {
 case MOD_QUERY:  color = COL_QUERY;  break;
 case MOD_INSERT: color = COL_NEWREC; break;
 case MOD_DELETE: color = COL_DELETED; break;
 default:         color = COL_FIELD;
}
if (cur && CM != MOD_DELETE) color = COL_CURRENT;
F(p[1]).writef(line, col, color, displaylen, "%.*s", displaylen, CM==MOD_QUERY ? queryhuman : *valuep());
if (cur) F(p[1]).wmov(line, col);
}

void Field::clear() {
char **v;
if (CM == MOD_QUERY) {
  *queryhuman = '\0';
  *querywhere = '\0';
} else {
  v = valuep();
  free(v);
  v = NULL;
} }

/* the current field value */
char **Field::valuep() {
return F(b[blockindex].q->w)(CB.currentrecord, sequencenum);
}

int Field::toggle() {
char **c;
if (CM == MOD_UPDATE && fldtype() == FTY_BOOL) {
  c = valuep();
  if (*c && strlen(*c)==1) {
    switch (**c) {
     case '0': **c = '1'; return KEF_NXTFLD;
     case '1': **c = '0'; return KEF_NXTFLD;
    }
  }
}
return 0;
}

int Field::increment(int ival) {
char **c;
int a;
char buf2[SMLSIZE];
if (CM == MOD_UPDATE && fieldtype == FTY_INT) {
  c = valuep();
  if (*c) {
    a = atoi(*c) + ival;
    letf(t(buf2), "%d", a);
    if (validate(c, buf2) != KEF_CANCEL) return 0; // KEF_NXTFLD;
  }
}
return 0;
}

int Field::validate(char **c, char *buf) {
char *u;
re_t re;
int s;
char buf2[SMLSIZE];
if (*validreg) {
  re = re_compile(validreg);
  if (re_matchp(re, buf, &s) == -1) { // || s != (int)strlen(buf)
    MSG1(MSG_NOMATCH, validreg);
    return KEF_CANCEL;
  }
}
switch (fieldtype) {
 case FTY_DATE:
  s = colquery(buf, buf2, "q", 0, 268);
  if (*buf2 == '{' && (u = rindex(buf2, ' ')) && u == buf2+strlen(buf2)-9) {
    strncpy(buf, u+1, 4);
    *(buf+4) = '-';
    strncpy(buf+5, u+5, 2);
    *(buf+7) = '-';
    strncpy(buf+8, u+7, 2);
    *(buf+10) = '\0';
  }
  re = re_compile("^[12]\\d\\d\\d-[012]\\d-[0123]\\d$");
  if (re_matchp(re, buf, &s) == -1) {
    MSG1(MSG_NOMATCH, "YYYY-MM-DD");
    return KEF_CANCEL;
  }
  break;
 case FTY_INT:
 case FTY_BOOL:
  if (lowvalue + highvalue != 0 && (lowvalue > atoi(buf) || highvalue < atoi(buf))) {
    letf(t(buf2), "%d - %d", lowvalue, highvalue);
    MSG1(MSG_NORANGE, buf2);
    return KEF_CANCEL;
  }
  break;
 case FTY_FLOAT: break;
 case FTY_CHAR: break;
 case FTY_ALL: break;
}
if (*c==NULL && *buf) *c = strdup(buf);
else {
  if (strlen(buf) > strlen(*c)) *c = (char*)realloc(*c, strlen(buf)+1);
  if (strcmp(*c, buf)) strcpy(*c, buf);
}
return 0;
}

int Field::edit(int pos) {
int pressed;
char buf[SMLSIZE];
char **c;
pressed = 0;
switch(CM) {
 case MOD_UPDATE:
  if (isprimarykey) { MSG(MSG_EDITKEY); return KEF_CANCEL; }
 case MOD_INSERT:
  if (noedit()) { MSG(MSG_FLDPROT); return KEF_CANCEL; }
  if (F(b[blockindex].q->rows)) {
    c = valuep();
    if (*c) let(buf, *c); else *buf = '\0';
    pressed = F(p[PGE_STATUS].sedit)(buf, pos, fldtype(), fieldlen);
    if (pressed != KEF_CANCEL && validate(c, buf) == KEF_CANCEL) pressed = KEF_CANCEL;
  }
  break;
 case MOD_QUERY:
  pressed = F(p[PGE_STATUS].sedit)(queryhuman, pos, FTY_ALL, SMLSIZE);
  colquery(queryhuman, querywhere, name, querycharm, 0);
  break;
 case MOD_DELETE:
  break;
}
pressed = pressed==KEY_ENTER ? KEF_NXTFLD : F(mapkey)(pressed);
return pressed;
}
