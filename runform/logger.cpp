#include <cstdarg>
#include <stdio.h>
#include <sqlite3.h>
#include "runform.h"

void Logger::logf(char *format, ...) {
/*
va_list args;
va_start (args, format);
vfprintf(stderr, format, args);
va_end (args);
*/
}

#ifdef nonono
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s", argv[i] ? argv[i] : "NULL");
    if (i!=argc-1) printf(" ");
  }
  printf("\n");
  return 0;
}

int main(int argc, char **argv){
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  fprintf(stderr, "SQLITE_VERSION: %s\n", SQLITE_VERSION);
  rc = sqlite3_open("/app/rails/rr4/db/devl.sq3", &db);
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return(1);
  }
  rc = sqlite3_exec(db, "select empno, ename from emps order by empno", callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  sqlite3_close(db);
  return 0;
}
#endif
