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
#define TRT_ENTERECORD 1016
#define TRT_EDITFILE   1017
#define TRT_EXITFORM   1018
#define TRT_HELP       1019
#define TRT_LEFT       1020
#define TRT_RIGHT      1021
#define TRT_DELETE     1022
#define TRT_BACKDEL    1023
#define TRT_INS        1024
#define TRT_INSERT     1025
#define TRT_KEY        1026
#define TRT_REFRESH    1027
#define TRT_COMMIT     1028
#define TRT_LIST       1029
#define TRT_QUIT       1030
#define TRT_EXIT       1031
#define TRL_JAVASCRIPT 0
#define TRL_KEYMACRO   1
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
   trgfld    INTEGER NOT NULL DEFAULT 0, /* negative is block id */
   trgtyp    INTEGER NOT NULL DEFAULT 0, /* % 10000 is type / 10000 is language 0.js 1.macro */
   page_id   INTEGER NOT NULL DEFAULT 0  /* foreign key for maps */
  );
INSERT INTO triggers (form_id, trgfld, trgtyp, page_id) VALUES (0,0,TRT_COPYREC,TRT_COPYREC);
INSERT INTO triggers (form_id, trgfld, trgtyp, page_id) VALUES (0,0,TRT_COPY,TRT_COPY);
INSERT INTO triggers (form_id, trgfld, trgtyp, page_id) VALUES (0,0,TRT_PASTE,TRT_PASTE);
#endif
