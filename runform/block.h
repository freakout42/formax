// the block
class Block: public Record {
public:
  int init(Qdata *blk, int rix);
  void addattr(int att);
  int select();
  int bnumfs;
private:
  int bflds[NFIELD1];
};
