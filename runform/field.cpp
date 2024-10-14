/* workhorse object most of the action is here
 * Represent columns or data entry areas and describe
 * how the data should be displayed and validated
 * and how an operator should interact with the data while it is entered
 */
#include "runform.h"
#include "regex/re.h"
#include "colquery/colquery.h"

#define page  F(p)[pageindex]
#define block F(b)[blockindex]

int Field::init(Qdata *fld, int rix, Block *bs) {
field_id      = fld->n(rix, 1);
let(name,       fld->v(rix, 2));
blockindex    = fld->n(rix, 3);
pageindex     = fld->n(rix, 4);
displaylen    = fld->n(rix, 5);
line          = fld->n(rix, 6) + (page.border ? 1 : 0);
col           = fld->n(rix, 7) + (page.border ? 1 : 0);
isprimarykey  = fld->n(rix, 8);
fieldtype     = (ftype)fld->n(rix, 9);
fieldlen      = fld->n(rix,10);
basetable     = fld->n(rix,11);
let(defaultval, fld->v(rix,12));
enterable     = fld->n(rix,13);
queryable     = fld->n(rix,14);
updateable    = fld->n(rix,15);
updnulable    = fld->n(rix,16);
mandatory     = fld->n(rix,17);
uppercase     = fld->n(rix,18);
let(lovtitle,   fld->v(rix,19));
lov_id        = fld->n(rix,20);
lovi_id       = fld->n(rix,21);
lowvalue      = fld->n(rix,22);
highvalue     = fld->n(rix,23);
let(validreg,   fld->v(rix,24));
let(helptext,   fld->v(rix,25));
field_id      = fld->n(rix,26);
let(queryhuman, "");
let(querywhere, "");
sequencenum = bs[blockindex].addattribute(rix-1, this);
index = rix - 1;
return 0;
}

/* check whether field is editable in current mode */
int Field::noedit() {
switch(CM) {
 case MOD_UPDATE: if (isprimarykey || !(updateable || (updnulable && *valuep()==NULL))) return 1; break;
 case MOD_QUERY:  if (!queryable)                                                       return 1; break;
 case MOD_INSERT: if (!updateable)                                                      return 1; break;
 case MOD_DELETE:                                                                                 break;
}
return 0;
}

/* high level field type adds boolean for special treatment */
ftype Field::fldtype() {
return (fieldtype==FTY_INT && lowvalue==0 && highvalue==1) ? FTY_BOOL : fieldtype;
}

/* display the field according to mode */
void Field::show(int cur) {
int color;
switch(block.rmode) {
 case MOD_QUERY:  color = COL_QUERY;  break;
 case MOD_INSERT: color = COL_NEWREC; break;
 case MOD_DELETE: color = COL_DELETED; break;
 default:         color = COL_FIELD;
}
if (cur && block.rmode != MOD_DELETE) color = COL_CURRENT;
if (displaylen > 0) page.writef(line, col, color, displaylen, "%.*s", displaylen, block.rmode==MOD_QUERY ? queryhuman : *valuep());
if (cur) page.wmov(line, col);
}

/* clear field content */
void Field::clear() {
char **v;
if (CM == MOD_QUERY) {
  empty(queryhuman);
  empty(querywhere);
} else {
  v = valuep();
  free(v);
  v = NULL;
} }

/* the current field value */
char **Field::valuep() {
return valuep(block.currentrecord);
}

/* field value any row */
char **Field::valuep(int row) {
//static char *emptystring = "";
static char **val;
val = block.q->w(row, sequencenum);
//if (!val || !(*val)) val = &emptystring;
return val;
}

/* toggle boolean field value between 0 and 1 */
int Field::toggle(char *val) {
if (CM == MOD_UPDATE && fldtype() == FTY_BOOL) {
  if (strlen(val)==1) {
    switch (*val) {
     case '0': *val = '1'; return KEF_NXTFLD;
     case '1': *val = '0'; return KEF_NXTFLD;
    }
  }
}
return KEF_CANCEL;
}

/* increment/decrement integer field value */
int Field::increment(char *val, int ival) {
int letvalue;
if (CM == MOD_UPDATE && fieldtype == FTY_INT) {
  if (isdigit(*val) || *val == '-') {
    letvalue = atoi(val) + ival;
    letf(val, SMLSIZE, "%d", letvalue);
    return 0; // KEF_NXTFLD;
  }
}
return KEF_CANCEL;
}

/* checks new field value with validation rules */
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
if (*c) {
  if (strlen(buf) > strlen(*c)) *c = (char*)realloc(*c, strlen(buf)+1);
  if (strcmp(*c, buf)) strcpy(*c, buf);
} else *c = strdup(buf);
return 0;
}

/* call the field editor depending on pos and current mode
 * when changed validate the new content and return next key
 * return KEF_CANCEL on cancel | no changes | validation fail
 */
int Field::edit(int pos) {
int pressed;
char **c;
pressed = 0;
switch(CM) {
 case MOD_UPDATE:
  if (isprimarykey) { MSG(MSG_EDITKEY); return KEF_CANCEL; }
  /*FALLTHRU*/
 case MOD_INSERT:
  if (noedit() || (!enterable && pos>FED_SPECIAL)) { MSG(MSG_FLDPROT); return KEF_CANCEL; }
  if (block.q->rows) {
    c = valuep();
    if (*c) let(a, *c); else empty(a);
    switch(pos) {
     case FED_FEDITOR: pressed = F(p)[PGE_EDITOR].editbuf(a); break;
     case FED_TRIGGER: pressed = u.edittrg(a);                break;
     case FED_TOGGLE:  pressed = toggle(a);                   break;
     case FED_INCR:    pressed = increment(a, 1);             break;
     case FED_DECR:    pressed = increment(a, -1);            break;
     default:          pressed = F(p)[PGE_STATUS].sedit(a, pos, fldtype(), fieldlen);
    }
    if (pressed != KEF_CANCEL && validate(c, a) == KEF_CANCEL) pressed = KEF_CANCEL;
  }
  break;
 case MOD_QUERY:
  pressed = F(p[PGE_STATUS].sedit)(queryhuman, pos<FED_SPECIAL ? -1 : pos, FTY_ALL, SMLSIZE);
  colquery(queryhuman, querywhere, name, querycharm, 0);
  break;
 case MOD_DELETE:
  break;
}
pressed = pressed==KEY_ENTER ? KEF_NXTFLD : F(mapkey)(pressed);
return pressed;
}

