#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "runform.h"

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

static int session = -1;
static char message[MEDSIZE];
static char sqlquery[MEDSIZE*2];

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
if (session == 0 && argc == 1) session = atoi(argv[0]);
return 0;
}
// for (i=0; i<argc; i++) {
//   fprintf(stderr, "%s", argv[i] ? argv[i] : "NULL");

void Logger::init(char *dsn) {
int rc;
char sql[SMLSIZE];
switch (session) {
 case -1:
  session = 0;
  let(logpath, "");
  break;
 case 0:
  if ((rc = sqlite3_open(logpath, &db))) return;
  letf(t(sql), INSERTLOGIN, dsn);
  while ((rc = sqlite3_exec(db, sql, callback, 0, NULL)) != SQLITE_OK) {
    if (sqlite3_exec(db, CREATESESSION, callback, 0, NULL) != SQLITE_OK) return;
        sqlite3_exec(db, CREATELOGS,    callback, 0, NULL);
  }
  break;
} }

void Logger::lclose() {
if (*logpath) sqlite3_close(db);
session = 0;
}

int Logger::setlogfile(char *lg3) {
session = 0;
let(logpath, lg3);
return 0;
}

void Logger::logfmt(char *format, ...) {
va_list args;
if (*logpath) {
va_start (args, format);
vsnprintf (t(message), format, args);
snprintf (t(sqlquery), INSERTLOG, session, message);
sqlite3_exec(db, sqlquery, callback, 0, NULL);
//vfprintf(stderr, format, args);
va_end (args);
} }

void Logger::logsql(char *sql, char *bnd[]) {
int i, j, k, l, m;
char *r;
char apostrophe = '\'';
if (*logpath) {
//let(message, sql);
m = (int)sizeof(message) - 8;
i = k = l = 0;
j = -1;
while (sql[i] && k < m) {
  switch (sql[i]) {
   case '?':
    if (j == -1) {
      j = 0;
      r = &apostrophe;
    } else if (bnd[l][j]) {
      r = bnd[l]+(j++);
    } else {
      j = -1;
      l++;
      i++;
      r = &apostrophe;
    }
    break;
   default:
    r = sql+(i++);
  }
  message[k++] = *r;
  if (*r == apostrophe) message[k++] = apostrophe;
}
message[k++] = '\0';
snprintf (t(sqlquery), INSERTLOG, session, message);
sqlite3_exec(db, sqlquery, callback, 0, NULL);
} }

