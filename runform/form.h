class Form: public Record {
public:
  Block b[NBLOCKS];
  Page  p[NBLOCKS];
  int cb;
  int cf;
  Block *init();
  int fill(int id);
  int run();
  char *id;
  char *name;
  char *title;
private:
};
