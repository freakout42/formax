#include <assert.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>
#include "runform.h"

#define FAILEDQ(hty) if (failed(hty)) return ret

static char buf[HUGSIZE];

void Record::setdrv(char *dbmsname) {
                                               drv = ODR_UNKNOWN;
if (!strcmp(dbmsname, "SQLite"))               drv = ODR_SQLITE;
if (!strcmp(dbmsname, "PostgreSQL"))           drv = ODR_PG;
if (!strcmp(dbmsname, "oracle"))               drv = ODR_ORACLE;
if (!strcmp(dbmsname, "Microsoft SQL Server")) drv = ODR_SQLSRVR;
if (!strcmp(dbmsname, "Advantage"))            drv = ODR_ADS;
g.logfmt("SQL_DBMS_NAME: %s -> %d", dbmsname, drv);
}

int Record::connect(char *dsn) {
SQLSMALLINT len;
char dbmsname[SMLSIZE];
if (useodbcve3) {
ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env); FAILEDQ(SQL_HANDLE_ENV);
ret = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);                     FAILEDQ(SQL_HANDLE_ENV);
ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);                                               FAILEDQ(SQL_HANDLE_DBC);
ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)dsn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT); FAILEDQ(SQL_HANDLE_DBC);
//ret = SQLSetEnvAttr(dbc, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);                    FAILEDQ(SQL_HANDLE_DBC);
} else {
ret = SQLAllocEnv(&env);                                                                       FAILEDQ(SQL_HANDLE_ENV);
ret = SQLAllocConnect(env, &dbc);                                                              FAILEDQ(SQL_HANDLE_ENV);
ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)dsn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT); FAILEDQ(SQL_HANDLE_DBC);
}
ret = SQLGetInfo(dbc, SQL_DBMS_NAME, &dbmsname, SMLSIZE, &len);                                FAILEDQ(SQL_HANDLE_DBC);
setdrv(dbmsname);
#define AUTOCOMMIT (SQLPOINTER)(autocommit ? SQL_AUTOCOMMIT_ON : SQL_AUTOCOMMIT_OFF)
ret = SQLSetConnectAttr(dbc, SQL_ATTR_AUTOCOMMIT, AUTOCOMMIT, SQL_IS_UINTEGER);                FAILEDQ(SQL_HANDLE_DBC);
ret = SQLGetFunctions(dbc, SQL_API_SQLMORERESULTS, &moreresults );                             FAILEDQ(SQL_HANDLE_DBC);
return ret;
}

int Record::connect(Record r) {
dbc = r.dbc;
drv = r.drv;
moreresults = r.moreresults;
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

int Record::ropen() {
if (stmt == NULL) {
  q = new(Qdata);
  ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);                                           FAILEDQ(SQL_HANDLE_STMT);
} else {
  ret = 0;
}
return ret;
}

void Record::rclose() {
delete(q);
if (stmt) {
  SQLFreeHandle(SQL_HANDLE_STMT, stmt);
  stmt = NULL;
} }

void Record::disconnect() {
if (dbc) {
  SQLDisconnect(dbc);
  SQLFreeHandle(SQL_HANDLE_DBC, dbc);
  SQLFreeHandle(SQL_HANDLE_ENV, env);
  dbc = NULL;
} }

int Record::execdirect(SQLCHAR *sql) {
ret = SQLExecDirect(stmt, sql, strlen((char*)sql));                                            FAILEDQ(SQL_HANDLE_STMT);
return ret;
}

int Record::execute(SQLCHAR *sql, char *b[]) {
SQLLEN len;
int i;
len = SQL_NTS;
let(sqlcmd, (char*)sql);
g.logsql(sqlcmd, b);
ret = SQLPrepare(stmt, sql, SQL_NTS);                                                          FAILEDQ(SQL_HANDLE_STMT);
for (i=0; b[i]; i++) {
  ret = SQLBindParameter(stmt, i+1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, b[i], SMLSIZE, &len);
                                                                                               FAILEDQ(SQL_HANDLE_STMT);
}
ret = SQLExecute(stmt);                                                                        FAILEDQ(SQL_HANDLE_STMT);
ret = SQLNumResultCols(stmt, &querycols);                                                      FAILEDQ(SQL_HANDLE_STMT);
return ret;
}

int Record::clear() {
if (q->alloc(columni)) return 13;
return 0;
}

int Record::query() {
int j;
if (clear()) return 13;
*whereorder = '\0';
j = *where ? letf(whereorder, sizeof(whereorder), " where %s", where) : 0;
if (*order) letf(whereorder+j, sizeof(whereorder)-j, " order by %s", order);
letf((char*)querystr, sizeof(querystr), "select %s from %s%s", attrs, table, whereorder);
bindv[0] = NULL;
if ((ret = execute(querystr, bindv))) return ret;
do {
  j = fetch(0);
  if (j > 0) return j;
} while (!j);
return complete();
}

int Record::complete() {
ret = SQLFreeStmt(stmt, SQL_CLOSE);                                                           FAILEDQ(SQL_HANDLE_STMT);
if (drv == ODR_ORACLE) {
  SQLFreeHandle(SQL_HANDLE_STMT, stmt);
  SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
}
return ret;
}

int Record::fetch(int row) {
SQLRETURN s;
SQLSMALLINT i;
SQLLEN indicator;
char *decimal;
char **qp;
//if (row) s = SQLMoreResults(stmt);
if (SQL_SUCCEEDED(s = SQLFetch(stmt))) {
  if (!row) row = q->rows++ + 1;
  for (i = 1; i <= columni; i++) {
    ret = SQLGetData(stmt, i, SQL_C_CHAR, buf, sizeof(buf), &indicator);
    if (SQL_SUCCEEDED(ret)) {
      if (!(qp = q->w(row, i))) return 13;
      free(*qp);
      if (indicator == SQL_NULL_DATA) *qp = NULL; else {
        decimal = buf + strspn(buf, "0123456789"); // cut trailing .00
        if (*decimal == '.' && strspn(decimal, "0.") == strlen(decimal)) *decimal = '\0';
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
   case SQL_HANDLE_STMT: handle = stmt; break;
  }
  rec = 1;
  while (SQLGetDiagRec(hty, handle, rec++, szSqlState, &nNativeError, szError, 500, &nErrorMsg) == SQL_SUCCESS) {
    if (szError[strlen((char*)szError)-1] == '\n') szError[strlen((char*)szError)-1] = '\0';
    g.logfmt("[%s]%s", szSqlState, szError );
  }
} else ret = 0;
return ret;
}
