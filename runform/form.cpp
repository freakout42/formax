#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "runform.h"

int *Form::init() {
stmt = NULL;
if (open()) return NULL;
let(table,  "forms");
let(prikey, "id");
let(where,  "");
let(order,  "id");
return 0;
}

int Form::fill(int fid) {
int i, s;

letf(t(where), "id = %d", fid);
if ((s = query("id,name,title"))) return s;
if (q.rows != 1) return 7;
id    = q.c(1, 1);
name  = q.c(1, 2);
title = q.c(1, 3);
close();

if (rerror.init()) return 9;
if ((s = rerror.query("num,severity,etext"))) return s;
e = rerror.q;
rerror.q.init();
rerror.close();

if (rblock.init()) return 9;
if ((s = rblock.query("name,prikey,whereand,orderby"))) return s;
numblock = rblock.q.rows;
if (numblock > NBLOCKS) return 7;
for (i=0; i<numblock; i++) {
  if (b[i].open()) return 9;
  if (b[i].init(rblock.q, i+1)) return 9;
}
rblock.close();
curblock = 1;

if (rpage.init()) return 9;
if ((s = rpage.query("name,ysiz,xsiz,vwpy0,vwpx0,border"))) return s;
numpage = rpage.q.rows;
if (numpage > NBLOCKS) return 7;
for (i=0; i<numpage; i++) if (p[i].init(rpage.q, i+1)) return 9;
rpage.close();
if (rmap.init(1)) return 9;
if ((s = rmap.query("line,mtext"))) return s;
if (p[1].maps(&rmap)) return 9;
rmap.close();
return 0;
}

void Form::clear() {
int i;
for (i=0; i<numblock; i++) b[i].close();
}

int Form::run() {
int i;
Function u;
if (d.init()) return 6;
for (i=0; i<numpage; i++) p[i].create();
d.refresh();
dirty = lastkey = 0;
while (u.dispatch()) lastkey = p[0].wait();
d.close();
return 0;
}