#define TRG_SUCCESS0   1001
#define TRT_ENTERFORM  10001
#define TRT_NEXTITEM   10002
#define TRT_EDITFIELD  10003
#define TRT_PREVITEM   10004
#define TRT_NEXTRECORD 10005
#define TRT_PREVRECORD 10006
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
   trgfld    INTEGER NOT NULL DEFAULT 0,
   trgtyp    INTEGER NOT NULL DEFAULT TRT_ENTERFORM,
   page_id   INTEGER NOT NULL DEFAULT TRG_SUCCESS0
  );
INSERT INTO triggers (form_id) VALUES (1);
#endif
