// form configuration
class Form: public Record {
public:
  char *id;
  char *name;
  char *title;
  rError rerror;
  rBlock rblock;
  rPage rpage;
  rMap rmap;
  Qdata *e;
  Screen d;
  Block b[NBLOCKS];
  Page  p[NBLOCKS];
  int numblock;
  int numpage;
  int curblock;
  int curfield;
  int lastkey;
  int dirty;
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
