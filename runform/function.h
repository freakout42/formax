/* event functions - the workhorse */
class Function {
public:
  int dispatch();
  int next_item();
  int previous_item();
  int next_record();
  int previous_record();
protected:
private:
  int notrunning;
  int changed;
  char *trigger(int tid);
  int triggern(int tid);
  int enter_the_form();
  int refresh_screen();
  int help_item();
  int keys_help();
  int edit_map();
  int switch_mode(fmode mod);
  int fmove(int bi, int fi);
  int fmover(int ri);
  int ftoggle();
  int fincrement(int ival);
  int fedit(int pos);
  int fexit();
  int fquit();
  int insert_record();
  int create_record();
  int enter_query();
  int execute_query();
  int delete_record();
  int destroy_record();
  int clear_record();
};
#define JSEXT(func) jsval_t j_ ## func (struct js *js, jsval_t *args, int nargs);
//jsval_t j_next_item(struct js *js, jsval_t *args, int nargs);
JSEXT(next_item)
JSEXT(previous_item)
JSEXT(next_record)
JSEXT(previous_record)
