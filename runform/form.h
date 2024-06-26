enum { MOD_INSERT, MOD_QUERY, MOD_UPDATE, MOD_DELETE };

// form configuration
class Form: public Record {
public:
  char id[SMLSIZE];
  char name[SMLSIZE];
  char title[SMLSIZE];
  rError rerror;
  rBlock rblock;
  rField rfield;
  rPage rpage;
  rMap rmap;
  Qdata *e;
  Screen d;
  Block b[NBLOCKS];
  Field l[NFIELDS];
  Page  p[NBLOCKS];
  int numblock;
  int numfield;
  int numpage;
  int curblock;
  int curfield;
  int lastkey;
  int dirty;
  int rmode;
  int *init();
  int fill(int id);
  void clear();
  int run();
private:
};

#ifdef BEGINSQL
CREATE TABLE forms
  (id        INTEGER PRIMARY KEY NOT NULL,
   name      TEXT    NOT NULL DEFAULT 'form0',
   title     TEXT    NOT NULL DEFAULT '',
   mnunam    TEXT    NOT NULL DEFAULT '',
   mnugrp    TEXT    NOT NULL DEFAULT ''
  );
#endif
