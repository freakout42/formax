#ifndef BEGINSQL
/* page configuration */
class rPage: public Record {
public:
  int init(int form_id);
private:
//  char *name;
};
#else
CREATE TABLE pages
  (id        INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
   form_id   INTEGER NOT NULL DEFAULT 1,
   name      TEXT    NOT NULL DEFAULT 'page0',
   seq       INTEGER NOT NULL DEFAULT 1,
   title     TEXT    NOT NULL DEFAULT '',
   ysiz      INTEGER NOT NULL DEFAULT 23,   /* heigth of page */
   xsiz      INTEGER NOT NULL DEFAULT 80,   /* width of page */
   vwpy0     INTEGER NOT NULL DEFAULT 1,    /* y coordinate of view position on page */
   vwpx0     INTEGER NOT NULL DEFAULT 0,    /* x coordinate of view position on page */
   popup     INTEGER NOT NULL DEFAULT 0,    /* if the page is a pop-up page */
   border    INTEGER NOT NULL DEFAULT 1,    /* if the view has a border */
   dislv     INTEGER NOT NULL DEFAULT 0     /* if the page dissolves when exited */
  );
INSERT INTO pages  (form_id, seq, name, ysiz, vwpy0, border) VALUES                     (0, 0, 'status', 1, 0, 0);
INSERT INTO pages  (form_id, seq, name, ysiz, xsiz, vwpy0, vwpx0, popup, border) VALUES (0, 2, 'keyhelp', 16, 41, 2, 30, 1, 1);
INSERT INTO pages  (form_id, seq, name, ysiz, xsiz, vwpy0, vwpx0, popup, border) VALUES (0, 3, 'editor',  21, 65, 2, 14, 1, 1);
INSERT INTO pages  (form_id, seq, name, ysiz, xsiz, vwpy0, vwpx0, popup, border) VALUES (0, 4, 'about',   20, 41, 2, 30, 1, 1);
#endif
