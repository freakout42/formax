#ifndef SQLITE3_H
typedef struct sqlite3 sqlite3;
#endif
class Logger {
public:
  void init();
  int setlogfile(char *lg3, char *dsn);
  void logf(char *format, ...);
  void lclose();
private:
  sqlite3 *db;
  char logpath[SMLSIZE];
};

