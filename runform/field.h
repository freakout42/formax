// the field
class Field {
public:
  char name[SMLSIZE];
  int dlen;
  int line;
  int col;
  char qhuman[SMLSIZE];
  int init(Qdata *fld, int rix);
  void fshow(int cur);
private:
};
