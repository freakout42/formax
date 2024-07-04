// the field
class Field {
public:
  char name[SMLSIZE];
  int dlen;
  int line;
  int col;
  int blockindex;
  int sequencenum;
  int isprimarykey;
  char queryhuman[SMLSIZE];
  char querywhere[MEDSIZE];
  int init(Qdata *fld, int rix);
  void show(int cur);
  int edit(int pos);
private:
};
