/* dynamic table setup from config data
 * or free access without sql-hassle
 */
class Block: public Record {
public:
  int block_id;
  int init(Qdata *blk, int rix);
  int addattribute(int att, void *fld);
  int select();
  int update(int row, int col);
  int insert(int row);
  int destroy(int r);
  int norec;
  int fieldcount;
  int prikeycnt;
  int blockfields[NFIELD1]; /* array index l[] */
  int primarykeys[NPRIKEY];
  char *cn(int c);
  fmode rmode;
  int currentrec;
  int toprec;
  int bdirty;
  int sequence; /* % 10 is blockindex for connection */
  int index;
private:
};
