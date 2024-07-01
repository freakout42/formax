// the block
class Block: public Record {
public:
  int init(Qdata *blk, int rix);
  void addattr(char *att);
  char battrs[SMLSIZE];
private:
};
