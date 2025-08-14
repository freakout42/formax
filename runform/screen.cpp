#define _XOPEN_SOURCE_EXTENDED 1
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

#ifdef WIN32
HANDLE stdinHandle;
#endif
char cursesversion[8] = NCURSES_VERSION;
char cursesrun[32];

#ifdef UTF8
#define CURVARIANT 'w'

static int cur_scpy(wchar_t *t, wchar_t *s, int z) {
int i, j;
j = 0;
for (i=0; i<z; i++) {
  switch (*s) {
    case '\t': j++; s++;
    case '\0': *t++ = ' '; break;
    default:   j++; *t++ = *s++;
    }
  }
  *t = '\0';
return j;
}
static int cur_wcpy(wchar_t *t, char *s) {
int i, j, z;
z = strlen(s);
j = 0;
for (i=0; i<z; i++) {
  switch (*s) {
    case '\t': j++; s++;
    case '\0': *t++ = ' '; break;
    default:   j++; *t++ = (unsigned char)*s++;
    }
  }
  *t = '\0';
return j;
}

static char *cur_8cpy(wchar_t *s) {
static char t[BIGSIZE+1];
char *y;
for (y=t; *s; s++) *y++ = (unsigned char)*s;
*y = '\0';
return t;
}

static int cur_slen(wchar_t *s) { return wcslen(s); }

/* Convert an UTF-8 string to UCODE
 * All invalid sequences are ignored.
 * Note: output == input is allowed,
 * but   input < output < input + length is not.
 * Output has to have room for (length+1) chars, including the trailing NUL byte.
 */
static int utf8_to_ucode(wchar_t *output, char *input, int length) {
    wchar_t                   *out = output;
    const unsigned char       *in  = (const unsigned char *)input;
    const unsigned char *const end = (const unsigned char *)input + strlen(input);
    unsigned int               c;

    while (in < end)
        if (*in < 128)
            *(out++) = *(in++); /* Valid codepoint */
        else
        if (*in < 192)
            break;
/*          in++;                * 10000000 .. 10111111 are invalid */
        else
        if (*in < 224) {        /* 110xxxxx 10xxxxxx */
            if (in + 1 >= end)
                break;
            if ((in[1] & 192U) == 128U) {
                c =   (((unsigned int)(in[0] & 0x1FU)) << 6U)
                    | ((unsigned int)(in[1] & 0x3FU)) ;
                *(out++) = c;
            }
            in += 2;

        } else
        if (*in < 240) {        /* 1110xxxx 10xxxxxx 10xxxxxx */
            if (in + 2 >= end)
                break;
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U) {
                c =   (((unsigned int)(in[0] & 0x0FU)) << 12U)
                    | (((unsigned int)(in[1] & 0x3FU)) << 6U)
                    |  ((unsigned int)(in[2] & 0x3FU)) ;
                *(out++) = c;
            }
            in += 3;

        } else
        if (*in < 248) {        /* 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
            if (in + 3 >= end)
                break;
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U &&
                (in[3] & 192U) == 128U) {
                c =   (((unsigned int)(in[0] & 0x07U)) << 18U)
                    | (((unsigned int)(in[1] & 0x3FU)) << 12U)
                    | (((unsigned int)(in[2] & 0x3FU)) << 6U)
                    |  ((unsigned int)(in[3] & 0x3FU)) ;
                *(out++) = c;
            }
            in += 4;

        } else
        if (*in < 252) {        /* 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
            if (in + 4 >= end)
                break;
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U &&
                (in[3] & 192U) == 128U &&
                (in[4] & 192U) == 128U) {
                c =   (((unsigned int)(in[0] & 0x03U)) << 24U)
                    | (((unsigned int)(in[1] & 0x3FU)) << 18U)
                    | (((unsigned int)(in[2] & 0x3FU)) << 12U)
                    | (((unsigned int)(in[3] & 0x3FU)) << 6U)
                    |  ((unsigned int)(in[4] & 0x3FU)) ;
                *(out++) = c;
            }
            in += 5;

        } else
        if (*in < 254) {        /* 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
            if (in + 5 >= end)
                break;
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U &&
                (in[3] & 192U) == 128U &&
                (in[4] & 192U) == 128U &&
                (in[5] & 192U) == 128U) {
                c =   (((unsigned int)(in[0] & 0x01U)) << 30U)
                    | (((unsigned int)(in[1] & 0x3FU)) << 24U)
                    | (((unsigned int)(in[2] & 0x3FU)) << 18U)
                    | (((unsigned int)(in[3] & 0x3FU)) << 12U)
                    | (((unsigned int)(in[4] & 0x3FU)) << 6U)
                    |  ((unsigned int)(in[5] & 0x3FU)) ;
                *(out++) = c;
            }
            in += 6;

        } else
            in++;               /* 11111110 and 11111111 are invalid */

    /* Terminate the output string. */
    *out = 0;

    if (in < end) return -1;
    else          return out - output;
}

