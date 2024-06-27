class Page: public Screen {
public:
  int init(Qdata *pag, int rix);
  void create();
  void destroy();
  char *name;
  int maps(Qdata *rmap);
  int wait();
  int message(int num);
protected:
  int vwpy0;
  int vwpx0;
  int border;
private:
  char *map[NLINES];
};
