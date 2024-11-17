/* simple ORM inspired from ActiveRecord
 * connect to ODBC DSN and allows queries
 * subclassed by the individual table classes or
 * by the dynamic table class Block
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
  char dbmsname[TNYSIZE];        /* database vendor string */
  char dbmsver[TNYSIZE];         /* database version string */
  char driver_odbc_ver[TNYSIZE]; /* odbc driver version string */
  char driver_ver[TNYSIZE];      /* driver version string */
  char table[TNYSIZE];           /* table name */
  char condition[NORSIZE];       /* default where clause */
  char whereorder[NORSIZE];      /* default order clause */
  SQLCHAR *querystr;             /* full sql query */
  char *bindv[NBINDPA];          /* array of bind variables - all char* */
  int connect(char *dsn);        /* connect table to dsn */
  int connect(Record r);         /* connect table to another record instance dsn */
  int commit();                  /* commit transaction */
  int rollback();                /* rollback transaction */
  void disconnect();             /* disconnect from dsn */
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
  char attrs[NORSIZE];
  char where[NORSIZE];
  char order[NORSIZE];
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

