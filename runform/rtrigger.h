#define TRT_ENTERFORM  1000
#define TRT_NEXTITEM   1001
#define TRT_PREVITEM   1002
#define TRT_NEXTRECORD 1003
#define TRT_PREVRECORD 1004
#define TRT_NEXTSETREC 1005
#define TRT_PREVSETREC 1006
#define TRT_EDITFIELD  1007
#define TRT_COPYREC    1008
#define TRT_COPY       1009
#define TRT_PASTE      1010
#define TRT_POSTQUERY  1011
#define TRT_NEXTBLOCK  1012
#define TRT_PREVBLOCK  1013
#define TRT_EXEQUERY   1014
#define TRT_POSTCHANGE 1015
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
INSERT INTO triggers (form_id, trgfld, trgtyp, page_id) VALUES (0,0,TRT_COPYREC,TRT_COPYREC);
INSERT INTO triggers (form_id, trgfld, trgtyp, page_id) VALUES (0,0,TRT_COPY,TRT_COPY);
INSERT INTO triggers (form_id, trgfld, trgtyp, page_id) VALUES (0,0,TRT_PASTE,TRT_PASTE);
#endif
