#undef DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif
#include <assert.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>
#include "runform.h"

static char buf[HUGSIZE];

int Record::connect(char *dsn) {
char dbmsname[SMLSIZE];
SQLSMALLINT len;
if (useodbcve3) {
if (ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env)) return ret;
if (ret = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0)) return ret;
if (ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc)) return ret;
if (ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)dsn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT)) return ret;
//if (ret = SQLSetEnvAttr(dbc, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0)) return ret;
} else {
if (ret = SQLAllocEnv(&env)) return ret;
if (ret = SQLAllocConnect(env, &dbc)) return ret;
if (ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)dsn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT)) return ret;
}
SQLGetInfo(dbc, SQL_DBMS_NAME, &dbmsname, SMLSIZE, &len);
drv = ODR_UNKNOWN;
if (!strcmp(dbmsname, "SQLite")) drv = ODR_SQLITE;
if (!strcmp(dbmsname, "oracle")) drv = ODR_ORACLE;
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
SQLSMALLINT i;
SQLLEN len;
int s;
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
//    ret = SQLNumResultCols(stmt, &i);
    ret = SQLExecute(stmt);
    if (ret && ret != SQL_NO_DATA && ret != SQL_SUCCESS_WITH_INFO && ret != -1) s = 11; else {
; //      if ((ret = SQLNumResultCols(stmt, &i))) s = 12; // else  assert(i == columni) rmode?;
    }
  }
}
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
char **qp;
//if (row) s = SQLMoreResults(stmt);
if (SQL_SUCCEEDED(s = SQLFetch(stmt))) {
  if (!row) row = q->rows++ + 1;
  for (i = 1; i <= columni; i++) {
    ret = SQLGetData(stmt, i, SQL_C_CHAR, buf, sizeof(buf), &indicator);
    if (SQL_SUCCEEDED(ret)) {
      if (!(qp = q->w(row, i))) return 13;
      free(*qp);
      if (indicator == SQL_NULL_DATA) *qp = NULL; else if (!(*qp = strdup(buf))) return 13;
/*
    } else {
//      while (SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, ++rec, szSqlState, &nNativeError, szError, 500, &nErrorMsg) == SQL_SUCCESS) {
//        fprintf(stderr, "[%s]%s\n", szSqlState, szError );
//      }
      return 14;
 */
    }
  }
}
return SQL_SUCCEEDED(s) ? 0 : -1;
}

