#include <string.h>
#include "version.h"
#include "qdata.h"
#include "record.h"
#include "rblock.h"
#include "rpage.h"
#include "rmap.h"
#include "block.h"
#include "screen.h"
#include "page.h"
#include "form.h"

Screen display;
Block b[NBLOCKS];
Page  p[NBLOCKS];

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
int s;

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

if (display.init()) return 6;
if (rpage.init(dbc)) return 9;
if ((s = rpage.query("name,ysiz,xsiz,vwpy0,vwpx0,border"))) return s;
if (rpage.q.rows != 1) return 7;
if (p[0].init(rpage.q, 0));
if (p[1].init(rpage.q, 1)) return 9;
rpage.close();
if (rmap.init(dbc, 1)) return 9;
if ((s = rmap.query("line,mtext"))) return s;
if (p[1].maps(&rmap)) return 9;
rmap.close();

return 0;
}

int Form::run() {
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
