// error configuration
class rError: public Record {
public:
  int init();
private:
};

#ifdef BEGINSQL
CREATE TABLE errors
  (id        INTEGER PRIMARY KEY NOT NULL,
   num       INTEGER NOT NULL DEFAULT 1,
   severity  INTEGER NOT NULL DEFAULT 0,    /* 0:info 1:warn 2:error 3:fatal */
   etext     TEXT    NOT NULL DEFAULT ''
  );
INSERT INTO errors (num, severity, etext) VALUES ( 24, 3, 'Out of memory');
INSERT INTO errors (num, severity, etext) VALUES ( 26, 3, 'Error opening key script file');
INSERT INTO errors (num, severity, etext) VALUES ( 50, 3, 'SQL');
INSERT INTO errors (num, severity, etext) VALUES (100, 1, 'At first record');
INSERT INTO errors (num, severity, etext) VALUES (101, 2, 'Can''t edit a key field');
INSERT INTO errors (num, severity, etext) VALUES (102, 2, 'Record must be entered or deleted first');
INSERT INTO errors (num, severity, etext) VALUES (103, 1, 'At last record');
INSERT INTO errors (num, severity, etext) VALUES (200, 2, 'Field is protected against update');
INSERT INTO errors (num, severity, etext) VALUES (207, 2, 'Must be in range');
INSERT INTO errors (num, severity, etext) VALUES (208, 2, 'Form running in query-only mode. No changes');
INSERT INTO errors (num, severity, etext) VALUES (209, 2, 'Field must be in the form');
INSERT INTO errors (num, severity, etext) VALUES (301, 2, 'Query caused no records to be retrieved');
INSERT INTO errors (num, severity, etext) VALUES (401, 1, 'No changes to commit');
#endif
