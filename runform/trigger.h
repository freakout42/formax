/* trigger data body in map page_id > 10000 */
class Trigger {
public:
  int map_id;
  char body[MEDSIZE];
  int init(Qdata *trg, int rix, rMap *map);
  char *jsexecdirect(char *prog);
  char *jsexec();
  int trgfld;
  int trgtyp;
private:
};
