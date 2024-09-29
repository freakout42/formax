/* the field */
class Field {
public:
  int field_id;
  char name[SMLSIZE];
  int sequencenum;
  int blockindex;
  char querywhere[MEDSIZE];
  int isprimarykey;
  char helptext[SMLSIZE];
  int init(Qdata *fld, int rix, Block *bs);
  void clear();
  int toggle();
  int increment(int ival);
  int edit(int pos);
  int noedit();
  void show(int cur);
  char **valuep();
  char **valuepr(int row);
private:
//  int blkn;
//  int pgen;
  int line;
  int col;
  int pageindex;
  int validate(char **c, char *buf);
  ftype fldtype();
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
};
