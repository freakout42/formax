// event functions - the workhorse
class Function {
public:
  int dispatch();
protected:
private:
  void fmove(int bi, int fi);
  void fstartup();
  int fedit();
  int fexit();
  int fquit();
  void fquery();
};
