/* event functions - the workhorse */
class Function {
public:
  int dispatch();            /* central function dispatcher */
  int next_item();           /* goto next field */
  int previous_item();       /* goto previous field */
  int next_block();          /* goto next block */
  int previous_block();      /* goto previous block */
  int next_record();         /* goto next record */
  int previous_record();     /* goto previous record */
  int next_setrecords();     /* goto next set of records */
  int previous_setrecords(); /* goto previous set of records */
  void fgoto(char *sel);     /* goto selected field */
  void fsearch(char *rex);   /* search in all fields for matching regex */
  int exec_query();          /* execute query */
  int edittrg(char *buf, int typ); /* edit field with trigger */
protected:
private:
  int notrunning;            /* running flag */
  int changed;               /* field change flag */
  int editrigger(int tid);   /* edit trigger */
  int qtrigger(int tid);     /* search trigger index in current field */
  int qtrigger(int tid, int bid, int fid); /* ... in specified field */
  char *trigger(int tid, char *av0); /* fire trigger with arguments */
  int triggern(int tid);     /* fire trigger in current field */
  int triggers(int tid, char *buf); /* fire trigger _FOR_ current field */
  int enter_the_form();      /* enter the form */
  void enter_record(int rid); /* enter a record */
  int refresh_screen();      /* refresh all windows */
  int help_item();           /* display help on field */
  int keys_help();           /* display key help popup */
  int aboutwin();            /* display about popup */
  int edit_map();            /* edit a map */
  int edit_file();           /* edit a file */
  int switch_mode(fmode mod); /* switch between modes */
  int fcopyrec();            /* copy field value from previous record */
  int fcopy();               /* copy to internal clipboard */
  int fpaste();              /* paste from internal clipboard */
  int fmove(int bi, int fi); /* move relative block and field */
  int fmover(int rn, int ri); /* move relative record */
  int goto_cell();           /* goto selected field */
  int search_cell();         /* search for given regex */
  void fwindow();            /* adjust top record according to current record */
  int ftoggle();             /* toggle binary field also edit with trigger */
  int fincrement(int ival);  /* increment integer field */
  int fedit(int pos);        /* decrement integer field */
  int fexit();               /* exit form */
  int fquit();               /* quit form */
  int insert_record();       /* goto insert mode */
  int create_record();       /* create the new record */
  int enter_query(Block *blk); /* enter human query mode */
  int execute_query();       /* execute human query */
  int delete_record();       /* goto delete mode */
  int destroy_record();      /* delete the record */
  int clear_record();        /* clear record from window */
};
