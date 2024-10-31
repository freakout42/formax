/* event functions - the workhorse */
class Function {
public:
  int dispatch();
  int next_item();
  int previous_item();
  int next_block();
  int previous_block();
  int next_record();
  int previous_setrecords();
  int next_setrecords();
  int previous_record();
  int exec_query();
  int edittrg(char *buf);
  char *etrigger(int tid);
protected:
private:
  int notrunning;
  int changed;
  int edittrgtyp;
  int editrigger(int tid);
  int qtrigger(int tid);
  int qtrigger(int tid, int bid, int fid);
  char *trigger(int tid);
  int triggern(int tid);
  int enter_the_form();
  void enter_record(int rid);
  int refresh_screen();
  int help_item();
  int keys_help();
  int aboutwin();
  int edit_map();
  int edit_file();
  int switch_mode(fmode mod);
  int fcopyrec();
  int fcopy();
  int fpaste();
  void fgoto(int fld, int row);
  int fmove(int bi, int fi);
  int fmover(int rn, int ri);
  int goto_cell();
  void fwindow();
  int ftoggle();
  int fincrement(int ival);
  int fedit(int pos);
  int fexit();
  int fquit();
  int insert_record();
  int create_record();
  int enter_query(Block *blk);
  int execute_query();
  int delete_record();
  int destroy_record();
  int clear_record();
};
