// the block
class Block: public Record {
public:
  int init(Qdata *blk, int rix);
  int addattribute(int att);
  int select();
  int update(int row, int col);
  int fieldcount;
  int bprikf;             /* primary key field */
  int bcur;               /* current record */
  int bcurf;              /* current field */
  int bflds[NFIELD1];     /* fields */
  int primarykeys[NPRIKEY];
  char *cn(int c);
  int bdirty;
private:
};
