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
  int fedit();
  int fexit();
  int fquit();
  int fquery();
};
