// the block
class Block: public Record {
public:
  int init(Qdata *blk, int rix);
  int addattribute(int att);
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
  int currentrecord;
  int bdirty;
private:
};
