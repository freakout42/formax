/* Pages are collections of display information, such as constant text
 * All fields are displayed on some page
 * page_id==0 these pages are present in all forms - 3 pages
 * array index ==3 is the first user page
 */
#define LINE0SIZE 80
class Page: public Screen {
public:
  int page_id;
  int init(Qdata *pag, int rix);
  void create();
  void repaint();
  int showpopup();
  int editbuf(char *buf);
  int editmap(int pid);
  void destroy();
  char name[SMLSIZE];
  int maps(Qdata *rmap);
  int wait();
  int message(int num, const char *pnt);
protected:
  int vwpy0;
  int vwpx0;
  int popup;
  int border;
private:
  char *map[NLINES];
  void refrnopop();
};
