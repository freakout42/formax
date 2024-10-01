/* logging sql engine via sqlite3 lib - not odbc */
#include <cstdarg>
#include <stdio.h>
#include <sqlite3.h>
#include "runform.h"

#define INSERTLOGIN   "insert into sessions (loginame, sshconn, logtime,           odbcdsn) " \
                                    "values ('%s',     '%s',    current_timestamp, '%s') returning id"
#define INSERTLOG     "insert into logs (session_id, logtime, logtext) values (%d, current_timestamp, '%s')"
#define CREATESESSION "create table sessions (" \
                        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " \
                        "loginame TEXT NOT NULL, " \
                        "sshconn TEXT NOT NULL, " \
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

/* the only read is for the session id from the returning clause of _INSERTLOGIN */
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
if (session == 0 && argc == 1) session = atoi(argv[0]);
return 0;
}

/* create the tables when not existing */
void Logger::init(char *dsn) {
int rc;
char na[4];
char *user;
char *conn;
char *tmp;
char sql[SMLSIZE];
strcpy(na, "n/a");
user = conn = na;
switch (session) {
 case -1:
  session = 0;
  let(logpath, "");
  break;
 case 0:
  if ((rc = sqlite3_open(logpath, &db))) return;
  if ((tmp = getenv("USER"))) user = tmp;
  if ((tmp = getenv("SSH_CONNECTION"))) conn = tmp;
  letf(t(sql), INSERTLOGIN, user, conn, dsn);
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

void Logger::logfmt(const char *format, ...) {
va_list args;
char *apostrophe;
if (*logpath) {
va_start (args, format);
vsnprintf (t(message), format, args);
while (apostrophe = strchr(message, '\'')) *apostrophe = '"';
snprintf (t(sqlquery), INSERTLOG, session, message);
sqlite3_exec(db, sqlquery, callback, 0, NULL);
//vfprintf(stderr, format, args);
va_end (args);
} }

/* must escape apostrophes and interpolate the bind variables
 * apostrophies in bind strings have to be quad repeated
 * so that the queries can be executed by cut and paste
 */
void Logger::logsql(char *sql, char *bnd[]) {
int i, j, k;
int l1, m1, n1;
char *r1;
char apostrophe = '\'';
if (*logpath) {
//let(message, sql); /* no handling of apos not acceptable */
m1 = (int)sizeof(message) - 8;
i = k = l1 = 0;
j = -1;
while (sql[i] && k < m1) {
  switch (sql[i]) {
   case '?':
    if (j == -1) {
      j = 0;
      r1 = &apostrophe;
    } else if (bnd[l1][j]) {
      r1 = bnd[l1]+(j++);
    } else {
      j = -1;
      l1++;
      i++;
      r1 = &apostrophe;
    }
    break;
   default:
    r1 = sql+(i++);
  }
  message[k++] = *r1;
  if (*r1 == apostrophe) for (n1=0; n1 < (j>0 ? 3 : 1); n1++) message[k++] = apostrophe;
}
message[k++] = '\0';
snprintf (t(sqlquery), INSERTLOG, session, message);
sqlite3_exec(db, sqlquery, callback, 0, NULL);
} }

