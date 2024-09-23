#define TRT_ENTERFORM 10001
#define TRT_NEXTITEM  10002
#ifndef BEGINSQL
/* trigger configuration */
class rTrigger: public Record {
public:
  int init(int form_id);
private:
};
#else
CREATE TABLE triggers
  (id        INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
   form_id   INTEGER NOT NULL DEFAULT 1,
   name      TEXT    NOT NULL DEFAULT '',
   trgfld    INTEGER NOT NULL DEFAULT 0,
   trgtyp    INTEGER NOT NULL DEFAULT 0,
   page_id   INTEGER NOT NULL DEFAULT 0
  );
INSERT INTO triggers (name, trgtyp, page_id) VALUES ('enter_the_form', TRT_ENTERFORM, 1001);
#endif
