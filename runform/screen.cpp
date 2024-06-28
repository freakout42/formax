#include <assert.h>
#include <cstdarg>
#include <stdlib.h>
#include <string.h>
#include <termio.h>
#include <term.h>
#include <curses.h>
#include "runform.h"

static attrel attrels[] = {

  { COL_DEFAULT,         A_NORMAL,              -1,            -1 },            /* default */
  { COL_BLACK,           A_NORMAL,              COLOR_BLACK,   -1 },            /* black */
  { COL_RED,             A_NORMAL,              COLOR_RED,     -1 },            /* red */
  { COL_GREEN,           A_NORMAL,              COLOR_GREEN,   -1 },            /* green */
  { COL_YELLOW,          A_NORMAL,              COLOR_YELLOW,  -1 },            /* yellow */
  { COL_BLUE,            A_NORMAL,              COLOR_BLUE,    -1 },            /* blue */
  { COL_MAGENTA,         A_NORMAL,              COLOR_MAGENTA, -1 },            /* magenta */
  { COL_CYAN,            A_NORMAL,              COLOR_CYAN,    -1 },            /* cyan */
  { COL_WHITE,           A_NORMAL,              COLOR_WHITE,   -1 },            /* white */

  { TEXTCOLOR,           A_NORMAL,              0,             0 },             /* text cell */
  { EOFCOLOR,            A_NORMAL,              COLOR_GREEN,   -1 },            /* eof cell */
  { ERRORCOLOR,          A_BLINK,               COLOR_RED,     -1 },            /* error cell */
  { VALUECOLOR,          A_NORMAL,              0,             0 },             /* value cell */
  { FORMULACOLOR,        A_NORMAL,              0,             0 },             /* formula cell */
  { STRINGCOLOR,         A_NORMAL,              0,             0 },             /* string cell */
  { BLANKCOLOR,          A_NORMAL,              0,             0 },             /* blank cell */
  { COMMANDCOLOR,        A_UNDERLINE,           COLOR_BLUE,    -1 },            /* command cell */

  { COL_CURRENT,         A_REVERSE|A_UNDERLINE, COLOR_BLUE,    COLOR_WHITE },   /* current field */
  { COL_FIELD,           A_REVERSE,             COLOR_MAGENTA, COLOR_WHITE },   /* field */
  { HEADERCOLOR,         A_REVERSE,             COLOR_CYAN,    COLOR_BLACK },   /* column and row headers */
  { CURHEADERCOLOR,      A_BOLD,                COLOR_WHITE,   COLOR_BLUE },    /* current col/row header */
  { MARKCOLOR,           A_REVERSE,             COLOR_MAGENTA, COLOR_YELLOW },  /* marked range info */
  { AUTOCALCCOLOR,       A_REVERSE,             0,             0 },             /* autocalc info */
  { FORMDISPLAYCOLOR,    A_REVERSE,             0,             0 },             /* formula display info */
  { MESSAGECOLOR,        A_BOLD|A_BLINK,        0,             0 },             /* messages */
  { PROMPTCOLOR,         A_BOLD,                0,             0 },             /* prompt */
  { INPUTCOLOR,          A_REVERSE,             0,             0 },             /* editor */
  { CELLCONTENTSCOLOR,   A_NORMAL,              0,             0 },             /* cell content info */

  { COL_UNDEF, 0, 0, 0 }
};

static int mcode2att(int colcode) {
return attrels[colcode].cattr;
}

void Screen::setcolor(int pairi) {
if (!monochrome && (attrels[pairi].foreg || attrels[pairi].backg))
  wattron(wndw, COLOR_PAIR(pairi));
}

void Screen::uncolor(int pairi) {
if (!monochrome && (attrels[pairi].foreg || attrels[pairi].backg))
  wattroff(wndw, COLOR_PAIR(pairi));
}

int Screen::init() {
struct termios termio;
int i;
tcgetattr (0, &termio); /* give me all emacs-controls */
termio.c_cc[VINTR] = 0; /* ctrl-c */
termio.c_cc[VSUSP] = 0; /* ctrl-z */
#ifdef VLNEXT
termio.c_cc[VLNEXT] = 0;/* ctrl-v */
#endif
tcsetattr (0, TCSANOW, &termio);
if ((wndw = initscr()) == NULL) return 1;
assert(wndw == stdscr);
nonl();
noecho();
//cbreak();
keypad(stdscr,TRUE);
if (has_colors() && !monochrome) {
  start_color();
  for (i=0; i<COL_UNDEF; i++) {
    assert(attrels[i].ccode == i);
    init_pair(i, attrels[i].foreg, attrels[i].backg);
  }
use_default_colors();
//assume_default_colors(-1,-1);
} else monochrome = 1;
refr();
getmaxyx(stdscr, ysiz, xsiz);
return 0;
}

