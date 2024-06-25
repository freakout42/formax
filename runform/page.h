class Page: public Screen {
public:
  int init(Qdata pag, int rix);
  void create();
  char *name;
  int maps(rMap *rmap);
  int wait();
protected:
  int vwpy0;
  int vwpx0;
  int border;
private:
  char *map[NLINES];
};
