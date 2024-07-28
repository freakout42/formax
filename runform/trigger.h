enum ttyp { TRT_ENTERFORM, TRT_NEXTITEM };
class Trigger {
public:
  int init(Qdata *trg, int rix);
  int jsexec();
  int triggerid();
private:
  int trgfld;
  int trgtyp;
  char name[SMLSIZE];
  char body[MEDSIZE];
};
