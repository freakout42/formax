// the field
class Field {
public:
  char name[SMLSIZE];
  int dlen;
  int line;
  int col;
  int init(Qdata *fld, int rix);
private:
};
