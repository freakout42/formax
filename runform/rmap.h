#ifndef BEGINSQL
// map configuration
class rMap: public Record {
public:
  int init(int page_id);
};
#else
CREATE TABLE maps
  (id        INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
   page_id   INTEGER NOT NULL DEFAULT 1,
   line      INTEGER NOT NULL DEFAULT 1,
   mtext     TEXT    NOT NULL DEFAULT ''
  );
#endif
