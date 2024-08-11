#ifndef BEGINSQL
// page configuration
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
/* vwys      INTEGER NOT NULL DEFAULT 0,       height of view */
/* vwxs      INTEGER NOT NULL DEFAULT 0,       width of view */
   vwpy0     INTEGER NOT NULL DEFAULT 1,    /* y coordinate of view position on page */
   vwpx0     INTEGER NOT NULL DEFAULT 0,    /* x coordinate of view position on page */
   popup     INTEGER NOT NULL DEFAULT 0,    /* if the page is a pop-up page */
   border    INTEGER NOT NULL DEFAULT 1,    /* if the view has a border */
   dislv     INTEGER NOT NULL DEFAULT 0     /* if the page dissolves when exited */
  );
#endif
