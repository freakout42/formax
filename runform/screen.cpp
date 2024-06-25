#include <assert.h>
#include <cstdarg>
#include <stdlib.h>
#include <string.h>
#include <termio.h>
#include <term.h>
#include "version.h"
#include "curkeys.h"

static attrel attrels[] = {

  { COL_DEFAULT,         A_NORMAL,          -1,            -1 },            /* default */
  { COL_BLACK,           A_NORMAL,          COLOR_BLACK,   -1 },            /* black */
  { COL_RED,             A_NORMAL,          COLOR_RED,     -1 },            /* red */
  { COL_GREEN,           A_NORMAL,          COLOR_GREEN,   -1 },            /* green */
  { COL_YELLOW,          A_NORMAL,          COLOR_YELLOW,  -1 },            /* yellow */
  { COL_BLUE,            A_NORMAL,          COLOR_BLUE,    -1 },            /* blue */
  { COL_MAGENTA,         A_NORMAL,          COLOR_MAGENTA, -1 },            /* magenta */
  { COL_CYAN,            A_NORMAL,          COLOR_CYAN,    -1 },            /* cyan */
  { COL_WHITE,           A_NORMAL,          COLOR_WHITE,   -1 },            /* white */

  { TEXTCOLOR,           A_NORMAL,          0,             0 },             /* text cell */
  { EOFCOLOR,            A_NORMAL,          COLOR_GREEN,   -1 },            /* eof cell */
  { ERRORCOLOR,          A_BLINK,           COLOR_RED,     -1 },            /* error cell */
  { VALUECOLOR,          A_NORMAL,          0,             0 },             /* value cell */
  { FORMULACOLOR,        A_NORMAL,          0,             0 },             /* formula cell */
  { STRINGCOLOR,         A_NORMAL,          0,             0 },             /* string cell */
  { BLANKCOLOR,          A_NORMAL,          0,             0 },             /* blank cell */
  { COMMANDCOLOR,        A_UNDERLINE,       COLOR_BLUE,    -1 },            /* command cell */

  { HIGHLIGHTCOLOR,      A_REVERSE,         COLOR_BLUE,    COLOR_WHITE },   /* highlighted current cell */
  { HIGHLIGHTERRORCOLOR, A_REVERSE|A_BLINK, COLOR_RED,     COLOR_WHITE },   /* highlighted current cell with error */
  { HEADERCOLOR,         A_REVERSE,         COLOR_CYAN,    COLOR_BLACK },   /* column and row headers */
  { CURHEADERCOLOR,      A_BOLD,            COLOR_WHITE,   COLOR_BLUE },    /* current col/row header */
  { MARKCOLOR,           A_REVERSE,         COLOR_MAGENTA, COLOR_YELLOW },  /* marked range info */
  { AUTOCALCCOLOR,       A_REVERSE,         0,             0 },             /* autocalc info */
  { FORMDISPLAYCOLOR,    A_REVERSE,         0,             0 },             /* formula display info */
  { MESSAGECOLOR,        A_BOLD|A_BLINK,    0,             0 },             /* messages */
  { PROMPTCOLOR,         A_BOLD,            0,             0 },             /* prompt */
  { INPUTCOLOR,          A_REVERSE,         0,             0 },             /* editor */
  { CELLCONTENTSCOLOR,   A_NORMAL,          0,             0 },             /* cell content info */

  { COL_UNDEF, 0, 0, 0 }
};

static int mcode2att(int colcode) {
return attrels[colcode].cattr;
}

void Screen::setcolor(int pairi) {
if (colors && (attrels[pairi].foreg || attrels[pairi].backg))
  wattron(wndw, COLOR_PAIR(pairi));
}

void Screen::uncolor(int pairi) {
if (colors && (attrels[pairi].foreg || attrels[pairi].backg))
  wattroff(wndw, COLOR_PAIR(pairi));
}

int Screen::init() {
struct termios termio;
int i;
if ((wndw = initscr()) == NULL) return 1;
assert(wndw == stdscr);
if (has_colors() && !monochrome) {
  colors = 1;
  start_color();
  use_default_colors();
  for (i=0; i<COL_UNDEF; i++) {
    assert(attrels[i].ccode == i);
    init_pair(i, attrels[i].foreg, attrels[i].backg);
  }
}
tcgetattr (0, &termio); /* give me all emacs-controls */
termio.c_cc[VINTR] = 0; /* ctrl-c */
termio.c_cc[VSUSP] = 0; /* ctrl-z */
#ifdef VLNEXT
termio.c_cc[VLNEXT] = 0;/* ctrl-v */
#endif
tcsetattr (fileno(stdin), TCSANOW, &termio);
noecho();
cbreak();
nonl();
keypad(stdscr,TRUE);
getmaxyx(stdscr, ysiz, xsiz);
return 0;
}

void Screen::wbox() {
box(wndw, 0, 0);
}

void Screen::refresh() {
wrefresh(wndw);
}

void Screen::close() {
endwin();
}

int Screen::getkey() {
int ch;
//return wgetch(stdscr);
//wrefresh(stdscr);
ch = wgetch(stdscr);
switch (ch)
 {
  case KEY_BS:         return KEY_BACKSPACE;
  case KEY_RETURN:
  case KEY_RRETURN:    return KEY_ENTER;
  case KEY_LL:         return KEY_END;
  case KEY_F0:         return KEY_F(10);
  case KEY_CTRL('A'):  return KEY_HOME;
  case KEY_CTRL('B'):  return KEY_LEFT;
  case KEY_CTRL('D'):  return KEY_DC;
  case KEY_CTRL('E'):  return KEY_END;
  case KEY_CTRL('F'):  return KEY_RIGHT;
  case KEY_CTRL('G'):  return KEY_BTAB;
  case KEY_CTRL('N'):  return KEY_DOWN;
  case KEY_CTRL('O'):  return KEY_IC;
  case KEY_CTRL('P'):  return KEY_UP;
  case KEY_CTRL('R'):  return KEY_PPAGE;
  case KEY_CTRL('V'):  return KEY_NPAGE;
 }
return ch;
}

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
int oldy, oldx;
int color;
va_start (args, format);
vsnprintf (s, sizeof(s), format, args);
va_end (args);
color = mcode2att(colcode & TYPEM);
switch (colcode & BIMASK) {
  case BOLD:   color |= A_BOLD;      break;
  case ITALIC: color |= A_UNDERLINE; break;
}
colcode &= TYPEM;
attrs(color);
setcolor(colcode);
getyx (stdscr, oldy, oldx);
mvwprintw (wndw, y<0 ? ysiz+y : y, x<0 ? xsiz+x : x, "%-*s", width, s);
move (oldy, oldx);
uncolor(colcode);
attrs(A_NORMAL);
}
