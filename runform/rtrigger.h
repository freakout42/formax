#define TRT_ANYBODY    10000
#define TRT_ENTERFORM  10001
#define TRT_NEXTITEM   10002
#define TRT_EDITFIELD  10003
#define TRT_PREVITEM   10004
#define TRT_NEXTRECORD 10005
#define TRT_PREVRECORD 10006
#define TRT_COPYREC    10007
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
   trgtyp    INTEGER NOT NULL DEFAULT 0,
   page_id   INTEGER NOT NULL DEFAULT 0
  );
INSERT INTO triggers (form_id, trgfld, trgtyp, page_id) VALUES (0,0,TRT_COPYREC,1100);
#endif
