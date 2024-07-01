// the block
class Block: public Record {
public:
  int init(Qdata *blk, int rix);
  void addattr(Field *att);
  int select();
  int bnumfs;
private:
  Field *bflds[NFIELD1];
};
