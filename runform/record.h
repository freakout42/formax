#ifndef __SQL_H
typedef void * SQLHDBC;
typedef void * SQLHENV;
typedef void * SQLHSTMT;
typedef unsigned char SQLCHAR;
typedef signed short int SQLSMALLINT;
typedef unsigned short int SQLUSMALLINT;
typedef SQLSMALLINT SQLRETURN;
#endif
class Record {
public:
  char table[SMLSIZE];
  char sqlcmd[MEDSIZE];
  SQLCHAR querystr[MEDSIZE];
  char *bindv[NBINDPA];
  char whereorder[MEDSIZE];
  int connect(char *dsn);
  int connect(Record r);
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
  enum odrvr drv;
  SQLRETURN ret;
  SQLHSTMT stmt;
  SQLSMALLINT columni;
  SQLSMALLINT querycols;
  char prikey[SMLSIZE];
  char attrs[SMLSIZE];
  char where[SMLSIZE];
  char order[SMLSIZE];
  int execdirect(SQLCHAR *sql);
  int execute(SQLCHAR *sql, char *bndv[]);
  int complete();
  int fetch(int row);
private:
  SQLHENV env;
  SQLHDBC dbc;
  SQLUSMALLINT moreresults;
  void setdrv(char *dbmsname);
  int failed(SQLSMALLINT hty);
  int succeeded(SQLRETURN s);
};

