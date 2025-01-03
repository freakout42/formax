#include "rtrigger.h"
#ifndef BEGINSQL
/* map configuration and default maps for standard popup pages
 * and standard trigger code
 */
class rMap: public Record {
public:
  int init(int page_id);
  int getbody(int page_id, char *buf, int n, int acr);
  char *extract(int page_id);
  void slurp(int pid, char *tmpf, int brdr);
};
#else
CREATE TABLE maps
  (id        INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
   page_id   INTEGER NOT NULL DEFAULT 1,
   line      INTEGER NOT NULL DEFAULT 1, /* =0 name */
   mtext     TEXT    NOT NULL DEFAULT ''
  );
INSERT INTO maps (page_id, line, mtext) VALUES (2,  0, 'KEY C FUNCTION     KEY   C FUNCTION');
INSERT INTO maps (page_id, line, mtext) VALUES (2,  1, '--- - ------------ ----- - ---------');
INSERT INTO maps (page_id, line, mtext) VALUES (2,  2, 'Tab i NextField    ^Tab  g PrevField');
INSERT INTO maps (page_id, line, mtext) VALUES (2,  3, 'F1  @ Field Help   Esc   y Cancel');
INSERT INTO maps (page_id, line, mtext) VALUES (2,  4, 'F2  c Copy         >     f Right');
INSERT INTO maps (page_id, line, mtext) VALUES (2,  5, 'F3  v Paste        <     b Left');
INSERT INTO maps (page_id, line, mtext) VALUES (2,  6, 'F4  t Duplicate    Up    p PreviousRec');
INSERT INTO maps (page_id, line, mtext) VALUES (2,  7, 'F5  u ListOfVal    Down  n NextRecord');
INSERT INTO maps (page_id, line, mtext) VALUES (2,  8, 'F6  o Insert       Ins   j Insert');
INSERT INTO maps (page_id, line, mtext) VALUES (2,  9, 'F7  d Delete       Del   d Delete');
INSERT INTO maps (page_id, line, mtext) VALUES (2, 10, 'F8  z Exit         <--   h DeleteLast');
INSERT INTO maps (page_id, line, mtext) VALUES (2, 11, 'F9  y Cancel       PgUp  r PrevSetRec');
INSERT INTO maps (page_id, line, mtext) VALUES (2, 12, 'F10 x Query        PgDn  w NextSetRec');
INSERT INTO maps (page_id, line, mtext) VALUES (2, 13, 'F11 k Key Help     Home  a PrevBlock');
INSERT INTO maps (page_id, line, mtext) VALUES (2, 14, 'F12 l Refresh      End   e NextBlock');
INSERT INTO maps (page_id, line, mtext) VALUES (2, 15, '+     Increment    -       Decrement');
INSERT INTO maps (page_id, line, mtext) VALUES (2, 16, '>     Goto         ~       Search');
INSERT INTO maps (page_id, line, mtext) VALUES (2, 17, 'Space Toggle binary');
INSERT INTO maps (page_id, line, mtext) VALUES (TRT_COPYREC, 1, '$(cb + "." + cf, cr - 1);');
INSERT INTO maps (page_id, line, mtext) VALUES (TRT_COPY,    1, 'clip = cv;529;');
INSERT INTO maps (page_id, line, mtext) VALUES (TRT_PASTE,   1, 'clip;');
#endif
