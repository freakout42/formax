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
  int bintoggle(char *val); /* toggle a binary field between true and false */
  int increment(char *val, int ival); /* increment and decrement integer field */
  void setcond(char *cond); /* set the query condition in human form */
  int edit(int pos);        /* edit field in various methods determined by pos */
  int noedit();             /* not an editable field */
  void show();              /* format the field for display */
  char **valuep();          /* current rows field value pointer */
  char **valuep(int row);   /* rows field value pointer */
  void destroy();
  void clear();
private:
  int line;                 /* y position on window starting 0 */
  int col;                  /* x position on window starting 0 */
  int validate(char **c, char *buf); /* validate the edited new value */
  ftype fldtype();          /* more detailed field type */
  ftype fieldtype;          /* static field type */
  int fieldlen;             /* field byte length */
  int decimalen;            /* decimal places for float type */
  int displaylen;           /* display length on window */
  int alignment;            /* display alignment =0 left =1 right */
  int queryable;            /* if the field can be queried */
  int updateable;           /* if the field can be updated */
  int updnulable;           /* if the field can be updated when NULL */
  int mandatory;            /* if the field value is requeried */
  char lovtitle[8];         /* title for list of values */
  int lov_id;               /* block id for list of values */
  int lovi_id;              /* page id for list of values */
  int lowvalue;             /* low range value */
  int highvalue;            /* high range value */
  char validreg[TNYSIZE];   /* validation regex pattern */
};
