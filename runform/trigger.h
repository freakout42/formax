/* trigger data body in map page_id > 10000 */
class Trigger {
public:
  int map_id;
  char *body;
  int init(Qdata *trg, int rix, rMap *map);
  void destroy();
  char *jsexecdirect(char *prg, int siz);
  char *jsexec(char *av0);
  char *execute(char *av0);
  int trgblk;
  int trgfld;
  int trgtyp;
  int trglng;
  int index;
  int fieldindex;
private:
};
