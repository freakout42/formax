#include "curkeys.h"
class Screen {
public:
	int init();
  void cwin(int y, int x, int py, int px);
  void dwin();
  void refr();
	void dclose();
  void wmov(int y, int x);
	void weras();
	void wbox();
	char *msg(int num);
  int attrs(int attrib);
  void writes(int y, int x, char *str);
  void writef(int y, int x, int colcode, int width, char *format, ...);
	int getkey();
  int sedit(char *toe);
  int getst(int y, int x, int width, int att, char *s, int pos, char *legal, int max, int *chg);
  int ysiz;
  int xsiz;
protected:
  WINDOW *wndw;
private:
  void setcolor(int pairi);
  void uncolor(int pairi);
};

#define CB f.b[f.curblock]
#define CF f.l[f.curfield]

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
  EOFCOLOR,            /* =10 */
  ERRORCOLOR,          /* =11 */
  VALUECOLOR,          /* =12 */
  FORMULACOLOR,        /* =13 */
  STRINGCOLOR,         /* =14 */
  BLANKCOLOR,          /* =15 */
  COMMANDCOLOR,        /* =16 */
  COL_CURRENT,         /* =17 */
  COL_FIELD,           /* =18 */
  HEADERCOLOR,         /* =19 */
  CURHEADERCOLOR,      /* =20 */
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
