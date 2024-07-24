// storage class for query results
class Qdata {
public:
  int rows;
  int cols;
  char **w(int row, int col);
  char *v(int row, int col);
  char *c(int row, int col);
  int m(int row, int col);
  int n(int row, int col);
  int alloc(int coln);
  void splice(int rown);
  Qdata();
  ~Qdata();
  void freed();
private:
  char *(*d)[];
  int allocatedrows; // flag for filled
};
