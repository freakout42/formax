/* simple ORM inspired from ActiveRecord
 * connect to ODBC DSN and allows queries
 * subclassed by the table class or
 * by dynamic table class Block
 */
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
  odrvr drv;
  char table[30];
//  char sqlcmd[MEDSIZE];
  char condition[SMLSIZE];
  char whereorder[SMLSIZE];
  SQLCHAR querystr[MEDSIZE];
  char *bindv[NBINDPA];
  int connect(char *dsn);
  int connect(Record r);
  int commit();
  int rollback();
  void disconnect();
  int ropen();
  void rclose();
  int clear();
  int query();
  int execdirect(char *sql);
  int execute();
  Qdata *q;
protected:
  SQLRETURN ret;
  SQLHSTMT stmt;
  SQLSMALLINT columni;
  SQLSMALLINT querycols;
  char prikey[TNYSIZE];
  char attrs[SMLSIZE];
  char where[SMLSIZE];
  char order[SMLSIZE];
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
  int fetchall();
};

