//#include <assert.h>
/* curses screen handling interface */
#include <cstdarg>
#include <unistd.h>
#ifdef WIN32
#undef USETERMIO
#endif
#ifdef USETERMIO
#include <termios.h>
#include <term.h>
#endif
#include <curses.h>
#include "runform.h"

#define nocurses(ret) if (screenclos) return ret

char cursesversion[8] = NCURSES_VERSION;
const char *cursesrun = NULL;

Screen::Screen() {
ysiz = 0;
cursesrun = curses_version();
}

/* curses attributes configuration array */
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
  { EDITCOLOR,           A_BOLD,                COLOR_BLUE,    COLOR_WHITE },   /* editor */
  { COL_ERROR,           A_BLINK,               COLOR_YELLOW,  COLOR_RED },     /* error */
  { VALUECOLOR,          A_NORMAL,              0,             0 },             /* value cell */
  { FORMULACOLOR,        A_NORMAL,              0,             0 },             /* formula cell */
  { STRINGCOLOR,         A_NORMAL,              0,             0 },             /* string cell */
  { COL_BUSY,            A_BLINK,               COLOR_YELLOW,  COLOR_BLUE },    /* busy */
  { COMMANDCOLOR,        A_UNDERLINE,           COLOR_BLUE,    -1 },            /* command cell */

  { COL_CURRENT,         A_REVERSE|A_UNDERLINE, COLOR_BLUE,    COLOR_WHITE },   /* current field */
  { COL_FIELD,           A_REVERSE,             COLOR_MAGENTA, COLOR_WHITE },   /* field */
  { COL_QUERY,           A_REVERSE,             COLOR_CYAN,    COLOR_BLACK },   /* query */
  { COL_HEADER,          A_BOLD,                COLOR_WHITE,   COLOR_BLUE },    /* status header */
  { COL_NEWREC,          A_REVERSE,             COLOR_YELLOW,  COLOR_BLACK },   /* insert */
  { COL_COMMIT,          A_BOLD,                COLOR_GREEN,   COLOR_BLACK },   /* commit key action */
  { COL_DELETED,         A_BLINK,               COLOR_RED,     COLOR_BLACK },   /* delete */
  { COL_DATA,            A_REVERSE,             COLOR_WHITE,   COLOR_BLACK },   /* data */
  { PROMPTCOLOR,         A_BOLD,                0,             0 },             /* prompt */
  { INPUTCOLOR,          A_REVERSE,             0,             0 },             /* editor */
  { CELLCONTENTSCOLOR,   A_NORMAL,              0,             0 },             /* cell content info */

  { COL_UNDEF, 0, 0, 0 }
};

/* symbolic color codes map to curses attribute */
static int mcode2att(int colcode) {
return attrels[colcode].cattr;
}

/* curses set attribute */
void Screen::setcolor(int pairi) {
nocurses();
if (!monochrome && (attrels[pairi].foreg || attrels[pairi].backg))
  wattron(wndw, COLOR_PAIR(pairi));
}

/* curses unset attribute */
void Screen::uncolor(int pairi) {
nocurses();
if (!monochrome && (attrels[pairi].foreg || attrels[pairi].backg))
  wattroff(wndw, COLOR_PAIR(pairi));
}

/* high level colcode to curses setting */
void Screen::setcode(int colcode) {
nocurses();
static int runningcolcode;
int color;
if (colcode == -1) {
  uncolor(runningcolcode);
  setattributs(A_NORMAL);
} else {
  runningcolcode = colcode;
  color = mcode2att(colcode & TYPEM);
  switch (colcode & BIMASK) {
    case BOLD:   color |= A_BOLD;      break;
    case ITALIC: color |= A_UNDERLINE; break;
  }
  setattributs(color);
  setcolor(colcode & TYPEM);
} }

int Screen::setattributs(int attrib) {
nocurses(attrib);
#ifdef USETERMIO
if (attrib & A_REVERSE) {
  if (enter_reverse_mode == NULL) /* terminal has no reverse mode */
    attrib |= A_STANDOUT;         /* use standout instead */
}
if (attrib & A_BLINK) {
  if (enter_blink_mode == NULL)   /* terminal has no blink mode */
    attrib |= A_STANDOUT;         /* use standout instead */
}
#endif
return wattrset(wndw, attrib);
}

