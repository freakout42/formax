// error configuration
class rError: public Record {
public:
  int init();
private:
};

#ifdef BEGINSQL
CREATE TABLE errors
  (id        INTEGER PRIMARY KEY NOT NULL,
   num       INTEGER NOT NULL DEFAULT 40000,
   severity  INTEGER NOT NULL DEFAULT 0,    /* 0:info 1:warn 2:error 3:fatal */
   etext     TEXT    NOT NULL DEFAULT ''
  );
INSERT INTO errors (num, severity, etext) VALUES (40401, 1, 'No changes to save');
#endif