static int utf162utf8(char* out, int c) {
int bits;
char *out1;
out1 = out;
if      (c <    0x80) {  *out++=  c;                         bits = -6; }
else if (c <   0x800) {  *out++= ((c >>  6) & 0x1F) | 0xC0;  bits =  0; }
else if (c < 0x10000) {  *out++= ((c >> 12) & 0x0F) | 0xE0;  bits =  6; }
else                  {  *out++= ((c >> 18) & 0x07) | 0xF0;  bits = 12; }
for ( ; bits >= 0; bits -= 6) *out++= ((c >> bits) & 0x3F) | 0x80;
return out - out1;
}

static int str_w2u8(char *tgt, wchar_t *src, int siz) {
char *t;
wchar_t *s;
t = tgt;
for (s = src; *s; s++) t += utf162utf8(t, *s);
*t = '\0';
return t - tgt;
}

/* s iso or utf-8 (cur_utf8=TRUE) char[]
 * f from pos
 * l min size padded <0 right align
 * z max length
 */
char *Screen::str_sub(char *tg, char *s, int f, int l, int z) {
char sv = '\0';
char *p;
char *q;
int n, m, v, o, r;
o = abs(l);
n = 0;
r = strlen(s);
q = s + r;
for (p = s; *p; p++) {
 if (!cur_utf8 || (*p & 0xC0) != 0x80) {
  if (n == f) q = p;
  if (z > 0 && n >= f+z) break;
  n++;
 }
}
n -= f;
if (z > 0 && *p) {
  sv = *p;
  *p = '\0';
}
m = strlen(q);
v = str_pos(q, m);
if (tg==NULL && ((tg = (char*)malloc(m + 1 + o + r)) == NULL)) return NULL;
if (o-n > 0) {
 if (l < 0) {
  memset(tg, ' ', o-n);
  strcpy(tg+o-n, q);
 } else {
  strcpy(tg, q);
  memset(tg+m, ' ', o-v);
  tg[m+o-v] = '\0';
 } } else {
  strcpy(tg, q);
}
if (z > 0 && sv) *p = sv;
return tg;
}

#else
#define CURVARIANT 'n'

static int cur_scpy(char *t, char *s, int z) {
int i, j;
j = 0;
for (i=0; i<z; i++) {
  switch (*s) {
    case '\t': j++; s++;
    case '\0': *t++ = ' '; break;
    default:   j++; *t++ = *s++;
    }
  }
  *t = '\0';
return j;
}

static int cur_slen(char *s) { return strlen(s); }

#endif

int Screen::str_pos(char *s, int f) {
char *p;
int n, m;
n = 0;
m = 0;
for (p = s; *p; p++) {
  if (m++ >= f) break;
  if (!cur_utf8 || (*p & 0xC0) != 0x80) n++;
  }
return n;
}

static int str_tlen (char *s)
{
char *p;

if (NULL == (p = strchr(s, '\0')))
	return (0);
for (; isspace(*s); s++);
for (--p; (p > s) && isascii(*p) && isspace(*p);)
	p--;
return (p-s+1);
}

void Screen::cur_puts(int y, int x, char *s, int w) {
#ifdef UTF8
char *out = NULL;
wchar_t t[BIGSIZE+1];

if (cur_utf8) {
  out = str_sub(out, s, 0, w, 0);
  utf8_to_ucode(t, out, BIGSIZE);
  wmov (y, x);
  waddwstr(wndw, t);
  free(out);
} else
#endif
mvwprintw (wndw, y, x, "%-*s", w, s);
}

