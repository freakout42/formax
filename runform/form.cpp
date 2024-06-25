#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "runform.h"

Block *Form::init() {
stmt = NULL;
if (open(NULL)) return NULL;
let(table,  "forms");
let(prikey, "id");
let(where,  "");
let(order,  "id");
return b+0;
}

int Form::fill(int fid) {
rBlock rblock;
rPage rpage;
rMap rmap;
int i, s;

letf(t(where), "id = %d", fid);
if ((s = query("id,name,title"))) return s;
if (q.rows != 1) return 7;
id    = q.c(1, 1);
name  = q.c(1, 2);
title = q.c(1, 3);
close();

if (rblock.init(dbc)) return 9;
if ((s = rblock.query("name"))) return s;
nb = rblock.q.rows;
if (nb > NBLOCKS) return 7;
for (i=0; i<nb; i++) if (b[i].init(rblock.q, i+1)) return 9;
rblock.close();
cb = 1;

if (rpage.init(dbc)) return 9;
if ((s = rpage.query("name,ysiz,xsiz,vwpy0,vwpx0,border"))) return s;
np = rpage.q.rows;
if (np > NBLOCKS) return 7;
for (i=0; i<np; i++) if (p[i].init(rpage.q, i+1)) return 9;
rpage.close();
if (rmap.init(dbc, 1)) return 9;
if ((s = rmap.query("line,mtext"))) return s;
if (p[1].maps(&rmap)) return 9;
rmap.close();
return 0;
}

int Form::run() {
int i;
Screen d;
Function u;
if (d.init()) return 6;
for (i=0; i<np; i++) p[i].create();
d.refresh();
lk = 0;
while (u.dispatch()) lk = p[0].wait();
d.close();
return 0;
}
