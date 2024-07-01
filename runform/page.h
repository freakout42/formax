class Page: public Screen {
public:
  int init(Qdata *pag, int rix);
  void create();
  void destroy();
  char name[SMLSIZE];
  int maps(Qdata *rmap);
  int wait();
  int message(int num, char *pnt);
  Field *pflds[NFIELD1];
  int pnumfs;
protected:
  int vwpy0;
  int vwpx0;
  int border;
private:
  char *map[NLINES];
  void wfield(int fno);
};
