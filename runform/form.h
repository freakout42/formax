class Form: public Record {
public:
  char *id;
  char *name;
  char *title;
  rError rerror;
  rBlock rblock;
  rPage rpage;
  rMap rmap;
  Qdata e;
  Screen d;
  Block b[NBLOCKS];
  Page  p[NBLOCKS];
  int numblock;
  int numpage;
  int curblock;
  int curfield;
  int lastkey;
  int dirty;
  int *init();
  int fill(int id);
  void clear();
  int run();
private:
};
