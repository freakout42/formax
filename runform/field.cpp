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
if (strcmp(fld->v(rix, 2), "user"))
let(column,     fld->v(rix, 2));
else
let(column,     "\"user\"");
blockindex    = fld->n(rix, 3);
pageindex     = fld->n(rix, 4);
displaylen    = fld->n(rix, 5);
alignment     = fld->n(rix, 6);
line          = fld->n(rix, 7) + (page.border ? 1 : 0);
col           = fld->n(rix, 8) + (page.border ? 1 : 0);
isprimarykey  = fld->n(rix, 9);
fieldtype     = (ftype)fld->n(rix,10);
fieldlen      = fld->n(rix,11);
decimalen     = fld->n(rix,12);
basetable     = fld->n(rix,13);
defaultval    = fld->c(rix,14);
queryable     = fld->n(rix,15);
updateable    = fld->n(rix,16);
updnulable    = fld->n(rix,17);
mandatory     = fld->n(rix,18);
let(lovtitle,   fld->v(rix,19));
lov_id        = fld->n(rix,20);
lovi_id       = fld->n(rix,21);
lowvalue      = fld->n(rix,22);
highvalue     = fld->n(rix,23);
let(validreg,   fld->v(rix,24));
helptext      = fld->c(rix,25);
field_id      = fld->n(rix,26);
empty(queryhuman);
empty(querywhere);
currentval = NULL;
trg_postchange = -1;
sequencenum = bs[blockindex].addattribute(rix-1, this);
index = rix - 1;
return 0;
}

void Field::rclose() {
  free(currentval);
  free(defaultval);
  free(helptext);
}

/* check whether field is editable in current mode */
int Field::noedit() {
switch(CM) {
 case MOD_UPDATE: if (isprimarykey || !(updateable || (updnulable && *valuep()==NULL))) return 1; break;
 case MOD_QUERY:  if (!queryable)                                                       return 1; break;
 case MOD_INSERT: if (!updateable && !updnulable)                                       return 1; break;
 case MOD_DELETE:                                                                                 break;
}
return 0;
}

/* high level field type adds boolean for special treatment */
ftype Field::fldtype() {
return (fieldtype==FTY_INT && lowvalue==0 && highvalue==1) ? FTY_BOOL : fieldtype;
}

/* display the field according to mode */
void Field::show() {
int cur, color, outline, outrec;
const char *outcell;
if (CP.index == pageindex && displaylen > 0)
  for (outline = line; outline < line + block.norec; outline++) {
    outcell = NULL;
    outrec = block.toprec + outline - line;
    switch(block.rmode) {
     case MOD_QUERY:  outcell = outline==line ? queryhuman : "";
                      color = COL_QUERY;   break;
     case MOD_INSERT: color = COL_NEWREC;  break;
     case MOD_DELETE: color = COL_DELETED; break;
     case MOD_UPDATE: color = COL_FIELD;   break;
     default:         color = COL_DATA;
    }
    cur = index == CFi;
    cur = cur && (outcell ? outline == line : outrec == block.currentrec);
    if (cur) color = COL_CURRENT;
    if (block.index != CB.index || (block.rmode != MOD_QUERY && outrec != block.currentrec)) color = COL_DATA;
    if (!outcell) outcell = outrec <= block.q->rows ? *valuep(outrec) : "";
   if (outcell && *outcell && fieldtype == FTY_FLOAT)
    page.writef(outline, col, color, displaylen, "%*.*f", displaylen, decimalen, atof(outcell));
   else
    page.writef(outline, col, color, displaylen, "%*.*s", alignment?displaylen:0, displaylen, outcell);
    if (cur) page.wmov(outline, col);
  }
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
return valuep(block.currentrec);
}

/* field value any row */
char **Field::valuep(int row) {
//static char *emptystring = "";
static char *null0 = NULL;
static char **val;
val = block.q ? (row ? block.q->w(row, sequencenum) : &null0) : &null0;
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
  if (noedit() && pos > FED_SPECIAL) { MSG(MSG_FLDPROT); return KEF_CANCEL; }
  if (block.q->rows) {
    c = valuep();
    if (*c) let(a, *c); else empty(a);
    switch(pos) {
     case FED_FEDITOR: pressed = F(p)[PGE_EDITOR].editbuf(a); break;
     case FED_TRIGGER: pressed = u.edittrg(a);                break;
     case FED_TOGGLE:  pressed = toggle(a);                   break;
     case FED_INCR:    pressed = increment(a, 1);             break;
     case FED_DECR:    pressed = increment(a, -1);            break;
     default:        if (displaylen == fieldlen)
                       pressed = F(p)[pageindex].sedit(a, pos, fldtype(), fieldlen, line+CR-CB.toprec, col);
                     else
                       pressed = F(p)[PGE_STATUS].sedit(a, pos, fldtype(), fieldlen);
    }
    if (pressed != KEF_CANCEL && validate(c, a) == KEF_CANCEL) pressed = KEF_CANCEL;
  }
  break;
 case MOD_QUERY:
  let(a, queryhuman);
  pressed = F(p)[PGE_STATUS].sedit(a, pos<FED_SPECIAL ? -1 : pos, FTY_ALL, SMLSIZE);
  setcond(a);
  break;
 case MOD_DELETE:
  break;
}
pressed = pressed==KEY_ENTER ? KEF_NXTFLD : F(mapkey)(pressed);
return pressed;
}

/* set the query condition in human form */
void Field::setcond(char *cond) {
let(queryhuman, cond);
colquery(queryhuman, querywhere, column, querycharm, 0);
}

