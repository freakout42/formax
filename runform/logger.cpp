#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "runform.h"

static int session;
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
/* int i;
 * for (i=0; i<argc; i++) {
 *   fprintf(stderr, "%s", argv[i] ? argv[i] : "NULL");
 */
if (session == 0 && argc == 1) session = atoi(argv[0]);
return 0;
}

void Logger::init() {
session = 0;
let(logpath, "");
}

void Logger::lclose() {
if (*logpath) sqlite3_close(db);
init();
}

#define INSERTLOGIN   "insert into sessions (logtime, odbcdsn) values (current_timestamp, '%s') returning id"
#define INSERTLOG     "insert into logs (session_id, logtime, logtext) values (%d, current_timestamp, '%s')"
#define CREATESESSION "create table sessions (" \
                        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " \
                        "logtime INTEGER NOT NULL, " \
                        "odbcdsn TEXT NOT NULL " \
                      ")"
#define CREATELOGS    "create table logs (" \
                        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " \
                        "session_id INTEGER NOT NULL, " \
                        "logtime INTEGER NOT NULL, " \
                        "logtext TEXT NOT NULL " \
                      ")"

int Logger::setlogfile(char *lg3, char *dsn) {
int rc;
//char *zErrMsg;
char sql[SMLSIZE];
rc = sqlite3_open(lg3, &db);
if (rc) return 2;
letf(t(sql), INSERTLOGIN, dsn);
while ((rc = sqlite3_exec(db, sql, callback, 0, NULL)) != SQLITE_OK) {
  if (sqlite3_exec(db, CREATESESSION, callback, 0, NULL) != SQLITE_OK) return 1;
      sqlite3_exec(db, CREATELOGS,    callback, 0, NULL);
}
/* rc = sqlite3_exec(db, "select empno, ename from emps order by empno", callback, 0, &zErrMsg);
 * if( rc!=SQLITE_OK ){
 *   fprintf(stderr, "SQL error: %s\n", zErrMsg);
 *   sqlite3_free(zErrMsg);
 */
let(logpath, lg3);
return 0;
}

void Logger::logf(char *format, ...) {
va_list args;
char msg[MEDSIZE];
char sql[MEDSIZE];
if (*logpath) {
va_start (args, format);
vsnprintf (t(msg), format, args);
snprintf (t(sql), INSERTLOG, session, msg);
sqlite3_exec(db, sql, callback, 0, NULL);
//vfprintf(stderr, format, args);
va_end (args);
} }

