// the block
class Block: public Record {
public:
  int init(Qdata *blk, int rix);
  void addattr(int att);
  int select();
  int bnumfs;
  int bcurf;
  int bflds[NFIELD1];
  int bdirty;
private:
};
