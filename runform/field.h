/* the field */
class Field {
public:
  int field_id;
  int index;
  char column[30];
  int sequencenum;
  int blockindex;
  int pageindex;
  int trg_postchange;
  char querywhere[SMLSIZE];
  char queryhuman[TNYSIZE];
  char *currentval;
  int basetable;
  int isprimarykey;
  char *helptext;
  char *defaultval;
  int init(Qdata *fld, int rix, Block *bs);
  void rclose();
  void clear();
  int toggle(char *val);
  int increment(char *val, int ival);
  void setcond(char *cond);
  int edit(int pos);
  int noedit();
  void show();
  char **valuep();
  char **valuep(int row);
private:
  int line;
  int col;
  int validate(char **c, char *buf);
  ftype fldtype();
  ftype fieldtype;
  int fieldlen;
  int decimalen;
  int displaylen;
  int alignment;
  int queryable;
  int updateable;
  int updnulable;
  int mandatory;
  char lovtitle[8];
  int lov_id;
  int lovi_id;
  int lowvalue;
  int highvalue;
  char validreg[TNYSIZE];
};
