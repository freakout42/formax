class Form: public Record {
public:
  char *id;
  char *name;
  char *title;
  rBlock rblock;
  rPage rpage;
  rMap rmap;
  Block b[NBLOCKS];
  Page  p[NBLOCKS];
  int nb; /* number of blocks */
  int np; /* number of pages */
  int cb; /* current block */
  int cf; /* current field */
  int lk; /* last key */
  int *init();
  int fill(int id);
  void clear();
  int run();
private:
};
