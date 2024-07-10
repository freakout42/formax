// event functions - the workhorse
enum { PRE_FORM };
class Function {
public:
  int dispatch();
protected:
private:
  int notrunning;
  int trigger(int trg);
  int enter_the_form();
  int fmove(int bi, int fi);
  int fmover(int ri);
  int fedit(int pos);
  int fexit();
  int fquit();
  int insert_record();
  int create_record();
  int enter_query();
  int execute_query();
  int delete_record();
};
