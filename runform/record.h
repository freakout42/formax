#include <sql.h>
#include <sqlext.h>
class Record {
public:
  SQLHDBC dbc;
  char table[SMLSIZE];
  void locale(char *lc);
  int connect(char *dsn);
  void disconnect();
  int open(SQLHDBC dbc0);
  void close();
  int query(char *attrs);
  char *c(int row, int col);
  int n(int row, int col);
  Qdata q;
protected:
  SQLHSTMT stmt;
  SQLSMALLINT columni;
  char prikey[SMLSIZE];
  char where[SMLSIZE];
  char order[SMLSIZE];
  int execute(SQLCHAR *sql);
private:
  SQLHENV env;
  SQLRETURN ret;
};
