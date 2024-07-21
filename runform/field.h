// the field
class Field {
public:
  char name[SMLSIZE];
  int blkn;
  int pgen;
  int dlen;
  int line;
  int col;
  int blockindex;
  int pageindex;
  int sequencenum;
  int isprimarykey;
  ftype fieldtype;
  int fieldlen;
  int basetable;
  char queryhuman[SMLSIZE];
  char querywhere[MEDSIZE];
  int init(Qdata *fld, int rix);
  void show(int cur);
  int edit(int pos);
  void clear();
private:
  char **valuep();
};
