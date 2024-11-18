#ifndef BEGINSQL
/* form configuration */
class Form: public Record {
public:
  Form();
  char id[8];           /* form_id currently always ==1 */
  char name[TNYSIZE];   /* not used yet */
  char title[TNYSIZE];  /* not used yet */
  Qdata *e;             /* error messages sql return in memory */
  int needredraw;       /* screen redraw flag */
  Block   b[NBLOCKS];   /* in memory block array */
  Field   l[NFIELDS];   /* in memory field array */
  Page    p[NBLOCKS];   /* in memory page array */
  Trigger r[NTRIGGERS]; /* in memory trigger array */
  rMap    rmap;         /* maps table is open */
  int numblock;         /* number of blocks */
  int numfield;         /* number of fields */
  int numpage;          /* number of pages */
  int numtrigger;       /* number of triggers */
  int curblock;         /* current block index */
  int curfield;         /* current field index */
  int lastcmd;          /* current key pressed */
  int lastkey;          /* last key to copy to next key from function */
  int dirty;            /* block is dirty */
  int fill(int id);     /* fill the form from configuration database */
  int qfield(char *sel); /* search for field by selector */
  void clear();         /* clear the form */
  int run();            /* run the form */
  int mapkey(int ckey); /* map curses keys to application key codes */
private:
  char signt[SMLSIZE];  /* crypto signature */
  rError rerror;        /* errors table */
  rBlock rblock;        /* blocks table */
  rField rfield;        /* fields table */
  rPage rpage;          /* pages table */
  rTrigger rtrigger;    /* triggers table */
};
#else
CREATE TABLE forms
  (id        INTEGER PRIMARY KEY NOT NULL,
   name      TEXT    NOT NULL DEFAULT 'form0',
   title     TEXT    NOT NULL DEFAULT '',
   mnunam    TEXT    NOT NULL DEFAULT '',
                        /* is used for crypto signature */
   mnugrp    TEXT    NOT NULL DEFAULT 'YyformaxyYABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklYnopTrstuvwxyzii23U56789+/YyformaxyY'
  );
#endif
