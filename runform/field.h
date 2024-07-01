// the field
class Field {
public:
  char name[SMLSIZE];
  int dlen;
  int line;
  int col;
  Block *blk;
  char qhuman[SMLSIZE];
  char qwhere[MEDSIZE];
  int init(Qdata *fld, int rix);
  void fshow(int cur);
private:
};
