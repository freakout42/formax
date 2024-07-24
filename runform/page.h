/* Pages are collections of display information, such as constant text.
 * All fields are displayed on some page.
 */
#define LINE0SIZE 80
class Page: public Screen {
public:
  int init(Qdata *pag, int rix);
  void create();
  void destroy();
  char name[SMLSIZE];
  int maps(Qdata *rmap);
  int wait();
  int message(int num, char *pnt);
protected:
  int vwpy0;
  int vwpx0;
  int border;
private:
  int page_id;
  char *map[NLINES];
};
