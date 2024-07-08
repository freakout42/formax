#include <assert.h>
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
if (ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)dsn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE)) return ret;
if (!autocommit) ret = SQLSetConnectAttr(dbc, SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER);
return ret;
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

int Record::execute(SQLCHAR *sql, char *b[]) {
int i, s;
SQLLEN len;
len = SQL_NTS;
for (i=0; b[i]; i++) ret = SQLBindParameter(stmt, i+1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, b[i], 0, &len);
let(sqlcmd, (char*)sql);
s = 0;
if ((ret = SQLPrepare(stmt, sql, SQL_NTS))) s = 10; else
if ((ret = SQLExecute(stmt))) s = 11;
if (ret) f.p[0].message(50, sqlcmd);
return s;
}

int Record::clear() {
q->freed();
if (q->alloc(columni)) return 13;
return 0;
}

int Record::query() {
SQLSMALLINT i;
SQLLEN indicator;
int j;
char **qp;
if (clear()) return 13;
*whereorder = '\0';
j = *where ? letf(whereorder, sizeof(whereorder), " where %s", where) : 0;
if (*order) letf(whereorder+j, sizeof(whereorder)-j, " order by %s", order);
letf((char*)querystr, sizeof(querystr), "select %s from %s%s", attrs, table, whereorder);
bindv[0] = NULL;
if ((ret = execute(querystr, bindv))) return ret;
if ((ret = SQLNumResultCols(stmt, &i))) {
  assert(i != columni);
  return 12;
}
while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
  q->rows++;
  for (i = 1; i <= columni; i++) {
    ret = SQLGetData(stmt, i, SQL_C_CHAR, buf, sizeof(buf), &indicator);
    if (SQL_SUCCEEDED(ret)) {
/*
 *    if (indicator == SQL_NULL_DATA) *buf = '\0';
 *    if (!(qp = q->w(q->rows, i))) return 13;
 *    if (!(*qp = strdup(buf))) return 13;
 */
      if (!(qp = q->w(q->rows, i))) return 13;
      if (indicator == SQL_NULL_DATA) *qp = NULL; else if (!(*qp = strdup(buf))) return 13;
    }
    else return 14;
  }
}
return SQLFreeStmt(stmt, SQL_CLOSE);
}

