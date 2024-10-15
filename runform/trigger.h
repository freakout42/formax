/* trigger data body in map page_id > 10000 */
class Trigger {
public:
  int map_id;
  char body[MEDSIZE];
  int init(Qdata *trg, int rix, rMap *map);
  char *jsexecdirect(char *prg, int siz);
  char *jsexec();
  int trgfld;
  int trgtyp;
  int index;
  int fieldindex;
private:
};
