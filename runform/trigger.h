class Trigger {
public:
  int init(Qdata *trg, int rix);
  int jsexec(char *cod);
  char name[SMLSIZE];
  char body[MEDSIZE];
private:
  int trgblk;
  int trgfld;
  int trgtyp;
};
