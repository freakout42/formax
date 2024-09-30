#include "curkeys.h"
class Screen {
public:
  Screen();
	int init();
  void createwindow(int y, int x, int py, int px);
  void deletewindow();
  void refr();
  void redraw();
	void closedisplay();
  void wmov(int y, int x);
	void wera();
	void wbox();
	char *msg(int num);
  int setattributs(int attrib);
  void writes(int y, int x, char *str);
  void writef(int y, int x, int colcode, int width, const char *format, ...);
  void toggle();
  int wgetc();
	int getkb();
  int sedit(char *toe, int pos, ftype fty, int len);
  int getst(int y, int x, int width, int att, char *s, int pos, char *legal, int max, int *chg);
  int ysiz;
  int xsiz;
protected:
  WINDOW *wndw;
  void noutrefr();
private:
  void setcolor(int pairi);
  void uncolor(int pairi);
  void setcode(int colcode);
};

#define TYPEM  0x1fu
#define BIMASK 0xc0u
#define BOLD   0x40u
#define ITALIC 0x80u

enum {
  COL_DEFAULT,         /* = 0 (-1,-1) */
  COL_BLACK,           /* = 1 */
  COL_RED,             /* = 2 */
  COL_GREEN,           /* = 3 */
  COL_YELLOW,          /* = 4 */
  COL_BLUE,            /* = 5 */
  COL_MAGENTA,         /* = 6 */
  COL_CYAN,            /* = 7 */
  COL_WHITE,           /* = 8 */
  TEXTCOLOR,           /* = 9 */
  EDITCOLOR,           /* =10 */
  ERRORCOLOR,          /* =11 */
  VALUECOLOR,          /* =12 */
  FORMULACOLOR,        /* =13 */
  STRINGCOLOR,         /* =14 */
  BLANKCOLOR,          /* =15 */
  COMMANDCOLOR,        /* =16 */
  COL_CURRENT,         /* =17 */
  COL_FIELD,           /* =18 */
  COL_QUERY,           /* =19 */
  COL_HEADER,          /* =20 */
  COL_NEWREC,          /* =21 */
  COL_COMMIT,          /* =22 */
  COL_DELETED,         /* =23 */
  MESSAGECOLOR,        /* =24 */
  PROMPTCOLOR,         /* =25 */
  INPUTCOLOR,          /* =26 */
  CELLCONTENTSCOLOR,   /* =27 */
  COL_UNDEF
};

typedef struct attrel {
  int ccode;
  int cattr;
  int foreg;
  int backg;
} attrel;

//int mcode2att(int colcode);
//void mcolor0(void);
//void setcolor(int pairi);
//void uncolor(int pairi);

//CSI  Ps SP q
//ESC[2 q
//          Set cursor style (DECSCUSR, VT520).
//            Ps = 0  -> blinking block.
//            Ps = 1  -> blinking block (default).
//            Ps = 2  -> steady block.
//            Ps = 3  -> blinking underline.
//            Ps = 4  -> steady underline.
//            Ps = 5  -> blinking bar (xterm).
//            Ps = 6  -> steady bar (xterm).
#define A_BLOCKCURSOR "\033[2 q"
#define A_LINECURSOR  "\033[6 q"

/* application key codes get translated only for main loop switch */
enum { KEF_NOOP = 1, KEF_NOOP2, KEF_DEL = 127, KEF_HELP = 512,
       KEF_LIST, KEF_COPY, KEF_PASTE, KEF_COPYREC, KEF_QUERY, KEF_EXIT, KEF_QUIT, KEF_CANCEL,
//     List 513       514        515          516        517       518       519         520
       KEF_HOME, KEF_LEFT, KEF_DELETE, KEF_INSERT, KEF_END, KEF_RIGHT,   KEF_PREFLD, KEF_BACKDEL, KEF_NXTFLD,
//     Home 521  Left 522  Del    523  Ins    524  End 525  Right 526    Btab   527  Backspace    Tab    529
       KEF_REFRESH, KEF_COMMIT, KEF_NXTREC, KEF_PREREC, KEF_PRESETR, KEF_NXTSETR, KEF_MENU, KEF_KEYHELP,
//             530         531         532         534          535          536       537          538
       KEF_NAVI0, KEF_NAVI1, KEF_NAVI2, KEF_NAVI3, KEF_NAVI4, KEF_NAVI5, KEF_NAVI6, KEF_NAVI7, KEF_NAVI8, KEF_NAVI9,
//           539        540        541        542        543        544        545        546        547        548
       KEF_NAVI10, KEF_NAVI11, KEF_NAVI12
//            549         550         551
};
#define KEF_NAVI(n) (KEF_NAVI0+(n))
