#ifndef __SQL_H
typedef void * SQLHDBC;
typedef void * SQLHENV;
typedef void * SQLHSTMT;
typedef unsigned char SQLCHAR;
typedef signed short int SQLSMALLINT;
typedef SQLSMALLINT SQLRETURN;
#endif
class Record {
public:
  SQLHDBC dbc;
  char table[SMLSIZE];
  char sqlcmd[MEDSIZE];
  SQLCHAR querystr[MEDSIZE];
  char whereorder[MEDSIZE];
  int connect(char *dsn);
  void disconnect();
  int ropen();
  void rclose();
  int query();
  char *c(int row, int col);
  int n(int row, int col);
  Qdata *q;
protected:
  SQLHSTMT stmt;
  SQLSMALLINT columni;
  char prikey[SMLSIZE];
  char attrs[SMLSIZE];
  char where[SMLSIZE];
  char order[SMLSIZE];
  int execute(SQLCHAR *sql);
private:
  SQLHENV env;
  SQLRETURN ret;
};

