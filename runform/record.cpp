/* record.cpp calls ODBC and
 * provides an interface in an ORM style like rails active-record
 */
#include "runform.h"
#include <sql.h>
#include <sqlext.h>

#define odbcver(hexver) char odbcversion[8] = str(hexver)
odbcver(ODBCVER);
char odbcrun[TNYSIZE] = "";

#define FAILEDQ(hty) if (failed(hty)) return ret

/* driver / database provider info */
void Record::setdrv(char *dbmsname) {          drv = ODR_UNKNOWN;
if (!strcmp(dbmsname, "SQLite"))               drv = ODR_SQLITE;
if (!strcmp(dbmsname, "MySQL"))                drv = ODR_MYSQL;
if (!strcmp(dbmsname, "PostgreSQL"))           drv = ODR_PG;
if (!strcmp(dbmsname, "oracle"))               drv = ODR_ORACLE;
if (!strcmp(dbmsname, "Microsoft SQL Server")) drv = ODR_SQLSRVR;
if (!strcmp(dbmsname, "Advantage"))            drv = ODR_ADS;
g.logfmt("SQL_DBMS_NAME: %s -> %d", dbmsname, drv);
}

/* connect to odbc dsn with mode 2 or 3
 * the connection can be shared with other instances
 * runform needs minimum 2 connections / dsns
 * one for the form itself (always sqlite3)
 * up to four for the actual data to manipulate
 */
int Record::connect(char *dsn) {
SQLSMALLINT len;
if (!dsn) {
  dbc = NULL;
  stmt = NULL;
  return 0;
}
if (useodbcve3) {
ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);                                   FAILEDQ(SQL_HANDLE_ENV);
ret = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);                     FAILEDQ(SQL_HANDLE_ENV);
ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);                                               FAILEDQ(SQL_HANDLE_DBC);
ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)dsn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT); FAILEDQ(SQL_HANDLE_DBC);
//ret = SQLSetEnvAttr(dbc, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);                    FAILEDQ(SQL_HANDLE_DBC);
} else {
ret = SQLAllocEnv(&env);                                                                       FAILEDQ(SQL_HANDLE_ENV);
ret = SQLAllocConnect(env, &dbc);                                                              FAILEDQ(SQL_HANDLE_ENV);
ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)dsn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT); FAILEDQ(SQL_HANDLE_DBC);
}
if (!odbcrun[0]) {
ret = SQLGetInfo(dbc, SQL_ODBC_VER,        &odbcrun,         TNYSIZE, &len);                   FAILEDQ(SQL_HANDLE_DBC); }
ret = SQLGetInfo(dbc, SQL_DRIVER_ODBC_VER, &driver_odbc_ver, TNYSIZE, &len);                   FAILEDQ(SQL_HANDLE_DBC);
ret = SQLGetInfo(dbc, SQL_DRIVER_VER,      &driver_ver,      TNYSIZE, &len);                   FAILEDQ(SQL_HANDLE_DBC);
ret = SQLGetInfo(dbc, SQL_DBMS_NAME,       &dbmsname,        TNYSIZE, &len);                   FAILEDQ(SQL_HANDLE_DBC);
ret = SQLGetInfo(dbc, SQL_DBMS_VER,        &dbmsver,         TNYSIZE, &len);                   FAILEDQ(SQL_HANDLE_DBC);
setdrv(dbmsname);
#define AUTOCOMMIT (SQLPOINTER)(autocommit ? SQL_AUTOCOMMIT_ON : SQL_AUTOCOMMIT_OFF)
ret = SQLSetConnectAttr(dbc, SQL_ATTR_AUTOCOMMIT, AUTOCOMMIT, SQL_IS_UINTEGER);                FAILEDQ(SQL_HANDLE_DBC);
ret = SQLGetFunctions(dbc, SQL_API_SQLMORERESULTS, &moreresults );                             FAILEDQ(SQL_HANDLE_DBC);
stmt = NULL;
return ret;
}

/* share the connection with another instance */
int Record::connect(Record r) {
dbc = r.dbc;
drv = r.drv;
moreresults = r.moreresults;
stmt = NULL;
return 0;
}

int Record::commit() {
ret = SQLEndTran(SQL_HANDLE_ENV, env, SQL_COMMIT);                                             FAILEDQ(SQL_HANDLE_ENV);
return 0;
}

int Record::rollback() {
ret = SQLEndTran(SQL_HANDLE_ENV, env, SQL_ROLLBACK);                                           FAILEDQ(SQL_HANDLE_ENV);
return 0;
}

/* open the table by setup a statement handle and allocating storage
 * the statement handle stmt works as the flag for an open table
 */
int Record::ropen() {
ret = 0;
if (dbc)
 if (stmt == NULL) {
  q = new Qdata();
  querystr = (SQLCHAR*)malloc(MEDSIZE);
  ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);                                           FAILEDQ(SQL_HANDLE_STMT);
 }
return ret;
}

/* close the table by deallocating storage and handle */
void Record::rclose() {
if (dbc) {
 delete(q);
 if (stmt) {
  SQLFreeHandle(SQL_HANDLE_STMT, stmt);
  free(querystr);
  stmt = NULL;
} } }

/* free the connection resources
 * only the last instance should do that
 */
