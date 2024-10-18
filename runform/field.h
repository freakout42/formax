/* the field */
class Field {
public:
  int field_id;
  int index;
  char column[SMLSIZE];
  int sequencenum;
  int blockindex;
  int trg_postchange;
  char querywhere[MEDSIZE];
  int basetable;
  int isprimarykey;
  int enterable;
  char helptext[SMLSIZE];
  char defaultval[MEDSIZE];
  int init(Qdata *fld, int rix, Block *bs);
  void clear();
  int toggle(char *val);
  int increment(char *val, int ival);
  int edit(int pos);
  int noedit();
  void show();
  char **valuep();
  char **valuep(int row);
private:
  int line;
  int col;
  int pageindex;
  int validate(char **c, char *buf);
  ftype fldtype();
  ftype fieldtype;
  int fieldlen;
  char queryhuman[SMLSIZE];
  char currentval[SMLSIZE];
  int displaylen;
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
