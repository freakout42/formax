// the field
class Field {
public:
  int field_id;
  char name[SMLSIZE];
  int sequencenum;
  char querywhere[MEDSIZE];
  int isprimarykey;
  int init(Qdata *fld, int rix);
  void clear();
  int edit(int pos);
  void show(int cur);
private:
  char **valuep();
  int blkn;
  int pgen;
  int line;
  int col;
  int blockindex;
  int pageindex;
  ftype fieldtype;
  int fieldlen;
  int basetable;
  char queryhuman[SMLSIZE];
  int displaylen;
  char defaultval[MEDSIZE];
  int enterable;
  int queryable;
  int updateable;
  int updnulable;
  int mandatory;
  int uppercase;
  char lovtitle[SMLSIZE];
  int lov_id;
  int lovi_id;
  int lowvalue;
  int highvalue;
  char validreg[SMLSIZE];
  char helptext[SMLSIZE];
};
