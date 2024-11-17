/* the field */
class Field {
public:
  int field_id;             /* id in table fields */
  int index;                /* array index l[] */
  char column[30];          /* database column */
  int sequencenum;          /* number of field in block/record array index Block.blockfields[] */
  int blockindex;           /* array index of parent block b[] */
  int pageindex;            /* array index of displaying page p[] */
  int trg_postchange;       /* array index of trigger postchange is existing -1 if not */
  char querywhere[SMLSIZE]; /* sql query where clause build from queryhuman with colquery parser */
  char queryhuman[TNYSIZE]; /* "human" query condition for colquery parser */
  char *currentval;         /* last primary key value to detect record changed for enterecord trigger */
  int basetable;            /* database table */
  int isprimarykey;         /* primary key field */
  char *helptext;           /* text to be displayed for help */
  char *defaultval;         /* default value for insert new records and query where clause */
  int init(Qdata *fld, int rix, Block *bs);
  void rclose();
  void clear();
  int toggle(char *val);    /* toggle a binary field between true and false */
  int increment(char *val, int ival); /* increment and decrement integer field */
  void setcond(char *cond); /* set the query condition in human form */
  int edit(int pos);        /* edit field in various methods determined by pos */
  int noedit();             /* not an editable field */
  void show();              /* format the field for display */
  char **valuep();          /* current rows field value pointer */
  char **valuep(int row);   /* rows field value pointer */
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
