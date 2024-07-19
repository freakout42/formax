#include <assert.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>
#include "runform.h"

#define FAILEDQ(hty) if (failed(hty)) return ret

static char buf[HUGSIZE];

int Record::connect(char *dsn) {
char dbmsname[SMLSIZE];
SQLSMALLINT len;
if (useodbcve3) {
if (ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env)) return ret;
if (ret = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0)) return ret;
if (ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc)) return ret;
ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)dsn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT); FAILEDQ(SQL_HANDLE_DBC);
//if (ret = SQLSetEnvAttr(dbc, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0)) return ret;
} else {
if (ret = SQLAllocEnv(&env)) return ret;
if (ret = SQLAllocConnect(env, &dbc)) return ret;
ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)dsn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT); FAILEDQ(SQL_HANDLE_DBC);
}
SQLGetInfo(dbc, SQL_DBMS_NAME, &dbmsname, SMLSIZE, &len);
drv = ODR_UNKNOWN;
if (!strcmp(dbmsname, "SQLite"))               drv = ODR_SQLITE;
if (!strcmp(dbmsname, "oracle"))               drv = ODR_ORACLE;
if (!strcmp(dbmsname, "Microsoft SQL Server")) drv = ODR_SQLSRVR;
#ifdef DEBUG
fprintf(stderr, "SQL_DBMS_NAME: %s\n", dbmsname);
#endif
if (ret = SQLSetConnectAttr(dbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)(autocommit ? SQL_AUTOCOMMIT_ON : SQL_AUTOCOMMIT_OFF), SQL_IS_UINTEGER)) return ret;
if (ret = SQLGetFunctions(dbc, SQL_API_SQLMORERESULTS, &moreresults )) return ret;
return ret;
}

int Record::connect(Record r) {
dbc = r.dbc;
drv = r.drv;
moreresults = r.moreresults;
return 0;
}

int Record::commit() {
ret = SQLEndTran(SQL_HANDLE_ENV, env, SQL_COMMIT);
return ret;
}
int Record::rollback() {
ret = SQLEndTran(SQL_HANDLE_ENV, env, SQL_ROLLBACK);
return ret;
}

int Record::ropen() {
if (stmt == NULL) {
  q = new(Qdata);
  q->init();
  ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
} else {
  ret = 0;
}
return ret;
}

void Record::rclose() {
q->freed();
delete(q);
SQLFreeHandle(SQL_HANDLE_STMT, stmt);
stmt = NULL;
}

void Record::disconnect() {
SQLDisconnect(dbc);
SQLFreeHandle(SQL_HANDLE_DBC, dbc);
SQLFreeHandle(SQL_HANDLE_ENV, env);
dbc = NULL;
}

int Record::execdirect(SQLCHAR *sql) {
return (ret = SQLExecDirect(stmt, sql, strlen((char*)sql)));
}

int Record::execute(SQLCHAR *sql, char *b[]) {
SQLLEN len;
int i, s;
s = 0;
len = SQL_NTS;
let(sqlcmd, (char*)sql);
ret = SQLPrepare(stmt, sql, SQL_NTS);
if (ret && ret != SQL_NO_DATA && ret != SQL_SUCCESS_WITH_INFO) s = 10; else {
  for (i=0; !s && b[i]; i++) {
    ret = SQLBindParameter(stmt, i+1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, b[i], SMLSIZE, &len);
    if (ret) s = 15;
  }
  if (!s) {
    ret = SQLExecute(stmt);
    if (ret && ret != SQL_NO_DATA && ret != SQL_SUCCESS_WITH_INFO && ret != -1) s = 11; else {
      if ((ret = SQLNumResultCols(stmt, &querycols))) s = 12;
    }
  }
}
g.logf("SQL: %d %d %s\n", s, ret, sqlcmd);
#ifdef DEBUG
fprintf(stderr, "SQL: %d %d %s\n", s, ret, sqlcmd);
#endif
return s;
}

int Record::clear() {
q->freed();
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
ret = SQLFreeStmt(stmt, SQL_CLOSE);
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
/*
SQLHANDLE handle;
SQLCHAR szError[SMLSIZE];
SQLCHAR szSqlState[SMLSIZE];
SQLINTEGER nNativeError;
SQLSMALLINT nErrorMsg;
int rec;
*/
if (ret && ret != SQL_NO_DATA && ret != SQL_SUCCESS_WITH_INFO) {
/*
  switch (hty) {
   case SQL_HANDLE_DBC:  handle = dbc;  break;
   case SQL_HANDLE_STMT: handle = stmt; break;
  }
  rec = 1;
  while (SQLGetDiagRec(hty, handle, rec++, szSqlState, &nNativeError, szError, 500, &nErrorMsg) == SQL_SUCCESS)
    if (szError[strlen((char*)szError)-1] == '\n') szError[strlen((char*)szError)-1] = '\0';
  g.logf("[%s]%s\n", szSqlState, szError );
*/
  return ret;
} else return 0;
}

