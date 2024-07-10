#define MSG_ALLOC 24
#define MSG_OPENKEY 26
#define MSG_SQL 50
#define MSG_FIRST 100
#define MSG_EDITKEY 101
#define MSG_RECDIRTY 102
#define MSG_LAST 103
#define MSG_NOREC 104
#define MSG_FLDPROT 200
#define MSG_NORANGE 207
#define MSG_RDONLY 208
#define MSG_FLDFORM 209
#define MSG_COUNT0 301
#define MSG_CLEAN 401
#define MSG_DIRTY 402
#ifndef BEGINSQL
// error configuration
class rError: public Record {
public:
  int init();
private:
};
#else
CREATE TABLE errors
  (id        INTEGER PRIMARY KEY NOT NULL,
   num       INTEGER NOT NULL DEFAULT 1,
   severity  INTEGER NOT NULL DEFAULT 0,    /* 0:info 1:warn 2:error 3:fatal */
   etext     TEXT    NOT NULL DEFAULT ''
  );
INSERT INTO errors (num, severity, etext) VALUES (MSG_ALLOC, 3, 'Out of memory');
INSERT INTO errors (num, severity, etext) VALUES (MSG_OPENKEY, 3, 'Error opening key script file');
INSERT INTO errors (num, severity, etext) VALUES (MSG_SQL, 3, 'SQL');
INSERT INTO errors (num, severity, etext) VALUES (MSG_FIRST, 1, 'At first record');
INSERT INTO errors (num, severity, etext) VALUES (MSG_EDITKEY, 2, 'Can''t edit a key field');
INSERT INTO errors (num, severity, etext) VALUES (MSG_RECDIRTY, 2, 'Record must be entered or deleted first');
INSERT INTO errors (num, severity, etext) VALUES (MSG_LAST, 1, 'At last record');
INSERT INTO errors (num, severity, etext) VALUES (MSG_NOREC, 2, 'No record');
INSERT INTO errors (num, severity, etext) VALUES (MSG_FLDPROT, 2, 'Field is protected against update');
INSERT INTO errors (num, severity, etext) VALUES (MSG_NORANGE, 2, 'Must be in range');
INSERT INTO errors (num, severity, etext) VALUES (MSG_RDONLY, 2, 'Form running in query-only mode. No changes');
INSERT INTO errors (num, severity, etext) VALUES (MSG_FLDFORM, 2, 'Field must be in the form');
INSERT INTO errors (num, severity, etext) VALUES (MSG_COUNT0, 2, 'Query caused no records to be retrieved');
INSERT INTO errors (num, severity, etext) VALUES (MSG_CLEAN, 1, 'No changes to commit');
INSERT INTO errors (num, severity, etext) VALUES (MSG_DIRTY, 2, 'Abort changes?');
#endif
