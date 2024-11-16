/* logging sql engine via sqlite3 lib */
#ifndef SQLITE3_H
typedef struct sqlite3 sqlite3;
#endif
class Logger {
public:
  void init(char *dsn);
  int setlogfile(char *lg3);
  void logfmt(const char *format, ...);
  void logsql(char *sql, char *bnd[]);
  void lclose();
private:
  sqlite3 *db;
  char logpath[SMLSIZE];
};

