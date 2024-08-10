#ifndef BEGINSQL
// block configuration
class rBlock: public Record {
public:
  int init(int form_id);
private:
//  char *name;
};
#else
CREATE TABLE blocks
  (id        INTEGER PRIMARY KEY NOT NULL,
   form_id   INTEGER NOT NULL DEFAULT 1,
   name      TEXT    NOT NULL DEFAULT 'block0',
   seq       INTEGER NOT NULL DEFAULT 1,
   norec     INTEGER NOT NULL DEFAULT 1,    /* number of records to display */
   prikey    TEXT    NOT NULL DEFAULT 'id', /* primary key */
   whereand  TEXT    NOT NULL DEFAULT '',   /* default where clause */
   orderby   TEXT    NOT NULL DEFAULT ''    /* default order clause */
  );
#endif
