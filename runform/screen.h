#include "curkeys.h"
class Screen {
public:
  Screen();
  int init();
  void closedisplay();     /* end curses mode and return to stdio */
  void wmov(int y, int x); /* move to window position */
  void toggle();           /* toggle insert replace mode */
  void writef(int y, int x, int colcode, int width, const char *format, ...);
  int sedit(char *toe, int pos, ftype fty, int len);
  int sedit(char *toe, int pos, ftype fty, int len, int col, int lin);
protected:
  int ysiz;                /* window horizontal size */
  int xsiz;                /* window vertical size */
  void createwindow(int y, int x, int py, int px);
  void deletewindow();     /* destroy window from curses */
  void refr();             /* refresh window to physical screen */
  void noutrefr();         /* curses wnoutrefresh() optimized update */
  void redraw();           /* full redraw the physical screen */
  void wera();             /* erase window */
  void wbox();             /* draw a box around window */
  int wadds(char *str);    /* write string to window */
  void wsleep(int sec);    /* sleep sec seconds */
  int fulledit(char *pth); /* full screen editor within a window */
  char *msg(int num);      /* get message string by id */
  void writes(int y, int x, char *str); /* write string to window */
  int getkb();             /* get next keycode from macro or terminal */
private:
  char *lclocale;          /* locale info */
  int cur_utf8;            /* UTF8 && CHARSET~utf8/i */
  WINDOW *wndw;            /* curses window structure */
  void setcolor(int pairi); /* change to a predefined color */
  void uncolor(int pairi); /* change back to uncolored */
  void setcode(int colcode); /* change to color/attribute combination */
  int setattributs(int attrib); /* change to attribute */
  int mapctrl(int code);   /* map ctrl-keys to curses KEY_* */
  int wgetc();             /* get key from physical keyboard */
                           /* get string from window */
  int getst(int y, int x, int width, int att, char *s, int pos, char *legal, int max, int *chg);
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
  COL_ERROR,           /* =11 */
  VALUECOLOR,          /* =12 */
  FORMULACOLOR,        /* =13 */
  STRINGCOLOR,         /* =14 */
  COL_BUSY,            /* =15 */
  COMMANDCOLOR,        /* =16 */
  COL_CURRENT,         /* =17 */
  COL_FIELD,           /* =18 */
  COL_QUERY,           /* =19 */
  COL_HEADER,          /* =20 */
  COL_NEWREC,          /* =21 */
  COL_COMMIT,          /* =22 */
  COL_DELETED,         /* =23 */
  COL_DATA,            /* =24 */
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
#define KEF_INS     (KEY_CTRL('J'))
