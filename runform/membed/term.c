/* termio.c
 * The functions in this file
 * negotiate with the operating system
 * for characters, and write characters in
 * a barely buffered fashion on the display.
 */

#define TERMC 1
#include	"ed.h"

#include	<stdio.h>

#if BSD
#include <termios.h>
struct  termios  ostate;
struct  termios  nstate;
#else
#include <termio.h>
struct  termio  ostate;
struct  termio  nstate;
#endif

WINDOW *windw1 = NULL;
#ifdef hpux
static int hpterm = FALSE;
#endif

/*
 * This function is called once
 * to set up the terminal device streams.
 * On VMS, it translates SYS$INPUT until it
 * finds the terminal, then assigns a channel to it
 * and sets it raw. On CPM it is a no-op.
 */
int ttopen()
{
  int y, x, y1, x1;
	struct termios t;

if (windw1 == NULL) {
	windw1 = initscr();
	term.t_ncol = columns;
	term.t_nrow = lines - 1;
	tcgetattr (fileno(stdin), &t);
	t.c_cc[VINTR] = 0;
	t.c_cc[VSUSP] = 0;
#ifdef VDSUSP
  t.c_cc[VDSUSP] = 0;
#endif
#ifdef VLNEXT
  t.c_cc[VLNEXT] = 0;
#endif
	tcsetattr (fileno(stdin), TCSANOW, &t);
/*	raw();*/
	nonl();
	noecho();
	keypad (stdscr, TRUE);
} else {
if (windw1 == stdscr) {
  refresh();
  windw1 = newwin(21, 62, 2, 18);
  box(windw1, 0, 0);
  wrefresh(windw1);
}
getmaxyx(windw1, y, x);
getbegyx(windw1, y1, x1);
windw1 = newwin(y-2, x-2, y1+1, x1+1);
getmaxyx(windw1, term.t_nrow, term.t_ncol);
term.t_nrow += -1;
term.t_ncol +=  0;
wrefresh(windw1);
}
return 0;
}

/*
 * This function gets called just
 * before we go back home to the command interpreter.
 * On VMS it puts the terminal back in a reasonable state.
 * Another no-operation on CPM.
 */
int ttclose()
{
if (windw1 != stdscr) {
  delwin(windw1);
  windw1 = stdscr;
  return 0;
} else {
  endwin();
  return 1;
}
}

/*
 * Write a character to the display.
 * On VMS, terminal output is buffered, and
 * we just put the characters in the big array,
 * after cheching for overflow. On CPM terminal I/O
 * unbuffered, so we just write the byte out.
 * Ditto on MS-DOS (use the very very raw console
 * output routine).
 */
int ttputc(c)
	int c;
{
	waddch(windw1, c);
return 0;
}

void ttputs(str)			/* mb: added */
	register char *str;
{
	register int c;
	while ((c=(*str++))!='\0')
		ttputc(c);
}

/*
 * Flush terminal buffer. Does real work
 * where the terminal output is buffered up. A
 * no-operation on systems where byte at a time
 * terminal I/O is done.
 */
int ttflush()
{
	wrefresh(windw1);
return 0;
}

/*
 *  mb: find out if there is a char waiting to be read.
 */
int
ttpending()
{
	return (FALSE);
}

/*
 * Read a character from the terminal,
 * performing no editing and doing no echo at all.
 * More complex in VMS than almost anyplace else, which
 * figures. Very simple on CPM, because the system can
 * do exactly what you want.
 */
int ttgetc()
{
	int ch;
	ch = getch();
#ifdef hpux
	/* hp-emulation returns a "RETURN" after every function-key! */
	if (hpterm && ch>=KEY_F(0) && ch<=KEY_F(12))
		getch();
#endif
	return ch;
}

/*
 *  mb: output to a hardcopy device (port identified by 'hdev').
 */
int
hardputc(c)
	int c;
{
	return (FALSE);
}

/*
 * file: tcurs.c
 *
 * This file does the term stuff for systems
 * that have 'curses' flexibility.
 */

#if CURSES

#define ESC	0x1B

int tcuropen()
{
	ttopen();
	return 0;
}

int tcurmove(row, col)
register int row, col;
{
	wmove(windw1, row, col);
	return 0;
}

int tcureeol()
{
	wclrtoeol(windw1);
	return 0;
}

int tcureeop()
{
	wclear(windw1);
	return 0;
}

int tcurbeep()
{
	beep();
	return 0;
}

int tcurhglt()
{
	standout();
	return 0;
}

int tcurnrml()
{
	standend();
	return 0;
}

void putpad(str)
char	*str;
{
	tputs(str, 1, ttputc);
}

/*
extern int	tput();
extern char	*tgoto();
 */

TERM term = {
	0,		/* will be set from curses entry */
	0,
	tcuropen,
	ttclose,
	ttgetc,
	ttputc,
	ttflush,
	tcurmove,
	tcureeol,
	tcureeop,
	tcurbeep,
	tcurhglt,
	tcurnrml,
	ttpending
};
#endif