Screen::Screen() {
ysiz = 0;
cursesvariant = CURVARIANT;
sprintf(cursesrun, "%s-%c", curses_version(), CURVARIANT);
#ifdef UTF8
strcat(cursesrun, "=");
strcat(cursesrun, cur_utf8 ? "1" : "0");
#endif
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
#ifndef WIN32
setenv("NCURSES_NO_HARD_TABS", "1", 1);
setenv("NCURSES_NO_UTF8_ACS",  "1", 1);
#endif
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

int  Screen::wadds(char *str) {
nocurses(OK);
return waddstr(wndw, str);
}

#ifdef UTF8
int  Screen::wadds(wchar_t *str) {
int status;
nocurses(OK);
if (cur_utf8) status = waddwstr(wndw, str);
else status = waddstr(wndw, cur_8cpy(str));
return status;
}
#endif

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
#define findkey(fky,len,ctl) if (ch == '{' && !strncmp(macropointer, #fky "}", len)) { macropointer += len; ch = KEY_CTRL(ctl); }
int Screen::wgetc() {
int ch;
int keycode = 1;
#ifdef UTF8
wint_t keypress = { 0 };
#endif
if (macropointer) {
  ch = *macropointer++;
    if (ch == '{') {
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
  return ch;
}
if (screenclos) {
  ch = getchar();
  if (ch == EOF) return 'q';
} else {
/* ncurses legacy
 *  i = wgetch(stdscr); // wgetch(wndw); getch(); /
 *  if (i < 0) i = 256 + i;
 */
#ifdef UTF8
if (cur_utf8) {
#ifdef WIN32
  DWORD n = 0;
  int uc, sc, ck;
  ch = 0;
  INPUT_RECORD ir[1];
  while ( ch == 0) {
    ReadConsoleInputW(stdinHandle, ir, 1, &n);
    if (ir[0].EventType & KEY_EVENT) {
     if (ir[0].Event.KeyEvent.bKeyDown) {
      sc = ir[0].Event.KeyEvent.wVirtualScanCode;
      ck = ir[0].Event.KeyEvent.dwControlKeyState;
      uc = ir[0].Event.KeyEvent.uChar.UnicodeChar;
      if (uc == 0) {
       if (!(ck & (SHIFT_PRESSED | LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED | LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED )))
        if (sc != ':') {
          keycode = -1;
          switch (sc) {
            case 0x3B: ch = KEY_F(1); break;
            case 0x3C: ch = KEY_F(2); break;
            case 0x3D: ch = KEY_F(3); break;
            case 0x3E: ch = KEY_F(4); break;
            case 0x3F: ch = KEY_F(5); break;
            case 0x40: ch = KEY_F(6); break;
            case 0x41: ch = KEY_F(7); break;
            case 0x42: ch = KEY_F(8); break;
            case 0x43: ch = KEY_F(9); break;
            case 0x44: ch = KEY_F(10); break;
            case 0x45: ch = KEY_F(11); break;
            case 0x46: ch = KEY_F(12); break;
            case 0x47: ch = KEY_HOME; break;
            case 0x48: ch = KEY_UP; break;
            case 0x49: ch = KEY_PPAGE; break;
            case 0x4B: ch = KEY_LEFT; break;
            case 0x4D: ch = KEY_RIGHT; break;
            case 0x4F: ch = KEY_END; break;
            case 0x50: ch = KEY_DOWN; break;
            case 0x51: ch = KEY_NPAGE; break;
            case 0x52: ch = KEY_IC; break;
            case 0x53: ch = KEY_DC; break;
          }
        }
      } else {
       if (uc == '\t' && (ck & SHIFT_PRESSED)) {
        keycode = -1;
        ch = KEY_BTAB;
       } else {
        keycode = 1;
        ch = uc;
       }
      }
     }
    }
  }
#else
keycode = (get_wch(&keypress) == KEY_CODE_YES) ? -1 : 1;
ch = keypress;
#endif /* WIN32 */
} else
#endif /* UTF8 */
{
ch = wgetch (stdscr);
keycode = ch > 255 ? -1 : 1;
}
#ifndef UTF8
#ifdef WIN32
if (ch < 0) ch = 256 + ch;
#endif
#endif
}
return ch * keycode;
}

/* map ctrl to function keys */
int Screen::getkb() {
int ch;
ch = wgetc();
if (ch < ' ') switch(abs(ch)) {
/* KEF_HELP    */  case KEY_CTRL('@'):  return -KEY_F(1);       /* Help                           Help */
/* KEF_HOME    */  case KEY_CTRL('A'):  return -KEY_HOME;       /* Home / Previous block          BeginningOfLine PreviousBlock */
/* KEF_LEFT    */  case KEY_CTRL('B'):  return -KEY_LEFT;       /* Previous char                  Left */
/* kcan=^C     */  case -KEY_CANCEL:
/* KEF_COPY    */  case KEY_CTRL('C'):  return -KEY_F(2);       /* Copy                           Copy */
/* KEF_DELETE  */  case KEY_CTRL('D'):  return -KEY_F(7);       /* Delete (record)                DeleteCharacter DeleteRecord */
/* KEF_END     */  case KEY_CTRL('E'):  return -KEY_END;        /* End / Next block               EndOfLine NextBlock */
/* KEF_RIGHT   */  case KEY_CTRL('F'):  return -KEY_RIGHT;      /* Next char                      Right */
/* KEF_PREFLD  */  case KEY_CTRL('G'):  return -KEY_BTAB;       /* Previous field                 PreviousField */
/* KEF_BACKDEL */  case KEY_CTRL('H'):  return -KEY_BACKSPACE;  /* Backspace                      DeleteBackward */
/* KEF_NXTFLD  */  case KEY_CTRL('I'):  return  KEY_TAB;        /* Next field                     NextField */
/* KEF_INSERT  *   case KEY_CTRL('J'):  return -KEY_IC;       *//* Insert toggle (record)         InsertReplace InsertRecord */
/* KEF_KEYHELP */  case KEY_CTRL('K'):  return -KEY_F(11);      /* Keyboard help                  KeyHelp */
/* KEF_REFRESH */  case KEY_CTRL('L'):  return -KEY_F(12);      /* Refresh                        Refresh */
/* KEF_COMMIT  */  case KEY_CTRL('M'):  return -KEY_ENTER;      /* Commit Accept                  Commit Select Execute */
/* KEF_NXTREC  */  case KEY_CTRL('N'):  return -KEY_DOWN;       /* Next record                    Down NextRecord */
/* KEF_INSERT  */  case KEY_CTRL('O'):  return -KEY_F(6);       /* Insert record                  InsertRecord */
/* KEF_PREREC  */  case KEY_CTRL('P'):  return -KEY_UP;         /* Previoud record                PreviousRecord */
//                 case KEY_CTRL('Q'):  return -KEY_F(?);       /* ?                              ? */
/* KEF_PRESETR */  case KEY_CTRL('R'):  return -KEY_PPAGE;      /* Previous set of records        PreviousSetOfRecords */
//                 case KEY_CTRL('S'):  return -KEY_F(?);       /* ?                              ? */
/* KEF_COPYREC */  case KEY_CTRL('T'):  return -KEY_F(4);       /* Copy record                    DuplicateRecord */
/* KEF_LIST    */  case KEY_CTRL('U'):  return -KEY_F(5);       /* List of values                 List */
/* KEF_PASTE   */  case KEY_CTRL('V'):  return -KEY_F(3);       /* Paste / Copy field             DuplicateField Paste */
/* KEF_NXTSETR */  case KEY_CTRL('W'):  return -KEY_NPAGE;      /* Next set of records            NextSetOfRecords */
/* KEF_QUERY   */  case KEY_CTRL('X'):  return -KEY_F(10);      /* Query                          EnterQuery */
/* KEF_QUIT    */  case KEY_CTRL('Y'):  return -KEY_F(9);       /* Rollback Cancel                ExitCancel */
/* kspd=^Z     */  case -KEY_SUSPEND:
/* KEF_EXIT    */  case KEY_CTRL('Z'):  return -KEY_F(8);       /* Save and exit                  Exit */
 }
return ch;
}

/* get the fully translated key */
int Screen::getkey() {
lastgetch = getkb();
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
int endx;                    /* end position   */

#ifdef UTF8
wchar_t se[MEDSIZE+1];
wchar_t *so = se;            /* position in string */
wchar_t tmp[MEDSIZE*2+1];    /* output string */
#else
char se[MEDSIZE];            /* my copy of string  */
char *so;                    /* position in string */
char tmp[MEDSIZE];           /* output string  */
#endif

if (strlen(s) > BIGSIZE) return(KEF_CANCEL);
done = 0;
changed = 0;
first = -1;
c = 0;
if (pos == -9999) pos = -1;
else if (pos < 0 && pos > -1000) pos += str_tlen(s) + 1;
else if (pos < 0) { first = -1 * (pos + 1000); pos = 0; }
pos = str_pos(s, pos);
sx = x + pos;
#ifdef UTF8
len = cur_utf8 ? utf8_to_ucode(se, s, BIGSIZE) : cur_wcpy(se, s);
if (len == -1) return(KEF_CANCEL);
#else
len = strlen(s);
strcpy(se, s);               /* save input string */
#endif
so = se;
endx = x + width - 1;
setcode(colcode);            /* set color  */
while (!done) {              /* input loop */
  if (sx-x >= width) {       /* behind end position? */
    sx = endx;               /* go to end pos  */
    so = se + pos - width + 1;
  }
  wmov(y, x);                /* move to print string */
  cur_scpy(tmp, so, width);  /* trim string */
  wadds(tmp);                /* paint out string */
  if (pos==-1) break;        /* done if only paint */
  if (so > se && sx > x) mvwaddch (wndw, y, x, '<'); /* signal overfl */
  if (cur_slen(so) > width && sx < endx) mvwaddch(wndw, y, endx, '>');
  wmov(y, sx);      /* move to cursor pos */
  if (!macropointer || watchmacro) refr();        /* show the screen */
  switch (c = (first > 0) ? first : getkey()) { /* get pressed key */
   case -KEY_HOME:            /* go to start of field */
    pos = 0;
    sx  = x;
    so  = se;
    break;
   case -KEY_LL:              /* go to end of field */
#if (-KEY_LL != -KEY_END)
   case -KEY_END:             /* go to end of field */
#endif
    pos = imin (len, max-1);
    sx  = x + pos;
    break;
   case -KEY_IC:              /* toggle insert mode */
   case -KEY_CTRL('J'):
    toggle();
    break;
   case -KEY_LEFT:            /* move left    */
    if (pos > 0) {
      pos--;
      if (sx > x) sx--;
      else    so--;
    }
    break;
   case -KEY_RIGHT:           /* move right   */
    if (pos < len) {
      pos++;
      sx++;
    }
//    else done = TRUE;
    break;
   case -KEY_BACKSPACE:      /* erase backward */
   case KEY_CTRL('H'):
    if (pos > 0) {
      changed = TRUE;
      memmove(&se[pos - 1], &se[pos], (len - pos + 1) * sizeof(wchar_t));
      len--;
      pos--;
      if (sx > x) sx--;
      else so--;
    }
    break;
   case -KEY_DC:             /* erase current  */
    if (pos < len) {
      changed = TRUE;
      memmove(&se[pos], &se[pos + 1], len - pos);
      len--;
    }
    else if (len==0) {c = KEY_CTRL('D'); done = TRUE;}
    break;
   case KEY_ESC:        /* cancel editing */
   case KEY_CTRL('C'):
   case -KEY_F(9):
   case -KEY_CANCEL:
    wmove(wndw, y, x);
#ifdef UTF8
    utf8_to_ucode(se, s, BIGSIZE);
#endif
    cur_scpy(tmp, se, width);
    wadds(tmp);
    changed = FALSE;
    done = TRUE;
    break;
   case '\'': if (!usebindvar) break;
   default:             /* char input?    */
		if ( ((c >= ' ') && (c <= '~')) || (c == '\t') || (c >= 0x80) ) {
		    if (    ((legal[0] == 0)	/* legal input?		*/
			 || (strchr(legal, c) != NULL)) )
			{
        changed = TRUE;
        if (pos==0 && first) len = 0; /* erase on pos0 */
        if (len < max) {
          if (insertmode) {
            memmove(se+pos+1, se+pos, (len - pos + 1) * sizeof(wchar_t));
            *(se+pos) = ' ';
            len++;
          } else if (pos >= len) len++;
        }
        se[pos] = c;
        if (len < max || pos < len-1) {
          pos++;
          sx++;
        } else {c = KEF_COMMIT; done = TRUE;}
      } else beep();    /* no valid char  */
    } else done = TRUE;
  } /* switch */
  first = FALSE;
  se[len] = '\0';
  }
setcode(-1);
if (changed) {
#ifdef UTF8
if (cur_utf8) str_w2u8(s, se, BIGSIZE*2);
else          strcpy(s, cur_8cpy(se));
#else
strcpy(s, se);
#endif
}
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
  if ((y + x) > 0) prnf(s);
} else {
setcode(colcode);
getyx(wndw, oldy, oldx);
//mvwprintw(wndw, y<0 ? ysiz+y : y, x<0 ? xsiz+x : x, "%-*s", width, s);
cur_puts(y<0 ? ysiz+y : y, x<0 ? xsiz+x : x, s, width);
wmov(oldy, oldx);
setcode(-1);
} }

#ifdef UTF8
void Screen::writew(int y, int x, int colcode, int width, int align, char *sval) {
char s[MEDSIZE];
char *crp;
int oldy, oldx;
str_sub(s, sval, 0, align, width);
if ((crp = strchr(s, '\n'))) strncpy(crp, "...", sizeof(s) - (crp - s));
if (screenclos) {
  if ((y + x) > 0) prnf(s);
} else {
setcode(colcode);
getyx(wndw, oldy, oldx);
cur_puts(y<0 ? ysiz+y : y, x<0 ? xsiz+x : x, s, width);
wmov(oldy, oldx);
setcode(-1);
} }
#endif
