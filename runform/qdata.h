/* storage class for query results
 * simple malloc'd **char array (d)
 * of malloc'd strings or NULL when empty/null
 * preallocation in 10 fold intervals
 * the w() method can access unallocated rows
 * in this case more rows are malloc'ed
 * the count for new rows increases by factor 10
 */
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
