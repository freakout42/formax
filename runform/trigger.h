class Trigger {
public:
  int init(Qdata *trg, int rix);
  int jsexec(); //char *cod);
  char *triggerid();
private:
  int trgblk;
  int trgfld;
  int trgtyp;
  char name[SMLSIZE];
  char body[MEDSIZE];
};