/* curses init and various terminal setup magic */
#ifdef USETERMIO
static struct termios otermio;
#endif
int Screen::init() {
int i;
#ifdef USETERMIO
struct termios termio;
#ifdef TABDLY_NOTUSED
tcgetattr (fileno(stdin), &termio);
termio.c_oflag &= (~TABDLY)|TAB0;
tcsetattr (fileno(stdin), TCSANOW, &termio);
#endif
#endif
setenv("NCURSES_NO_HARD_TABS", "1", 1);
if ((wndw = initscr()) == NULL) return 1;
/*assert(wndw == stdscr);*/
#ifdef USETERMIO
tcgetattr (fileno(stdin), &termio); /* give me all attributes */
/*assert(O_TABDLY(termio) == TAB0);*/
otermio = termio;
termio.c_cc[VINTR] = 0; /* ctrl-c */
#ifdef VDSUSP
termio.c_cc[VDSUSP] = 0; /* ctrl-y */
#endif
termio.c_cc[VSUSP] = 0; /* ctrl-z */
#ifdef VLNEXT
termio.c_cc[VLNEXT] = 0;/* ctrl-v */
#endif
tcsetattr (fileno(stdin), TCSANOW, &termio);
#else
#ifdef WIN32
meta(stdscr, TRUE);
#endif
raw();
#endif
nonl();
noecho();
//cbreak();
keypad(stdscr,TRUE);
if (has_colors() && !monochrome) {
  start_color();
  for (i=0; i<COL_UNDEF; i++) {
/*  assert(attrels[i].ccode == i); */
    init_pair(i, attrels[i].foreg, attrels[i].backg);
  }
if (usedefault) use_default_colors();
} else monochrome = 1;
wrefresh(wndw);
getmaxyx(stdscr, ysiz, xsiz);
return 0;
}

/* wrapper for curses functions */
void Screen::createwindow(int y, int x, int py, int px) {
nocurses();
wndw = newwin(y, x, py, px);
wattrset(wndw, A_NORMAL);
wattron(wndw, COLOR_PAIR(0));
}
void Screen::deletewindow() { nocurses(); wera(); delwin(wndw); }
void Screen::wera() { nocurses(); werase(wndw); }
void Screen::wbox() {
nocurses();
if (usepoorman) wborder(wndw, '|', '|', '-', '-', '+', '+', '+', '+');
else            box(wndw, 0, 0);
}
void Screen::wmov(int y, int x) { nocurses(); wmove(wndw, y, x); }
void Screen::refr() { nocurses(); wrefresh(wndw); }
void Screen::noutrefr() { nocurses(); wnoutrefresh(wndw); }
void Screen::redraw() { nocurses(); redrawwin(wndw); }
void Screen::wsleep(int sec) { nocurses(); sleep(sec); }
int  Screen::wadds(char *str) { nocurses(OK); return waddstr(wndw, str); }
#ifndef NOUSEDITOR
int  Screen::fulledit(char *pth) { nocurses(0); return mainloop(pth, wndw); }
#endif

void Screen::closedisplay() {
nocurses();
endwin();
#ifdef USETERMIO
/*tcsetattr (fileno(stdin), TCSANOW, &otermio); curses takes care of resetting stty */
#endif
}

/* toggle overwrite/insert mode cursor shape not possible? */
void Screen::toggle() {
insertmode = !insertmode;
//if (insertmode) fputs(A_LINECURSOR,  stdout);
//else            fputs(A_BLOCKCURSOR, stdout);
//fflush(stdout);
}

/* get error message */
char *Screen::msg(int num) {
int i;
for (i=1; i<F(e)->rows; i++) if (F(e)->m(i,1) == num) break;
return F(e)->v(i,3);
}

