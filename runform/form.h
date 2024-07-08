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
  int notrunning;
  int *init();
  int fill(int id);
  void clear();
  int run();
  int mapkey(int ckey);
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
