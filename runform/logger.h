class Logger {
public:
  void init();
  void setlogfile(char *lg3);
  void logf(char *format, ...);
private:
  char logpath[SMLSIZE];
  int session;
};