/* get key pressed or from macro */
#define findkey(fky,len,ctl) if (i == '{' && !strncmp(macropointer, #fky "}", len)) { macropointer += len; i = KEY_CTRL(ctl); }
int Screen::wgetc() {
int i;
if (macropointer) {
  i = *macropointer++;
    if (i == '{') {
      findkey(HELP,   5,'@')
      findkey(HOME,   5,'A')
      findkey(LEFT,   5,'B')
      findkey(COPY,   5,'C')
      findkey(DELETE, 7,'D')
      findkey(END,    4,'E')
      findkey(RIGHT,  6,'F')
      findkey(PREFLD, 7,'G')
      findkey(BACKDEL,8,'H')
      findkey(NXTFLD, 7,'I')
      findkey(INS,    4,'J')
      findkey(KEYHELP,8,'K')
      findkey(REFRESH,8,'L')
      findkey(COMMIT, 7,'M')
      findkey(NXTREC, 7,'N')
      findkey(INSERT, 7,'O')
      findkey(PREREC, 7,'P')
      findkey(PRESETR,8,'R')
      findkey(COPYREC,8,'T')
      findkey(LIST,   5,'U')
      findkey(PASTE,  6,'V')
      findkey(NXTSETR,8,'W')
      findkey(QUERY,  6,'X')
      findkey(QUIT,   5,'Y')
      findkey(EXIT,   5,'Z')
    }
  if (!(*macropointer)) macropointer = NULL;
  return i;
}
if (screenclos) {
  i = getchar();
  if (i == EOF) return 'q';
} else {
  i = wgetch(stdscr); /* wgetch(wndw); getch(); */
  if (i < 0) i = 256 + i;
}
return i;
}

/* map ctrl to function keys */
int Screen::getkb() {
lastgetch = wgetc();
switch(lastgetch) {
/* KEF_HELP    */  case KEY_CTRL('@'):  return KEY_F(1);       /* Help                           Help */
/* KEF_HOME    */  case KEY_CTRL('A'):  return KEY_HOME;       /* Home / Previous block          BeginningOfLine PreviousBlock */
/* KEF_LEFT    */  case KEY_CTRL('B'):  return KEY_LEFT;       /* Previous char                  Left */
/* kcan=^C     */  case KEY_CANCEL:
/* KEF_COPY    */  case KEY_CTRL('C'):  return KEY_F(2);       /* Copy                           Copy */
/* KEF_DELETE  */  case KEY_CTRL('D'):  return KEY_F(7);       /* Delete (record)                DeleteCharacter DeleteRecord */
/* KEF_END     */  case KEY_CTRL('E'):  return KEY_END;        /* End / Next block               EndOfLine NextBlock */
/* KEF_RIGHT   */  case KEY_CTRL('F'):  return KEY_RIGHT;      /* Next char                      Right */
/* KEF_PREFLD  */  case KEY_CTRL('G'):  return KEY_BTAB;       /* Previous field                 PreviousField */
/* KEF_BACKDEL */  case KEY_CTRL('H'):  return KEY_BACKSPACE;  /* Backspace                      DeleteBackward */
/* KEF_NXTFLD  */  case KEY_CTRL('I'):  return KEY_TAB;        /* Next field                     NextField */
/* KEF_INSERT  *   case KEY_CTRL('J'):  return KEY_IC;       *//* Insert toggle (record)         InsertReplace InsertRecord */
/* KEF_KEYHELP */  case KEY_CTRL('K'):  return KEY_F(11);      /* Keyboard help                  KeyHelp */
/* KEF_REFRESH */  case KEY_CTRL('L'):  return KEY_F(12);      /* Refresh                        Refresh */
/* KEF_COMMIT  */  case KEY_CTRL('M'):  return KEY_ENTER;      /* Commit Accept                  Commit Select Execute */
/* KEF_NXTREC  */  case KEY_CTRL('N'):  return KEY_DOWN;       /* Next record                    Down NextRecord */
/* KEF_INSERT  */  case KEY_CTRL('O'):  return KEY_F(6);       /* Insert record                  InsertRecord */
/* KEF_PREREC  */  case KEY_CTRL('P'):  return KEY_UP;         /* Previoud record                PreviousRecord */
//                 case KEY_CTRL('Q'):  return KEY_F(?);       /* ?                              ? */
/* KEF_PRESETR */  case KEY_CTRL('R'):  return KEY_PPAGE;      /* Previous set of records        PreviousSetOfRecords */
//                 case KEY_CTRL('S'):  return KEY_F(?);       /* ?                              ? */
/* KEF_COPYREC */  case KEY_CTRL('T'):  return KEY_F(4);       /* Copy record                    DuplicateRecord */
/* KEF_LIST    */  case KEY_CTRL('U'):  return KEY_F(5);       /* List of values                 List */
/* KEF_PASTE   */  case KEY_CTRL('V'):  return KEY_F(3);       /* Paste / Copy field             DuplicateField Paste */
/* KEF_NXTSETR */  case KEY_CTRL('W'):  return KEY_NPAGE;      /* Next set of records            NextSetOfRecords */
/* KEF_QUERY   */  case KEY_CTRL('X'):  return KEY_F(10);      /* Query                          EnterQuery */
/* KEF_QUIT    */  case KEY_CTRL('Y'):  return KEY_F(9);       /* Rollback Cancel                ExitCancel */
/* kspd=^Z     */  case KEY_SUSPEND:
/* KEF_EXIT    */  case KEY_CTRL('Z'):  return KEY_F(8);       /* Save and exit                  Exit */
 }
return lastgetch;
}