void Screen::cwin(int y, int x, int py, int px) {
wndw = newwin(y, x, py, px);
wattrset(wndw, A_NORMAL);
wattron(wndw, COLOR_PAIR(0));
}

void Screen::dwin() {
delwin(wndw);
}

void Screen::wbox() {
box(wndw, 0, 0);
}

void Screen::wmov(int y, int x) {
wmove(wndw, y, x);
}

void Screen::refr() {
wrefresh(wndw);
}

void Screen::dclose() {
endwin();
}

char *Screen::msg(int num) {
int i;
for (i=1; f.e->v(i,1); i++) if (f.e->n(i,1) == num) break;
return f.e->v(i,3);
}

int Screen::getkey() {
int ch;
ch = wgetch(stdscr);
switch (ch)
 {
//case KEY_CTRL('H'):
  case KEY_BS:         return KEY_BACKSPACE;
//case KEY_CTRL('J'):
  case KEY_RETURN:     return KEY_ENTER;      /* Commit Accept */
  case KEY_LL:         return KEY_END;
  case KEY_F0:         return KEY_F(10);
  case KEY_CTRL('@'):  return KEY_F(0);
  case KEY_CTRL('A'):  return KEY_HOME;
  case KEY_CTRL('B'):  return KEY_LEFT;
  case KEY_CTRL('C'):  return KEY_F(12);      /* Rollback Cancel */
  case KEY_CTRL('D'):  return KEY_DC;
  case KEY_CTRL('E'):  return KEY_END;
  case KEY_CTRL('F'):  return KEY_RIGHT;
  case KEY_CTRL('G'):  return KEY_BTAB;
  case KEY_CTRL('I'):  return KEY_TAB;
  case KEY_CTRL('K'):  return KEY_F(0);       /* Delete Record */
  case KEY_CTRL('L'):  return KEY_F(0);       /* Refresh */
//case KEY_CTRL('M'):
  case KEY_RRETURN:    return KEY_F(0);
  case KEY_CTRL('N'):  return KEY_DOWN;
  case KEY_CTRL('O'):  return KEY_IC;         /* Insert Toggle */
  case KEY_CTRL('P'):  return KEY_UP;
  case KEY_CTRL('Q'):  return KEY_F(0);       /* Query */
  case KEY_CTRL('R'):  return KEY_PPAGE;
  case KEY_CTRL('S'):  return KEY_F(0);
  case KEY_CTRL('T'):  return KEY_F(0);
  case KEY_CTRL('U'):  return KEY_F(0);
  case KEY_CTRL('V'):  return KEY_NPAGE;
  case KEY_CTRL('W'):  return KEY_F(0);
  case KEY_CTRL('Y'):  return KEY_F(0);       /* Insert Record */
  case KEY_CTRL('Z'):  return KEY_F(8);
 }
return ch;
}

#ifdef nonono
TAB	Next Field
F1	Help
F6	Insert Record
F7	Enter Query
F8	Execute Query
F9	List of Values
F10	Save
Shift + F4	Clear Record
Shift + F6	Delete Record
Ctrl + u	Clear Item
Ctrl + q	Exit Application
Ctrl + F1	Show Key Mapping
Ctrl + F4	Close Current Window
Ctrl + F6	Switch windows from front to back
Alt + a	Action Menu
Alt + e	Edit Menu
Alt + g	Go Menu
Alt + q	Query Menu
Alt + f	Folder Menu
Alt + t	Tools Menu
Alt + h	Help Menu
#endif

int Screen::attrs(int attrib) {
if (attrib & A_REVERSE) {
  if (enter_reverse_mode == NULL) /* terminal has no reverse mode */
    attrib |= A_STANDOUT;         /* use standout instead */
}
if (attrib & A_BLINK) {
  if (enter_blink_mode == NULL)   /* terminal has no blink mode */
    attrib |= A_STANDOUT;         /* use standout instead */
}
return wattrset(wndw, attrib);
}

void Screen::writes(int y, int x, char *str) {
writef(y, x, 0, strlen(str), str);
}

/* Prints a string in video memory at a selected location in a color */
void Screen::writef(int y, int x, int colcode, int width, char *format, ...) {
va_list args;
char s[MEDSIZE];
int color;
int oldy, oldx;

va_start (args, format);
vsnprintf (s, sizeof(s), format, args);
va_end (args);
color = mcode2att(colcode & TYPEM);
switch (colcode & BIMASK) {
  case BOLD:   color |= A_BOLD;      break;
  case ITALIC: color |= A_UNDERLINE; break;
}
attrs(color);
setcolor(colcode & TYPEM);
getyx(wndw, oldy, oldx);
mvwprintw(wndw, y<0 ? ysiz+y : y, x<0 ? xsiz+x : x, "%-*s", width, s);
wmov(oldy, oldx);
uncolor(colcode);
attrs(A_NORMAL);
}
