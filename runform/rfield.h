#ifndef BEGINSQL
// field configuration
class rField: public Record {
public:
  int init(int form_id);
private:
  char *name;
};
#else
CREATE TABLE fields
  (id        INTEGER PRIMARY KEY NOT NULL,
   form_id   INTEGER NOT NULL DEFAULT 1,
   blcn      INTEGER NOT NULL DEFAULT 1,
   pgen      INTEGER NOT NULL DEFAULT 1,
   name      TEXT    NOT NULL DEFAULT 'field0',
   seq       INTEGER NOT NULL DEFAULT 1,
   ftype     INTEGER NOT NULL DEFAULT 0,    /* 0:ALL 1:CHAR 2:INT 3:FLOAT 4:DATE */
   len       INTEGER NOT NULL DEFAULT 30,   /* field length */
   dlen      INTEGER NOT NULL DEFAULT 20,   /* display length =0 no display */
   btab      INTEGER NOT NULL DEFAULT 1,    /* if the field is a base table field */
   key       INTEGER NOT NULL DEFAULT 0,    /* if the field is a primary key field */
   dflt      TEXT    NOT NULL DEFAULT '',   /* default value */
   line      INTEGER NOT NULL DEFAULT 0,    /* page location */
   col       INTEGER NOT NULL DEFAULT 20,   /* column location */
   enter     INTEGER NOT NULL DEFAULT 1,    /* if the field is enterable */
   query     INTEGER NOT NULL DEFAULT 1,    /* if the field can be queried */
   upd       INTEGER NOT NULL DEFAULT 1,    /* if the field can be updated */
   updnul    INTEGER NOT NULL DEFAULT 1,    /* if the field can be updated when NULL */
   mand      INTEGER NOT NULL DEFAULT 0,    /* if the field is requeried */
   upper     INTEGER NOT NULL DEFAULT 0,    /* if the field converts to uppercase */
   lovtit    TEXT    NOT NULL DEFAULT '',   /* title for list of values */
   lov_id    INTEGER NOT NULL DEFAULT 0,    /* block for list of values */
   lovi_id   INTEGER NOT NULL DEFAULT 0,    /* page for list of values */
   low       INTEGER NOT NULL DEFAULT -99999999999, /* low range value */
   high      INTEGER NOT NULL DEFAULT 99999999999,  /* high range value */
   valpatn   TEXT    NOT NULL DEFAULT '',   /* validation regex pattern */
   help      TEXT    NOT NULL DEFAULT ''    /* help text */
  );
#endif
