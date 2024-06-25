class Form: public Record {
public:
  char *id;
  char *name;
  char *title;
  Block b[NBLOCKS];
  Page  p[NBLOCKS];
  int nb; /* number of blocks */
  int np; /* number of pages */
  int cb; /* current block */
  int cf; /* current field */
  int lk; /* last key */
  Block *init();
  int fill(int id);
  int run();
private:
};
