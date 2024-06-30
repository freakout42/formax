// event functions - the workhorse
class Function {
public:
  int dispatch();
protected:
private:
  void fmove(int bi, int fi);
  void fstartup();
  void fedit();
  int fexit();
  int fquit();
};
