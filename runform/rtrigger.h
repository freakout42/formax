#ifndef BEGINSQL
// trigger configuration
class rTrigger: public Record {
public:
  int init(int form_id);
private:
};
#else
CREATE TABLE triggers
  (id        INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
   form_id   INTEGER NOT NULL DEFAULT 1,
   name      TEXT    NOT NULL DEFAULT 'trigger0',
   trgblk    INTEGER NOT NULL DEFAULT 1,
   trgfld    INTEGER NOT NULL DEFAULT 1,
   trgtyp    INTEGER NOT NULL DEFAULT 0,
   body      TEXT    NOT NULL DEFAULT ''
  );
#endif
