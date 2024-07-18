#ifndef BEGINSQL
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
  Screen y;
  Block b[NBLOCKS];
  Field l[NFIELDS];
  Page  p[NBLOCKS];
  int numblock;
  int numfield;
  int numpage;
  int curblock;
  int curfield;
  int lastcmd;
  int lastkey;
  int dirty;
  enum fmode rmode;
  int *init();
  int fill(int id);
  void clear();
  int run();
  int mapkey(int ckey);
private:
};
#else
CREATE TABLE forms
  (id        INTEGER PRIMARY KEY NOT NULL,
   name      TEXT    NOT NULL DEFAULT 'form0',
   title     TEXT    NOT NULL DEFAULT '',
   mnunam    TEXT    NOT NULL DEFAULT '',
   mnugrp    TEXT    NOT NULL DEFAULT ''
  );
#endif
