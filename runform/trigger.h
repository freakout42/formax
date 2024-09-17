/* trigger data body should be in map */
enum ttyp { TRT_ENTERFORM, TRT_NEXTITEM=100 };
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