void Record::disconnect() {
if (dbc) {
  SQLDisconnect(dbc);
  SQLFreeHandle(SQL_HANDLE_DBC, dbc);
  SQLFreeHandle(SQL_HANDLE_ENV, env);
  dbc = NULL;
} }

/* direct sql without orm support */
int Record::execdirect(char *sql) {
ret = SQLExecDirect(stmt, (SQLCHAR*)sql, strlen(sql));                                         FAILEDQ(SQL_HANDLE_STMT);
ret = SQLNumResultCols(stmt, &querycols);                                                      FAILEDQ(SQL_HANDLE_STMT);
columni = querycols;
if (clear()) return 13;
g.logfmt("SQL: '%s' [%d]", sql, querycols);
return fetchall();
}

/* direct sql with bind support from variable array */
int Record::execute() { return execute(querystr, bindv); }
int Record::execute(SQLCHAR *sql, char *b[]) {
SQLLEN len;
int i;
len = SQL_NTS;
g.logsql((char*)sql, b);
ret = SQLPrepare(stmt, sql, SQL_NTS);                                                          FAILEDQ(SQL_HANDLE_STMT);
for (i=0; b[i]; i++) {
  ret = SQLBindParameter(stmt, i+1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, b[i], SMLSIZE, &len);
                                                                                               FAILEDQ(SQL_HANDLE_STMT);
}
ret = SQLExecute(stmt);                                                                        FAILEDQ(SQL_HANDLE_STMT);
ret = SQLNumResultCols(stmt, &querycols);                                                      FAILEDQ(SQL_HANDLE_STMT);
return ret;
}

/* cleanup the query data storage und allocate empty for 2 records */
int Record::clear() {
if (q->alloc(columni)) return 13;
return 0;
}

/* orm select with where / order support */
int Record::query() {
int j;
if (clear()) return 13;
empty(whereorder);
j = 0;
if (  *where  && !(*condition)) j = letf(t(whereorder), " where %s", where);
if (!(*where) &&   *condition)  j = letf(t(whereorder), " where %s", condition);
if (  *where  &&   *condition)  j = letf(t(whereorder), " where (%s) AND (%s)", where, condition);
if (  *order                 )      letf(whereorder+j, sizeof(whereorder)-j, " order by %s", order);
letf((char*)querystr, MEDSIZE, "select %s from %s%s", attrs, table, whereorder);
bindv[0] = NULL;
if ((ret = execute())) return ret;
empty(condition);
return fetchall();
}

/* cleanup select handle after execution and fetch
 * oracle must free and alloc the handle new
 */
int Record::complete() {
ret = SQLFreeStmt(stmt, SQL_CLOSE);                                                           FAILEDQ(SQL_HANDLE_STMT);
if (drv == ODR_ORACLE) {
  SQLFreeHandle(SQL_HANDLE_STMT, stmt);
  SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
}
return ret;
}

/* fetch all rows */
int Record::fetchall() {
int j;
do {
  j = fetch(0);
  if (j > 0) return j;
} while (!j);
return complete();
}

/* orm fetch query data */
int Record::fetch(int row) {
SQLRETURN s;
SQLSMALLINT i;
SQLLEN indicator;
char **qp;
char buf[HUGSIZE];
//if (row) s = SQLMoreResults(stmt);
if (SQL_SUCCEEDED(s = SQLFetch(stmt))) {
  if (!row) row = q->rows++ + 1;
  for (i = 1; i <= columni; i++) {
    ret = SQLGetData(stmt, i, SQL_C_CHAR, buf, sizeof(buf), &indicator);
    if (SQL_SUCCEEDED(ret)) {
      if (!(qp = q->w(row, i))) return 13;
      free(*qp);
      if (indicator == SQL_NULL_DATA) *qp = NULL; else {
        rtrim0white(buf);
        if (!(*qp = strdup(buf))) return 13;
      }
    }
  }
}
return succeeded(s) ? -1 : 0;
//return failed(SQL_HANDLE_STMT) ? 0 : -1;
}

int Record::succeeded(SQLRETURN s) {
return !SQL_SUCCEEDED(s);
}

/* handle failed calls and log them with additional info */
int Record::failed(SQLSMALLINT hty) {
SQLHANDLE handle;
SQLCHAR szError[SMLSIZE];
SQLCHAR szSqlState[SMLSIZE];
SQLINTEGER nNativeError;
SQLSMALLINT nErrorMsg;
int rec;
if (ret && ret != SQL_NO_DATA && ret != SQL_SUCCESS_WITH_INFO) {
  switch (hty) {
   case SQL_HANDLE_ENV:  handle = env;  break;
   case SQL_HANDLE_DBC:  handle = dbc;  break;
   case SQL_HANDLE_STMT:
   default:              handle = stmt;
  }
  rec = 1;
  while (SQLGetDiagRec(hty, handle, rec++, szSqlState, &nNativeError, szError, 500, &nErrorMsg) == SQL_SUCCESS) {
    if (szError[strlen((char*)szError)-1] == '\n') szError[strlen((char*)szError)-1] = '\0';
    g.logfmt("[%s]%s", szSqlState, szError);
  }
} else ret = 0;
return ret;
}

