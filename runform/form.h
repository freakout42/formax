#ifndef BEGINSQL
// form configuration
class Form: public Record {
public:
  Form();
  char id[SMLSIZE];
  char name[SMLSIZE];
  char title[SMLSIZE];
  Qdata *e;
  Screen y;
  Block   b[NBLOCKS];
  Field   l[NFIELDS];
  Page    p[NBLOCKS];
  Trigger r[NTRIGGERS];
  int numblock;
  int numfield;
  int numpage;
  int numtrigger;
  int curblock;
  int curfield;
  int lastcmd;
  int lastkey;
  fmode rmode;
  int fill(int id);
  void clear();
  int run();
  int mapkey(int ckey);
  void rconnect();
private:
  rError rerror;
  rBlock rblock;
  rField rfield;
  rPage rpage;
  rTrigger rtrigger;
  rMap rmap;
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
