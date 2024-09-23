/* trigger data body in map page_id > 10000 */
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
  int fillbody(int qid);
};
