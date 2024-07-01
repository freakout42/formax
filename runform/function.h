// event functions - the workhorse
enum { PRE_FORM };
class Function {
public:
  int dispatch();
protected:
private:
  int run;
  int trigger(int trg);
  void fmove(int bi, int fi);
  int enter_the_form();
  int fedit();
  int fexit();
  int fquit();
  void fquery();
};
