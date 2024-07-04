// the block
class Block: public Record {
public:
  int init(Qdata *blk, int rix);
  int addattribute(int att);
  int select();
  int update(int row, int col);
  int fieldcount;
  int prikeycnt;
  int bcurf;              /* current field */
  int blockfields[NFIELD1]; /* array indexs l[] */
  int primarykeys[NPRIKEY];
  char *cn(int c);
  int currentrecord;
  int bdirty;
private:
};
