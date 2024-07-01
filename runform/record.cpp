#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>
#include "runform.h"

static char buf[HUGSIZE];

/* the dbc-handle can be given to other record-class */
int Record::connect(char *dsn) {
SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);
SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
return SQLDriverConnect(dbc, NULL, (SQLCHAR*)dsn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
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

int Record::execute(SQLCHAR *sql) {
let(sqlcmd, (char*)sql);
if ((ret = SQLPrepare(stmt, sql, SQL_NTS))) return 10;
if ((ret = SQLExecute(stmt))) return 11;
return ret;
}

int Record::query() {
SQLCHAR select[MEDSIZE];
char whereorder[MEDSIZE] = "";
SQLUSMALLINT i;
SQLINTEGER indicator;
int j;
char **qp;
q->freed();
j = *where ? letf(whereorder, sizeof(whereorder), " where %s", where) : 0;
if (*order) letf(whereorder+j, sizeof(whereorder)-j, " order by %s", order);
letf((char*)select, sizeof(select), "select %s from %s%s", attrs, table, whereorder);
if ((ret = execute(select))) return ret;
if ((ret = SQLNumResultCols(stmt, &columni))) {
  f.p[0].message(40100, (char*)select);
  return 12;
}
if (q->alloc(columni)) return 13;
while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
  q->rows++;
  for (i = 1; i <= columni; i++) {
    ret = SQLGetData(stmt, i, SQL_C_CHAR, buf, sizeof(buf), &indicator);
    if (SQL_SUCCEEDED(ret)) {
      if (indicator == SQL_NULL_DATA) *buf = '\0'; // strcpy(buf, "");
      if (!(qp = q->w(q->rows, i))) return 13;
      if (!(*qp = strdup(buf))) return 13;
    }
    else return 14;
  }
}
return SQLFreeStmt(stmt, SQL_CLOSE);
}
