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
  char *bindv[NBINDPA];
  char whereorder[MEDSIZE];
  int connect(char *dsn);
  int commit();
  int rollback();
  void disconnect();
  int ropen();
  void rclose();
  int clear();
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
  int execute(SQLCHAR *sql, char *bndv[]);
private:
  SQLHENV env;
  SQLRETURN ret;
};

