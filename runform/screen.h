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
  int cur_utf8;            /* UTF8 && CHARSET~utf8/i */
  char cursesvariant;      /* ncurses:n ncursesw:w */
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
#ifdef UTF8
  int wadds(wchar_t *str); /* wide string to window */
#endif
  void wsleep(int sec);    /* sleep sec seconds */
  int fulledit(char *pth); /* full screen editor within a window */
  char *msg(int num);      /* get message string by id */
  void writes(int y, int x, char *str); /* write string to window */
  int getkey();            /* get next keycode to use lastgetch */
private:
  int str_pos(char *s, int f);
  char *str_sub(char *tg, char *s, int f, int l, int z);
  void cur_puts(int y, int x, char *s, int w);
  char *lclocale;          /* locale info */
  WINDOW *wndw;            /* curses window structure */
  void setcolor(int pairi); /* change to a predefined color */
  void uncolor(int pairi); /* change back to uncolored */
  void setcode(int colcode); /* change to color/attribute combination */
  int setattributs(int attrib); /* change to attribute */
  int wgetc();             /* get key from physical keyboard */
                           /* get string from window */
  int getkb();             /* get next keycode from macro or terminal */
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
enum { KEF_NOOP = 1, KEF_NOOP2, KEF_DEL = 127, KEF_HELP = -2047,
//                                             Help -2047
       KEF_LIST, KEF_COPY, KEF_PASTE, KEF_COPYREC, KEF_QUERY, KEF_EXIT, KEF_QUIT, KEF_CANCEL,
//     List -2046   -2045      -2044        -2043      -2042     -2041     -2040       -2039
       KEF_HOME, KEF_LEFT, KEF_DELETE, KEF_INSERT, KEF_END, KEF_RIGHT,   KEF_PREFLD, KEF_BACKDEL, KEF_NXTFLD,
//     Home-2038 Left-2037 Del  -2036  Ins  -2035  End-2034 Right -2033  Btab -2032  Backspac-2031 Tab -2030
       KEF_REFRESH, KEF_COMMIT, KEF_NXTREC, KEF_PREREC, KEF_PRESETR, KEF_NXTSETR, KEF_MENU, KEF_KEYHELP,
//           -2029       -2028       -2027       -2026        -2025        -2024     -2023        -2022
       KEF_NAVI0, KEF_NAVI1, KEF_NAVI2, KEF_NAVI3, KEF_NAVI4, KEF_NAVI5, KEF_NAVI6, KEF_NAVI7, KEF_NAVI8, KEF_NAVI9,
//         -2021      -2020      -2019      -2018      -2017      -2016      -2015      -2014      -2013      -2012
       KEF_NAVI10, KEF_NAVI11, KEF_NAVI12
//          -2011       -2010       -2009
};
#define KEF_NAVI(n) (KEF_NAVI0+(n))
#define KEF_INS     (KEY_CTRL('J'))
