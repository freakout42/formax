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
  int id;                        /* connection id 0..4 */
  odrvr drv;                     /* driver instance */
  char dbmsname[TNYSIZE];        /* database vendor string */
  char dbmsver[TNYSIZE];         /* database version string */
  char driver_odbc_ver[TNYSIZE]; /* odbc driver version string */
  char driver_ver[TNYSIZE];      /* driver version string */
  char table[TNYSIZE];           /* table name */
  int blockfields[NFIELD1];      /* array index l[] fields/columns */
  char condition[NORSIZE];       /* where clause */
  char whereorder[NORSIZE];      /* default where / order clause */
  SQLCHAR *querystr;             /* full sql query */
  char *bindv[NBINDPA];          /* array of bind variables - all char* */
  int connect(char *dsn);        /* connect table to dsn */
  int connect(Record r);         /* connect table to another record instance dsn */
  int commit();                  /* commit transaction */
  int rollback();                /* rollback transaction */
  void disconnect();             /* disconnect from dsn */
  int ropen();                   /* open the table */
  void rclose();                 /* close the table */
  int clear();                   /* clear current row */
  int query();                   /* select rows on current condition */
  int execute();                 /* execute build query */
  int execdirect(char *sql);     /* sql direct */
  Qdata *q;                      /* raw data in two dimension array */
protected:
  SQLRETURN ret;                 /* odbc parameters */
  SQLHSTMT stmt;                 /* sql statement */
  SQLSMALLINT columni;           /* column count */
  SQLSMALLINT querycols;         /* returned column count */
  char prikey[TNYSIZE];          /* primary key column */
  char attrs[NORSIZE];           /* column list */
  char where[NORSIZE];           /* where clause */
  char order[NORSIZE];           /* order clause */
  char *cn(int c);               /* column/field name */
  int execute(SQLCHAR *sql, char *bndv[]);
  int complete();                /* cleanup select handle after execution and fetch */
  int fetch(int row);            /* fetch select data */
private:
  SQLHENV env;                   /* odbc parameters */
  SQLHDBC dbc;                   /* odbc handle */
  SQLUSMALLINT moreresults;      /* additional flags */
  void setdrv(char *dbmsname);   /* get database vendor type */
  void print(int, char*);        /* print column/field data in /rdb format */
  int failed(SQLSMALLINT hty);   /* odbc fail processing */
  int succeeded(SQLRETURN s);    /* odbc success processing */
  int fetchall();                /* fetch all select data */
};
