/* Pages are collections of display information, such as constant text.
 * All fields are displayed on some page.
 */
#define LINE0SIZE 80
class Page: public Screen {
public:
  int page_id;
  int init(Qdata *pag, int rix);
  void create(int force);
  void repaint();
  int showpopup();
  void destroy(int force);
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
};
