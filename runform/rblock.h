#ifndef BEGINSQL
/* block configuration
 * the dual blocks 1-4 are present in all forms form_id=0
 * and hold the database connection info in the Record
 */
class rBlock: public Record {
public:
  int init(int form_id);
private:
};
#else
CREATE TABLE blocks
  (id        INTEGER PRIMARY KEY NOT NULL,
   form_id   INTEGER NOT NULL DEFAULT 1,
   name      TEXT    NOT NULL DEFAULT 'dual',
   seq       INTEGER NOT NULL DEFAULT 1,
   norec     INTEGER NOT NULL DEFAULT 1,    /* number of records to display */
   prikey    TEXT    NOT NULL DEFAULT 'id', /* primary key */
   whereand  TEXT    NOT NULL DEFAULT '',   /* default where clause */
   orderby   TEXT    NOT NULL DEFAULT ''    /* default order clause */
  );
INSERT INTO blocks (form_id, name, seq) VALUES (0, 'dual', 1);
INSERT INTO blocks (form_id, name, seq) VALUES (0, 'dual2', 2);
INSERT INTO blocks (form_id, name, seq) VALUES (0, 'dual3', 3);
INSERT INTO blocks (form_id, name, seq) VALUES (0, 'dual4', 4);
#endif
