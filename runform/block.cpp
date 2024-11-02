/* Top objects of any form
 * Describe each section or subsection of the form,
 * and serve as the basis of default database interaction.
 * Corresponds to a single table.
 */
#include "runform.h"

int Block::init(Qdata *blk, int rix) {
block_id =  blk->n(rix, 1);
let(table,  blk->v(rix, 2));
sequence =  blk->n(rix, 3);
norec =     blk->n(rix, 4);
let(prikey, blk->v(rix, 5));
let(where,  blk->v(rix, 6));
let(order,  blk->v(rix, 7));
let(attrs,  "");
let(searchre, "regex");
fieldcount = 0;
prikeycnt = 0;
empty(condition);
index = rix - 1;
return 0;
}

/* the fields correspond to the columns of the table
 * we keep lists for the fields in the block => blockfields
 */
int Block::addattribute(int att, void *fld) {
Field *l;
l = (Field*)fld;
if (*attrs) cats(t(attrs), ","); /* build the column list for query */
cats(t(attrs), l->basetable ? l->column : l->defaultval);
blockfields[fieldcount++] = att;
if (l->isprimarykey) primarykeys[prikeycnt++] = att;
columni = fieldcount;
return fieldcount;
}

/* orm retrieve respects the colquery conditions */
int Block::select() {
int i;
char sep[7];
empty(condition);
empty(sep);
for (i=0; i<fieldcount; i++)
  if (F(l)[blockfields[i]].querywhere[0]) {
    cats(t(condition), sep);
    strcpy(sep, " AND ");
    cats(t(condition), "(");
    cats(t(condition), F(l[blockfields[i]]).querywhere);
    cats(t(condition), ")");
  }
return query();
}

/* block field column */
char *Block::cn(int c) {
return F(l[blockfields[c]]).column;
}

/* orm update by bind variables - disabled usebindvar=FALSE completely */
int Block::update(int r, int c) {
#ifdef USEBINDVARFALSEENABLED
if (usebindvar) {
#endif
letf((char*)querystr, MEDSIZE, "update %s set %s = ? where %s = ?", table, cn(c-1), F(l[primarykeys[0]]).column);
bindv[0] = q->v(r, c);
bindv[1] = q->v(r, F(l[primarykeys[0]]).sequencenum);
bindv[2] = NULL;
#ifdef USEBINDVARFALSEENABLED
} else {
letf((char*)querystr, MEDSIZE, "update %s set %s = '%s' where %s = '%s'", table, cn(c-1), q->v(r, c),
                                           F(l[primarykeys[0]]).column, q->v(r, F(l[primarykeys[0]]).sequencenum));
bindv[0] = NULL;
}
#endif
if ((ret = execute())) return ret;
return complete();
}

int Block::destroy(int r) {
#ifdef USEBINDVARFALSEENABLED
if (usebindvar) {
#endif
letf((char*)querystr, MEDSIZE, "delete from %s where %s = ?", table, F(l[primarykeys[0]]).column);
bindv[0] = q->v(r, F(l[primarykeys[0]]).sequencenum);
bindv[1] = NULL;
#ifdef USEBINDVARFALSEENABLED
} else {
letf((char*)querystr, MEDSIZE,
  "delete from %s where %s = '%s'", table, F(l)[primarykeys[0]].column, q->v(r, F(l)[primarykeys[0]].sequencenum));
bindv[0] = NULL;
}
#endif
if ((ret = execute())) return ret;
return complete();
}

/* fill the empty record with default values */
void Block::filldefault(int r) {
int i;
Field *fld;
char *querytoken;
int isdistinct;
for (i=0; i<fieldcount; i++) {
  fld = &fldi(i);
  if (!fld->isprimarykey) {
    if (*fld->defaultval) *q->w(r, i+1) = strdup(fld->defaultval);
    isdistinct = 0;
    for (querytoken=fld->queryhuman; *querytoken; querytoken++)
      if (!(isdistinct = isalnum(*querytoken))) break;
    if (isdistinct)       *q->w(r, i+1) = strdup(fld->queryhuman);
  }
} }

/* bind the given fields and insert by sql returning the new row when supported */
int Block::insert(int r) {
int i, j;
char columnslist[MEDSIZE];
char valueslist[SMLSIZE];
char sep;
empty(columnslist);
empty(valueslist);
sep = '\0';
j = 0;
for (i=0; i<fieldcount; i++) {
  if (fldi(i).basetable && q->v(r, i+1)) {
    catc(t(columnslist), sep);
    cats(t(columnslist), F(l[blockfields[i]]).column);
    catc(t(valueslist),  sep);
#ifdef USEBINDVARFALSEENABLED
if (usebindvar) {
#endif
    cats(t(valueslist),  "?");
    bindv[j++] = q->v(r, i+1);
#ifdef USEBINDVARFALSEENABLED
} else {
    letf(t(valueslist),  "'%s'", q->v(r, i+1));
}
#endif
    sep = ',';
  }
}
bindv[j] = NULL;
if (drv == ODR_ORACLE || drv == ODR_ADS || (drv == ODR_SQLITE && sqlitevernumber < 3035000)) {
//{CALL begin insert into %s (%s) values (%s) return %s into ?", table, columnslist, valueslist, }
  letf((char*)querystr, MEDSIZE, "insert into %s (%s) values (%s)", table, columnslist, valueslist);
  if ((ret = execute())) return ret;
} else {
  letf((char*)querystr, MEDSIZE, "insert into %s (%s) values (%s) returning %s", table, columnslist, valueslist, attrs);
  if ((ret = execute())) return ret;
  if ((ret = fetch(r))) MSG(MSG_NOREC);
}
return complete();
}