/* edit a string on the screen */
int Screen::sedit(char *toe, int pos, ftype fty, int len) {
return sedit(toe, pos, fty, len, 0, 0);
}
int Screen::sedit(char *toe, int pos, ftype fty, int len, int lin, int col) {
char *legal;
char legalall[] = "";
char legalint[] = "0123456789";
char legalbool[] = "01";
char legalfloat[] = "0123456789.";
char legaldate[] = "0123456789./-";
switch (fty) {
 case FTY_DATE:  legal = legaldate;  break;
 case FTY_INT:   legal = legalint;   break;
 case FTY_BOOL:  legal = legalbool;  break;
 case FTY_FLOAT: legal = legalfloat; break;
 case FTY_CHAR:
 case FTY_ALL:
 default:        legal = legalall;
}
F(p)[PGE_STATUS].writef(0, 67, COL_COMMIT,13,"   Enter-Data");
if (!macropointer || watchmacro) F(p)[PGE_STATUS].refr();
return getst(lin, col, lin+col==0 ? 67 : len, EDITCOLOR, toe, pos, legal, len, NULL);
}

/* Allows the user to edit a string with only certain characters allowed
 * pos == -999 signals only output no interaction
 * ESC cancels any changes
 * Returns last key
 */
int Screen::getst(int y, int x, int width, int colcode, char *s, int pos, char *legal, int max, int *chg) {
int done;                    /* end-of-loop flag */
int changed;                 /* changed the string */
int first;                   /* first input flag/char */
int c;                       /* input key */
int sx;                      /* current position x */
int len;                     /* currrent string len  */
char *tp;                    /* position of tab */
char se[MEDSIZE];            /* my copy of string  */
char *so;                    /* position in string */
char tmp[MEDSIZE];           /* output string  */
int endx;                    /* end position   */

done = 0;
changed = 0;
first = -1;
c = 0;
if (pos == -9999) pos = -1;
else if (pos < 0 && pos > -1000) pos += strlen(s) + 1;
else if (pos < 0) { first = -1 * (pos + 1000); pos = 0; }
sx = x + pos;
len = strlen(s);
so = se;
endx = x + width - 1;

if (strlen(s) > BIGSIZE) return(KEY_ESC);
strcpy(se, s);               /* save input string */
setcode(colcode);            /* set color  */
while (!done) {              /* input loop */
  if (sx-x >= width) {       /* behind end position? */
    sx = endx;               /* go to end pos  */
    so = se + pos - width + 1;
  }
  wmov(y, x);                /* move to print string */
  snprintf(t(tmp), "%-*.*s", width, width, so);
  tmp[width] = '\0';         /* cut to width   */
  while ((tp = strchr(tmp,'\t')) != NULL) *tp = ' '; /* tab erase */
  wadds(tmp);                /* paint out string */
  if (pos==-1) break;        /* done if only paint */
  if (so > se && sx > x) mvwaddch (wndw, y, x, '<'); /* signal overfl */
  if ((int)strlen(so) > width && sx < endx) mvwaddch(wndw, y, endx, '>');
  wmov(y, sx);      /* move to cursor pos */
  if (!macropointer || watchmacro) refr();        /* show the screen */
  switch (c = (first > 0) ? first : getkb()) { /* get pressed key */
   case KEY_HOME:            /* go to start of field */
    pos = 0;
    sx  = x;
    so  = se;
    break;
   case KEY_LL:              /* go to end of field */
#if (KEY_LL != KEY_END)
   case KEY_END:             /* go to end of field */
#endif
    pos = imin (len, max-1);
    sx  = x + pos;
    break;
   case KEY_IC:              /* toggle insert mode */
   case KEY_CTRL('J'):
    toggle();
    break;
   case KEY_LEFT:            /* move left    */
    if (pos > 0) {
      pos--;
      if (sx > x) sx--;
      else    so--;
    }
    break;
   case KEY_RIGHT:           /* move right   */
    if (pos < len) {
      pos++;
      sx++;
    }
//    else done = TRUE;
    break;
   case KEY_BACKSPACE:      /* erase backward */
   case KEY_CTRL('H'):
    if (pos > 0) {
      changed = TRUE;
      memmove(&se[pos - 1], &se[pos], len - pos + 1);
      len--;
      pos--;
      if (sx > x) sx--;
      else so--;
    }
    break;
   case KEY_DC:             /* erase current  */
    if (pos < len) {
      changed = TRUE;
      memmove(&se[pos], &se[pos + 1], len - pos);
      len--;
    }
    else if (len==0) {c = KEY_CTRL('D'); done = TRUE;}
    break;
   case KEY_ESC:        /* cancel editing */
   case KEY_CTRL('C'):
   case KEY_F(9):
   case KEY_CANCEL:
    wmove(wndw, y, x);
    snprintf(t(tmp), "%-*.*s", width, width, s);
    tmp[width] = '\0';
    while ((tp = strchr(tmp,'\t')) != NULL) *tp = ' ';
    wadds(tmp);
    changed = FALSE;
    done = TRUE;
    break;
   case '\'': if (!usebindvar) break;
   default:             /* char input?    */
    if (   ((c >= ' ') && (c <= '~'))
        || ((c >= 0x80) && (c < 0xff)) ) {
      if (   ((legal[0] == 0)  /* legal input? */
          || (strchr(legal, c) != NULL)) ) {
        changed = TRUE;
        if (pos==0 && first) len = 0; /* erase on pos0 */
        if (len < max) {
          if (insertmode) {
            memmove(se+pos+1, se+pos, len - pos +1);
            *(se+pos) = ' ';
            len++;
          } else if (pos >= len) len++;
        }
        se[pos] = (char)c;
        if (len < max || pos < len-1) {
          pos++;
          sx++;
        } else {c = KEY_ENTER; done = TRUE;}
      } else beep();    /* no valid char  */
    } else done = TRUE;
  } /* switch */
  first = FALSE;
  se[len] = '\0';
  }
setcode(-1);
if (changed) strcpy(s, se);
if (chg) *chg = changed;
return(c);
}

/* prints a string at a selected location */
void Screen::writes(int y, int x, char *str) {
writef(y, x, 0, strlen(str), str);
}

/* prints a formatted string at a selected location in a color */
void Screen::writef(int y, int x, int colcode, int width, const char *format, ...) {
va_list args;
char s[MEDSIZE];
char *crp;
int oldy, oldx;
va_start (args, format);
vsnprintf (s, sizeof(s), format, args);
va_end (args);
if (width < MEDSIZE) s[width] = '\0';
if ((crp = strchr(s, '\n'))) strncpy(crp, "...", sizeof(s) - (crp - s));
if (screenclos) {
  prnf(s);
} else {
setcode(colcode);
getyx(wndw, oldy, oldx);
mvwprintw(wndw, y<0 ? ysiz+y : y, x<0 ? xsiz+x : x, "%-*s", width, s);
wmov(oldy, oldx);
setcode(-1);
} }

