#include "curkeys.h"
class Screen {
public:
	int init();
  void createwindow(int y, int x, int py, int px);
  void deletewindow();
  void refr();
	void closedisplay();
  void wmov(int y, int x);
	void wera();
	void wbox();
	char *msg(int num);
  int setattributs(int attrib);
  void writes(int y, int x, char *str);
  void writef(int y, int x, int colcode, int width, char *format, ...);
  void toggle();
	int getkb();
	int getkey();
  int sedit(char *toe, int pos);
  int getst(int y, int x, int width, int att, char *s, int pos, char *legal, int max, int *chg);
  int ysiz;
  int xsiz;
protected:
  WINDOW *wndw;
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
  MARKCOLOR,           /* =21 */
  AUTOCALCCOLOR,       /* =22 */
  FORMDISPLAYCOLOR,    /* =23 */
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

#define A_NORMAL     0L
#define A_ATTRIBUTES NCURSES_BITS(~(1UL - 1UL),0)
#define A_CHARTEXT  (NCURSES_BITS(1UL,0) - 1UL)
#define A_COLOR      NCURSES_BITS(((1UL) << 8) - 1UL,0)
#define A_STANDOUT   NCURSES_BITS(1UL,8)
#define A_UNDERLINE  NCURSES_BITS(1UL,9)
#define A_REVERSE    NCURSES_BITS(1UL,10)
#define A_BLINK      NCURSES_BITS(1UL,11)
#define A_DIM        NCURSES_BITS(1UL,12)
#define A_BOLD       NCURSES_BITS(1UL,13)
#define A_PROTECT    NCURSES_BITS(1UL,16)

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

enum { KEF_HELP = 0777+1, KEF_LIST, KEF_COPY, KEF_PASTE, KEF_COPYREC, KEF_INSREC, KEF_QUERY, KEF_EXIT, KEF_CANCEL,
//     F1                    F2        F3        F4         F5           F6          F7         F8        F9
       KEF_HOME, KEF_LEFT, KEF_DELETE, KEF_END, KEF_RIGHT,   KEF_PREFLD, KEF_BACKDEL, KEF_NXTFLD, KEF_REFRESH,
//     Home      Left      Del         End      Right        Btab        Backspace    Tab         Ctrl-L
       KEF_COMMIT, KEF_NXTREC, KEF_INSERT, KEF_PREREC, KEF_PRESETR, KEF_NXTSETR, KEF_MENU,
       KEF_NAVI0, KEF_NAVI1, KEF_NAVI2, KEF_NAVI3, KEF_NAVI4, KEF_NAVI5, KEF_NAVI6, KEF_NAVI7, KEF_NAVI8, KEF_NAVI9
};
#define KEF_NAVI(n) (KEF_NAVI0+n)
