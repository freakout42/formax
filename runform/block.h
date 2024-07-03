// the block
class Block: public Record {
public:
  int init(Qdata *blk, int rix);
  void addattr(int att);
  int select();
  int update(int row, int col);
  int bnumfs;             /* number of fields */
  int bprikf;             /* primary key field */
  int bcur;               /* current record */
  int bcurf;              /* current field */
  int bflds[NFIELD1];     /* fields */
  char *cn(int c);
  int bdirty;
private:
};
