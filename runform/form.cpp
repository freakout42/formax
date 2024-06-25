#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "version.h"

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

if (rblock.init(dbc)) return 9;
if ((s = rblock.query("name"))) return s;
if (rblock.q.rows != 1) return 7;
if (b[0].init(rblock.q, 1));
rblock.close();

if (rpage.init(dbc)) return 9;
if ((s = rpage.query("name,ysiz,xsiz,vwpy0,vwpx0,border"))) return s;
if (rpage.q.rows > NBLOCKS) return 7;
for (i=0; i<rpage.q.rows; i++) if (p[i].init(rpage.q, i+1)) return 9;
rpage.close();
if (rmap.init(dbc, 1)) return 9;
if ((s = rmap.query("line,mtext"))) return s;
if (p[1].maps(&rmap)) return 9;
rmap.close();

return 0;
}

int Form::run() {
Screen display;
fprintf(stderr,"%d\n", p[0].ysiz);
if (display.init()) return 6;
p[0].create();
p[1].create();
p[0].writes(STATUSL, 2,                 title);
p[0].writef(STATUSL, 12, 0, 4,  "%3s-", id);
p[0].writes(STATUSL, 16,                name);
p[0].writes(STATUSL, 24,                b[0].name);
p[0].writes(STATUSL, 60,                p[1].name);
p[0].writes(STATUSL, 67,                "runform-");
p[0].writes(STATUSL, 75,                (char*)VERSION);
display.refresh();
p[1].refresh();
p[0].refresh();
move(0,0);
display.getkey();
display.close();
close();
return 0;
}
