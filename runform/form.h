#ifndef BEGINSQL
/* form configuration */
class Form: public Record {
public:
  Form();
  char id[8];
  char name[TNYSIZE];
  char title[TNYSIZE];
  Qdata *e;
  int needredraw;
  Block   b[NBLOCKS];
  Field   l[NFIELDS];
  Page    p[NBLOCKS];
  Trigger r[NTRIGGERS];
  rMap    rmap;
  int numblock;
  int numfield;
  int numpage;
  int numtrigger;
  int curblock;
  int curfield;
  int lastcmd;
  int lastkey;
  int dirty;
  int sign(int fid, char *signature);
  int fill(int id);
  int qfield(char *sel);
  void clear();
  int run();
  int mapkey(int ckey);
  void rconnect();
private:
  char signt[SMLSIZE];
  rError rerror;
  rBlock rblock;
  rField rfield;
  rPage rpage;
  rTrigger rtrigger;
};
#else
CREATE TABLE forms
  (id        INTEGER PRIMARY KEY NOT NULL,
   name      TEXT    NOT NULL DEFAULT 'form0',
   title     TEXT    NOT NULL DEFAULT '',
   mnunam    TEXT    NOT NULL DEFAULT '',
   mnugrp    TEXT    NOT NULL DEFAULT 'YyformaxyYABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklYnopTrstuvwxyzii23U56789+/YyformaxyY'
  );
#endif
