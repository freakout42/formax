class Form: public Record {
public:
  Block *init();
  int fill(int id);
  int run();
private:
  char *id;
  char *name;
  char *title;
};
