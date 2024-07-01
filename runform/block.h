// the block
class Block: public Record {
public:
  int init(Qdata *blk, int rix);
  void addattr(Field *att);
  Field *bflds[NFIELD1];
  int bnumfs;
private:
};
