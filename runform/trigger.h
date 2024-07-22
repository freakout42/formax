class Trigger {
public:
  int init(Qdata *trg, int rix);
  char name[SMLSIZE];
private:
  int trgblk;
  int trgfld;
  int trgtyp;
  char body[MEDSIZE];
};
