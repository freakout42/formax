// the field
class Field {
public:
  char name[SMLSIZE];
  int dlen;
  int line;
  int col;
  int blk;
  int num;
  int isprimarykey;
  char qhuman[SMLSIZE];
  char qwhere[MEDSIZE];
  int init(Qdata *fld, int rix);
  void fshow(int cur);
  int ledit(int pos);
private:
};
