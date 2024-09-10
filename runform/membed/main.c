/* Original code written by Dave G. Conroy,
 * substantially modified by Moshe Braner, July-December 1986.
 * Further substantial modifications by MB: January and April, 1988.
 *
 * Parts of the program Copyright (c) 1986, 1988 by Moshe Braner.
 * Program name changed from microEMACS to MEX.
 *
 * Ported to Coherent 3.1 and UN*X System V
 * Copyright (c) 1991 by Udo Munk
 *
 * Permission is granted to copy and use this program,
 * provided that this copyright notice and the source
 * code for the program are included in each copy,
 * and that it is not sold for profit.
 */

/*
 * This file contains the
 * driving routine, and some
 * keyboard processing code
 */
#include	<setjmp.h>
#include	<stdio.h>
#include	<string.h>
#include	"ed.h"
void edmore(char fname[]);

#include	"curkeys.h"
#ifndef GOOD
#define GOOD	0
#endif

#define BASE	0		/* mb: states of the machine */
#define ESC	1
#define ARG	2
#define EXEC	3
#define DASTART	990		/* starting the DA	*/
#define DACLOSE	991		/* closing the DA	*/

char	*rcsid = "$Id: main.c,v 1.41 2024/05/22 17:56:04 axel Exp $";
jmp_buf loop1;
int	logit = LOGIT;			/* mb: log keystrokes		*/
int	playback = FALSE;		/* mb: playback from log file	*/
#if ST_DA
char	*clfn[NFILES];
#else
char	**clfn;				/* mb: filenames storage	*/
#endif
int	nfiles;				/* mb: number of file args	*/
int	maxnfiles;			/* mb: max # of fn defaults	*/
int	fileindex;			/* mb: index into filename list	*/
int	currow;				/* Working cursor row		*/
int	curcol;				/* Working cursor column	*/
int	fillcol = 0;			/* Current fill column		*/
int	lmargin = 0;			/* mb: Current left margin	*/
int	tabsize = 2;			/* mb: added			*/
int	hdev;				/* mb: hard copy device	ID	*/
int	thisflag;			/* Flags, this command		*/
int	lastflag;			/* Flags, last command		*/
int	curgoal;			/* Goal column			*/
int	ovrstrk = FALSE;		/* mb: insert/overstrike flag	*/
int	deldir  = FALSE;		/* mb: <Delete> direction flag	*/
int	casesens = FALSE;		/* mb: search case sensitive	*/
BUFFER  *curbp;				/* Current buffer		*/
BUFFER  *oldbp = NULL;			/* mb: previous buffer		*/
WINDOW  *curwp;				/* Current window		*/
BUFFER  *bheadp = NULL;			/* BUFFER listhead		*/
WINDOW  *wheadp;			/* WINDOW listhead		*/
BUFFER  *blistp;			/* Buffer list BUFFER		*/
BUFFER  *bhelpp;			/* Help screens BUFFER		*/
int	kbdm[NKBDM];			/* Macro			*/
int	*kbdmip;			/* Input  for above		*/
int	*kbdmop;			/* Output for above		*/
char	pat[NPAT] = {'\0'};		/* Pattern			*/

typedef struct  {
	int	k_code;			/* Key code			*/
	int	(*k_fp)();		/* Routine to handle it		*/
}	KEYTAB;

extern  int	mlrelpy();		/* get a string from the user	*/
extern  int	quit();			/* Quit				*/
extern  int	ctlxlp();		/* Begin macro			*/
extern  int	ctlxrp();		/* End macro			*/
extern  int	ctlxe();		/* Execute macro		*/
extern  int	fileread();		/* Get a file, read only	*/
extern  int	filevisit();		/* Get a file, read write	*/
extern  int	filewrite();		/* Write a file			*/
extern  int	filesave();		/* Save current file		*/
extern  int	filename();		/* Adjust file name		*/
extern  int	getccol();		/* Get current column		*/
extern  int	gotobol();		/* Move to start of line	*/
extern  int	forwchar();		/* Move forward by characters	*/
extern  int	gotoeol();		/* Move to end of line		*/
extern  int	backchar();		/* Move backward by characters  */
extern  int	forwline();		/* Move forward by lines	*/
extern  int	backline();		/* Move backward by lines	*/
extern  int	forwpage();		/* Move forward by pages	*/
extern  int	backpage();		/* Move backward by pages	*/
extern  int	gotobob();		/* Move to start of buffer	*/
extern  int	gotoeob();		/* Move to end of buffer	*/
extern  int	setfillcol();		/* Set fill column.		*/
extern  int	setlmargin();		/* mb: added			*/
extern  int	reformat();		/* mb: added			*/
extern  int	setmark();		/* Set mark			*/
extern  int	swapmark();		/* Swap "." and mark		*/
extern  int	forwsearch();		/* Search forward		*/
extern  int	backsearch();		/* Search backwards		*/
extern  int	showcpos();		/* Show the cursor position	*/
extern  int	nextwind();		/* Move to the next window	*/
extern  int	prevwind();		/* Move to the previous window  */
extern  int	onlywind();		/* Make current window only one */
extern  int	splitwind();		/* Split current window		*/
extern  int	mvdnwind();		/* Move window down		*/
extern  int	mvupwind();		/* Move window up		*/
extern  int	enlargewind();		/* Enlarge display window.	*/
extern  int	listbuffers();		/* Display list of buffers	*/
extern  int	usebuffer();		/* Switch a window to a buffer  */
extern  int	killbuffer();		/* Make a buffer go away.	*/
extern  int	reposition();		/* Reposition window		*/
extern  int	negrepos();		/* Reposition window other way	*/
extern  int	refresh();		/* Refresh the screen		*/
extern  int	twiddle();		/* Twiddle characters		*/
extern  int	ltwiddle();		/* Twiddle lines - mb: added	*/
extern  int	tab();			/* Insert tab			*/
extern  int	tnewline();		/* Insert CR-LF			*/
extern  int	indent();		/* Insert CR-LF, then indent	*/
extern  int	openline();		/* Open up a blank line		*/
extern  int	deblank();		/* Delete blank lines		*/
extern  int	backword();		/* Backup by words		*/
extern  int	forwword();		/* Advance by words		*/
extern  int	forwdel();		/* Forward delete		*/
extern  int	backdel();		/* Backward delete		*/
extern  int	killtxt();		/* Kill forward                 */
extern  int	yank();			/* Yank back from killbuffer.	*/
extern  int	unyank();		/* mb: added			*/
extern  int	upperword();		/* Upper case word		*/
extern  int	lowerword();		/* Lower case word		*/
extern  int	upperregion();		/* Upper case region		*/
extern  int	lowerregion();		/* Lower case region		*/
extern  int	capword();		/* Initial capitalize word	*/
extern  int	delfword();		/* Delete forward word		*/
extern  int	delbword();		/* Delete backward word		*/
extern  int	killregion();		/* Kill region			*/
extern  int	copyregion();		/* Copy region to kill buffer	*/
extern  int	spawncli();		/* Run CLI in a subjob		*/
extern  int	spawn();		/* Run a command in a subjob	*/
extern  int	quickexit();		/* low keystroke style exit	*/
extern	int	hardcopy();		/* mb: added. Output to printer	*/
extern  int	wrapword();		/* mb: added the rest, 		*/
extern  int	help();			/*   just in case      		*/
extern  int	forw_brace();
extern  int	back_brace();
extern  int	instog();
extern  int	casestog();
extern  int	fortog();
extern  int	editog();
extern  int	visitog();
extern  int	gotolinum();
extern  int	forwparag();
extern  int	backparag();
extern  int	renambuf();
extern  int	page_nextw();
extern  int	back_nextw();
extern  int	flush_kbuf();
extern  int	doplay();		/* fwd ref */
extern  int	emacs_quit();		/* fwd ref */
extern  int	ctrlg();		/* fwd ref */
extern  int	undo();			/* fwd ref */
extern  int	bkill();		/* fwd ref */
extern  int	fbdel();		/* fwd ref */
extern  int	togdeldir();		/* fwd ref */
extern  int	defmacro();		/* fwd ref */

/*
 * Command table.
 * This table  is *roughly* in ASCII
 * order, left to right across the characters
 * of the command. This expains the funny
 * location of the control-X commands.
 */
KEYTAB  keytab[] = {
	CNTL|'@',		setmark,
	CNTL|'A',		gotobol,
	CNTL|'B',		backchar,
	CNTL|'C',		quit,		/* mb: was C-X C-C */
	0x163,			quit,		/* CentOS5.2 */
     ED|CNTL|'D',		forwdel,
	CNTL|'E',		gotoeol,
	CNTL|'F',		forwchar,
	CNTL|'G',		undo,
     ED|CNTL|'H',		backdel,
     ED|CNTL|0x3F,		fbdel,		/* mb: DELETE key */
     ED|CNTL|'I',		tab,
     ED|CNTL|'J',		indent,
     ED|CNTL|'K',               killtxt,
	CNTL|'L',		refresh,
     ED|CNTL|'M',		tnewline,
	CNTL|'N',		forwline,
     ED|CNTL|'O',		instog,		/* openline, */
	CNTL|'P',		backline,
	CNTL|'S',		forwsearch,
	CNTL|'R',		backpage,
     ED|CNTL|'T',		twiddle,
	CNTL|'V',		forwpage,
     ED|CNTL|'W',		killregion,
     ED|CNTL|'Y',		yank,
	CNTL|'Z',		quickexit,	/* quick save and exit  */
	0x197,			quickexit,	/* CentOS5.2 */
	CTLX|CNTL|'B',		renambuf,	/* mb: added */
	CTLX|CNTL|'C',		spawncli,	/* Run CLI in subjob.	*/
	CTLX|CNTL|'D',		fortog,		/* ar: added */
	CTLX|CNTL|'E',		editog,		/* mb: added */
	CTLX|CNTL|'F',		filename,
	CTLX|CNTL|'R',		fileread,
	CTLX|CNTL|'V',		filevisit,
     ED|CTLX|CNTL|'W',		filewrite,
	CTLX|CNTL|'X',		swapmark,
     ED|CTLX|CNTL|'Y',		unyank,		/* mb: added */
	CTLX|CNTL|'Z',		emacs_quit,
	CTLX|CNTL|0x3F,		togdeldir,	/* DELETE key dir */
	CTLX|'?',		listbuffers,
	CTLX|'!',		spawn,
	CTLX|'=',		showcpos,
	CTLX|'(',		ctlxlp,
	CTLX|')',		ctlxrp,
	CTLX|'1',		onlywind,
	CTLX|'2',		splitwind,
	CTLX|'B',		usebuffer,
	CTLX|'C',		casestog,
	CTLX|'E',		ctlxe,
     ED|CTLX|'F',		reformat,	/* mb: added */
	CTLX|'K',		killbuffer,
     ED|CTLX|'L',		setlmargin,	/* mb: added */
	CTLX|'M',		defmacro,	/* mb: added */
	CTLX|'N',		nextwind,
	CTLX|'O',		nextwind,	/* mb: EMACS-like */
	CTLX|'P',		prevwind,
	CTLX|'Q',		visitog,	/* mb: added */
     ED|CTLX|'R',		setfillcol,
     ED|CTLX|'S',		filesave,	/* mb: instead of ^X^S  */
     ED|CTLX|'T',		ltwiddle,	/* mb: added */
	CTLX|'V',		page_nextw,	/* mb: added */
	CTLX|'Z',		back_nextw,	/* mb: added */
	META|'!',		reposition,
	META|']',		forw_brace,	/* mb: added */
	META|'[',		back_brace,	/* mb: added */
	META|')',		forw_brace,	/* mb: added */
	META|'}',		forw_brace,	/* mb: added */
	META|'{',		back_brace,	/* mb: added */
	META|'(',		back_brace,	/* mb: added */
	META|'.',		setmark,
	META|' ',		setmark,
	META|'>',		gotoeob,
	META|'<',		gotobob,
	META|'B',		backword,
     ED|META|'C',		capword,
     ED|META|'D',		delfword,
	META|'F',		forwword,
	META|'G',		gotolinum,	/* mb: added */
     ED|META|'I',		instog,		/* mb: added */
     ED|META|'L',		lowerword,
	META|'R',		backsearch,
	META|'S',		forwsearch,
     ED|META|'U',		upperword,
	META|'V',		backpage,
	META|'W',		copyregion,
	META|'Y',		flush_kbuf,
     ED|META|CNTL|'H',		delbword,
     ED|META|CNTL|0x3F,		fbwdel,

#if	EXTRA
     ED|CTLX|CNTL|'L',		lowerregion,
	CTLX|CNTL|'N',		mvdnwind,
     ED|CTLX|CNTL|'O',		deblank,
	CTLX|CNTL|'P',		mvupwind,
     ED|CTLX|CNTL|'U',		upperregion,
	CTLX|'^',		enlargewind,
	META|'N',		forwparag,	/* mb: added */
	META|'P',		backparag,	/* mb: added */
#endif

#if	HELP					/* mb: added */
	META|'?',		help,
	META|CNTL|'[',		help,
#ifdef	HELPCH
	HELPCH,			help,
#endif
#endif

	0,			ctrlg
};

#define NKEYTAB (sizeof(keytab)/sizeof(keytab[0]))

/*
 * Initialize all of the buffers
 * and windows. The file name is passed down as
 * an argument, because the main routine may have been
 * told to read in a file by default, and we want the
 * buffer name to be right.
 */
void edinit(fname)
	char	fname[];
{
	BUFFER *bp;
	WINDOW *wp;
	char	bname[NBUFN];

	strncpy(bname, fname, sizeof(bname));
	bp = bfind(bname, TRUE, BFEDIT);	/* First buffer		*/
	blistp = bfind("[List]", TRUE, BFTEMP); /* Buffer list buffer	*/
	bhelpp = bfind("[Help]", TRUE, BFTEMP); /* Help screens buffer	*/
	wp = (WINDOW *) malloc(sizeof(WINDOW)); /* First window		*/
	if (bp==NULL || wp==NULL || blistp==NULL || bhelpp==NULL)
		exit(1);
	curbp  = bp;				/* Make this current	*/
	wheadp = wp;
	curwp  = wp;
	wp->w_wndp  = NULL;			/* Initialize window	*/
	wp->w_bufp  = bp;
	bp->b_nwnd  = 1;			/* Displayed.		*/
	wp->w_linep = bp->b_linep;
	wp->w_dotp  = bp->b_linep;
	wp->w_doto  = 0;
	wp->w_markp = NULL;
	wp->w_marko = 0;
	wp->w_toprow = 0;
	wp->w_ntrows = term.t_nrow-1;		/* "-1" for mode line.  */
	wp->w_force = 0;
	wp->w_offset = 0;
	wp->w_flag  = WFMODE|WFHARD;		/* Full.		*/
}

/*
 * This is the general command execution
 * routine. It handles the fake binding of all the
 * keys to "self-insert". It also clears out the "thisflag"
 * word, and arranges to move it to the "lastflag", so that
 * the next command can look at it. Return the status of
 * command.
 * mb: added the BFEDIT / ED stuff.
 */
int
execute(c, f, n)
	register int	c;
  int f, n;
{
	register KEYTAB *ktp;
	register int	k;
	register int	d;
	register int	status;

	if ((c & 0xFF00) == 0)
		goto ascii;
	d = c;
	if (curbp->b_flag & BFEDIT)
		d |= ED;
	ktp = &keytab[0];			/* Look in key table.	*/
	while (ktp < &keytab[NKEYTAB]) {
		k = ktp->k_code;
		if (k==c || k==d) {		/* mb: fit, ED| or not */
			thisflag = 0;
			status   = (*ktp->k_fp)(f, n);
			lastflag = thisflag;
			return (status);
		}
		++ktp;
	}
	if (c != d) {
		mlwrite("No such command");
		return (FALSE);
	} /* else fall thru to "view-only" message */
ascii:
	if (! (curbp->b_flag & BFEDIT)) {
		mlwrite("View-only mode - ^X^E to edit");
		lastflag = 0;			/* Fake last flags.	*/
		return(FALSE);
	}

	/* mb:
	 * If fill column is defined, the argument is positive,
	 * and we are now past fill column, perform word wrap.
	 * Don't insert a space if just wrapped.
	 */
	if (fillcol>0 && n>0 && getccol(FALSE)>=fillcol) {
		wrapword(c);
		if (c == ' ') {
			lastflag = 0;
			return (FALSE);
		}
	}
					/* mb: skipped the ASCII check	*/
	if (n <= 0) {				/* Fenceposts.		*/
		lastflag = 0;
		return (n<0 ? FALSE : TRUE);
	}
	thisflag = 0;				/* For the future.	*/
	status   = linsert(n, c, ovrstrk);	/* mb: added param	*/
	lastflag = thisflag;
	return (status);
}

void usage()
{
	puts("\nMEX version " VERSION);
	puts("\nUsage: mex [options] [file(s)]");
	puts("Options:");
	puts("\t-c #\t# columns");
	puts("\t-f #\tfill column = #");
	puts("\t-f #-#\tleft & right margins");
	puts("\t-g #\tgoto line #");
	puts("\t-r #\t# rows");
	puts("\t-t #\ttabsize = #");
	puts("\t-v\tview-only mode");
	exit(0);
}

void mainloop(char *buf, WINDOW *scr) {
	int	c=0;
	int	f;
	int	n;
	int	r;
	int	negarg;
	int	state;

  if (scr) {
    windw1 = scr;
    vtinit();
  }
  if (buf) {
    edinit("main");
    fileindex = 0;
  }

	kbdm[0] = CTLX|')';			/* Empty macro		*/
	lastflag = 0;				/* Fake last flags.	*/
	f = FALSE;
	n = 1;
	negarg = FALSE;
	update(TRUE);
	c = getkey();
	while (c==(CNTL|'Q') || c==(CNTL|'S'))
		c = getkey();			/* mb: dump handshakes	*/
	if (c != NOKEY)
		mlerase();
	state = BASE;

  r = setjmp(loop1);
	if (r == 0) for(;;) {				/* main loop */

	while (c == NOKEY) {	/* mb: happens after errors in getkey() */
		update(FALSE);
		c = getkey();
	}

	switch (state) {

	case BASE:
		if (c==(CNTL|'U')) {
			n = 4;
			mlwrite("arg: 4");
			c = getkey();
			state = ARG;
			break;
		}
		if (c == METACH) {
			c = getkey();
			state = ESC;
			break;
		}
		if (c == (CNTL|'X')) {
			c = getkey();
			if (c == (CNTL|'G')) {
				c = getkey();
				break;
			}
			if (c>='a' && c<='z')
				c -= 0x20;	/* force upper case */
			c |= CTLX;
		}
		state = EXEC;
		break;

	case ESC:
		if (c==('#')) {
			n = 4;
			mlwrite("arg: 4");
			c = getkey();
			state = ARG;
			break;
		}
		if (c=='-' || (c>='0' && c<='9')) {
			state = ARG;
			break;
		}
		if (c == (CNTL|'G')) {
			c = getkey();
			state = BASE;
			break;
		}
		if (c>='a' && c<='z')
			c -= 0x20;		/* force upper case */
		c |= META;
		state = EXEC;
		break;

	case ARG:
		if (c == (CNTL|'U')) {
			n *= 4;
		} else if (c=='-' && f==FALSE) {
			negarg = TRUE;
			n = (-1);
		} else if  (c==(CNTL|'H')  || c==(CNTL|0x3F)
							) {
			n /= 10;
			if (n == 0)
				negarg = FALSE;

		} else if (c == (CNTL|'G') || c==(CNTL|'C')
								) {
			f = FALSE;
			n = 1;
			mlwrite("[aborted]");
			update(FALSE);		/* put cursor back */
			c = getkey();
			state = BASE;
			break;
		} else if (c>='0' && c<='9') {
			if (f == FALSE) {
				n = 0;
				f = TRUE;
			}
			n = 10*n + ((negarg) ? ('0'-c) : (c-'0'));
		} else {
			mlwrite("[numeric prefix used]");
			state = BASE;
			f = TRUE;
			break;
		}
		mlwrite("arg: %d",n);
		c = getkey();
		break;

	case EXEC:
		if (c == (META|'Q')) {
			c = getkey();
			if (c & CNTL)
				c ^= 0x40;
			c &= 0xFF;
		}
		if (kbdmip != NULL) {		/* Save macro strokes.  */
			if (c!=(CTLX|')') && kbdmip>&kbdm[NKBDM-6]) {
				ctrlg();
				c = getkey();
				state = BASE;
				break;
			}
			if (f != FALSE) {
				*kbdmip++ = (CNTL|'U');
				*kbdmip++ = n;
			}
			*kbdmip++ = c;
		}
		f = execute(c, f, n);		/* Do it, finally */
		f = FALSE;
		n = 1;
		negarg = FALSE;
		update(FALSE);			/* Fix up the screen	*/
		c = getkey();
		if (c != NOKEY && mpresf != FALSE) {
			mlerase();
		}
		state = BASE;
		break;

	}					/* end of switch */

	}					/* end of for()  */

}						/* end of mainloop() */

#ifndef EMBEDDED
int main(argc, argv)
	int	argc;
	char	*argv[];
{
	int	c=0;
	int	f;
	int	n;
	int	gline = 1;
	int	visitmode = FALSE;
	char	*cp;
  BUFFER b;

if (argc==2 && !strcmp(argv[1], "-e")) {
	vtinit();
	mainloop(&b, windw1);
	vttidy();
	return 0;
} else {
	nfiles = 0;
	maxnfiles = argc + NFILES;
	if ((clfn = (char **) malloc (maxnfiles * sizeof(char *))) == NULL)
		exit (1);
	while(--argc > 0 && ++argv != NULL) {
		cp = *argv;
		if(*cp == '-') {	/* cmd line parameter */
			f = *(++cp);
			if (f >= 'a' && f <= 'z')
				f += ('A' - 'a');
			if (f && cp[1]=='\0'	/* space before number   */
			 && argc > 1 && argv[1] != NULL
			 && (c=argv[1][0])>='0' && c<='9') {
				if (f=='C' || f=='F' || f=='G'
				 || f=='R' || f=='T') {
					--argc;
					++argv;
					cp = (*argv) - 1;
				}
			}
			n = 0;
			while (f && (c=(*(++cp)))>='0' && c<='9')
				n = 10*n + (c-'0');
			if (f == 'C')
				term.t_ncol = n;
			else if (f == 'F') {
				if (c == '-') {
					lmargin = n;
					n = 0;
					while (f &&
						(c=(*(++cp)))>='0' && c<='9')
							n = 10*n + (c-'0');
				}
				fillcol = n;
				if (lmargin + tabsize > fillcol)
					lmargin = fillcol = 0;
			}
			else if (f == 'G')
				gline = n;
			else if (f == 'R')
				term.t_nrow = n-1;
			else if (f == 'T')
				tabsize = n;
			else if (f == 'V')
				visitmode = TRUE;
			else
				usage();
		} else {			/* a filename	*/
			clfn[nfiles++] = cp;
		}
	}
	vtinit();
	if (nfiles) {
		cp = clfn[0];
		edinit(cp);
		cpyfname (curbp->b_fname, cp);
		update(TRUE);
		if (readin(cp) == FIOFNF)
			mlwrite("[New file]");
		if (visitmode) {
			curbp->b_flag &= (~BFEDIT);
			logit = FALSE;
		}
		for (fileindex = 1; fileindex<nfiles; fileindex++) {
			edmore(clfn[fileindex]);
		}
	} else {				/* no filename given */
		edinit("main");
		fileindex = 0;
	}
	gotolinum(TRUE,gline);
  mainloop(NULL, NULL);
}
return 0;
}
#endif

/* mb: Display another file in another window.
 *     Called only if a second filename appears in command line.
 */
void
edmore(fname)
	char	fname[];
{
	register BUFFER *bp;
	char	bname[NBUFN];

	makename(bname, fname);
	if (bfind(bname, FALSE, 0))	/* buffer with that name exists */
		strcpy (bname, "[same as another]");
	bp = bfind(bname, TRUE, 0);	/* create second buffer 	*/
	if ( bp == NULL
	  || splitwind(0,1) != TRUE	/* display it in a 2nd window	*/
	  || nextwind(0,1)  != TRUE)
		return;
	gotobuf(bp);
	cpyfname (bp->b_fname, fname);
	if (readin(fname) == FIOEOF) {
		curwp->w_flag |= WFMODE|WFFORCE|WFHARD;
		mlwrite("[more files]");
	} else
		mlwrite("Error reading next file!");
	prevwind(0,1);
}

/*
 * Read in a key.
 * Convert ctrl keys to the internal character set.
 * mb: simplified, but added the log file stuff.
 */
int
getkey() {
	int c;

	c = (*term.t_getchar)();
#ifdef CNTLCH
	if (c == CNTLCH) {			/* Apply C- prefix	*/
		c = (*term.t_getchar)();
		if (c>='a' && c<='z')		/* Force to upper	*/
			c -= 0x20;
		c |= CNTL;
	}
#endif
	switch (c)
		{
		case KEY_BACKSPACE:		/* backspace */
					c = (CNTL | 'H');		break;
		case KEY_NEXT:			/* tabulator */
					c = (CNTL | 'I');		break;
		case KEY_UP:			/* up arrow */
					c = (CNTL | 'P');		break;
		case KEY_DOWN:			/* down arrow */
					c = (CNTL | 'N');		break;
		case KEY_RIGHT:		/* right arrow */
					c = (CNTL | 'F');		break;
		case KEY_LEFT:			/* left arrow */
					c = (CNTL | 'B');		break;
		case KEY_PPAGE:		/* PGUP key */
					c = (META | 'V');     		break;
		case KEY_NPAGE:		/* PGDN key */
					c = (CNTL | 'V');     		break;
		case KEY_LL:			/* END key */
#ifdef KEY_END
		case KEY_END:			/* END key */
#endif
					c = (CNTL | 'E');  		break;
		case KEY_HOME:			/* home key */
					c = (CNTL | 'A');		break;
		case KEY_IC:			/* insert/overstrike toggle */
					c = (META | 'I');		break;
		case KEY_DC:			/* delete key */
					c = (CNTL | 'D');		break;
		case KEY_F(1):			/* F1 key, help */
					c = (META | '?');		break;
		case KEY_F(2):			/* F2 key, new window */
					c = (CTLX | '2');		break;
		case KEY_F(3):			/* F3 key, new file */
					c = (CTLX | CNTL | 'V');	break;
		case KEY_F(4):			/* F4 one window */
					c = (CTLX | '1');		break;
		case KEY_F(5):			/* F5 search forward */
					c = (CNTL | 'S');		break;
		case KEY_F(6):			/* F6 search backward */
					c = (CNTL | 'R');		break;
		case KEY_F(7):			/* F7 search bracket */
					c = (META | '{'); 		break;
		case KEY_F(8):			/* F8 save/exit */
					c = (CNTL | 'Z');		break;
		case KEY_F(9):			/* F9 save/exit */
					c = (CNTL | 'Z');		break;
		case KEY_F(10):			/* F10 save/exit */
					c = (CNTL | 'Z');		break;
		}

	/* control -> CNTL */
	if ((!(FUNC&c)) && (c<0x20 || c==0x7F))
		c = CNTL | (c ^ 0x40);
	return (c);
}

#if CANLOG
/*
 * mb: turn on playback from inside the editor.
 */
int
doplay(f,n)
	int f, n;
{
	register int c, s;
	char buf[80];

	if (playback == TRUE)
		return (FALSE);
	s = mlreply("Playback? (y/n)", "no", buf, 80, FALSE);
	if (s==TRUE && ((c=buf[0])=='y' || c=='Y')) {
		if (logit == TRUE) {
			flushlog(TRUE);
			closelogf();
		}
		logit = FALSE;
		playback = TRUE;
		return (TRUE);
	}
	return (ctrlg());
}
#endif

/*
 * Fancy quit command, as implemented
 * by Norm. If the current buffer has changed
 * do a write current buffer and exit emacs,
 * otherwise simply exit.
 */
int
quickexit(f,n)
	int f, n;
{
	if ((curbp->b_flag&BFCHG) != 0		/* Changed.		*/
	&& (curbp->b_flag&BFTEMP) == 0)		/* Real.		*/
		if (filesave(f, n) != TRUE)
			return (FALSE);
	return (quit(f, n));			/* conditionally quit	*/
}

/*
 * Quit command. If an argument, always
 * quit. Otherwise confirm if a buffer has been
 * changed and not written out. Normally bound
 * to "C-X C-C" now "C-C".
 */
int
quit(f, n)
	int f, n;
{
	int	s;
	char	*msg = "Discard changes";
	if (anycb() == FALSE			/* All buffers clean.	*/
	|| (s=mlyesno(msg)) == TRUE) {		/* User says OK.	*/
		if (vttidy()) exit(GOOD); else longjmp(loop1, 1);
	}
  mlwrite("[aborted]");
	return (s);
}

/* mb: added */
int
emacs_quit(f, n)
	int f, n;
{
	if (f)
		return(quickexit(FALSE, 1));
	else
		return(quit(f, n));
}

/*
 * Begin a keyboard macro.
 * Error if not at the top level
 * in keyboard processing. Set up
 * variables and return.
 */
int
ctlxlp(f, n)
	int f, n;
{
	if (kbdmip!=NULL || kbdmop!=NULL) {
		mlwrite("Not now");
		return (FALSE);
	}
	mlwrite("[Start macro]");
	kbdmip = &kbdm[0];
	return (TRUE);
}

/*
 * End keyboard macro. Check for
 * the same limit conditions as the
 * above routine. Set up the variables
 * and return to the caller.
 */
int
ctlxrp(f, n)
	int f, n;
{
	if (kbdmip == NULL) {
		mlwrite("Not now");
		return (FALSE);
	}
	*(--kbdmip) = CTLX|')';		/* in case CTLX|'M' */
	mlwrite("[End macro]");
	kbdmip = NULL;
	return (TRUE);
}

/* mb: added.
 */
int
defmacro(f, n)
	int f, n;
{
	if (kbdmip == NULL)
		return (ctlxlp(f, n));
	else
		return (ctlxrp(f, n));
}

/*
 * Execute a macro.
 * The command argument is the
 * number of times to loop. Quit as
 * soon as a command gets an error.
 * Return TRUE if all ok, else
 * FALSE.
 */
int
ctlxe(f, n)
	int f, n;
{
	register int	c;
	register int	af;
	register int	an;
	register int	s;

	if (kbdmip!=NULL || kbdmop!=NULL) {
		mlwrite("Not now");
		return (FALSE);
	}
	if (n <= 0) 
		return (TRUE);
	do {
		kbdmop = &kbdm[0];
		do {
			af = FALSE;
			an = 1;
			if ((c = *kbdmop++) == (CNTL|'U')) {
				af = TRUE;
				an = *kbdmop++;
				c  = *kbdmop++;
			}
			s = TRUE;
		} while (c!=(CTLX|')') && (s=execute(c, af, an))==TRUE);
		kbdmop = NULL;
	} while (s==TRUE && --n);
	return (s);
}

/*
 * Abort.  Kill off any keyboard macro, etc., that is in progress.
 * Sometimes called as a routine, to do general aborting of stuff.
 */
int
ctrlg()
{
	mlwrite("[aborted]");		/* mb: instead of beep */
	if (kbdmip != NULL) {
		kbdm[0] = (CTLX|')');
		kbdmip  = NULL;
	}
	return (ABORT);
}

#ifdef nonono
  CNTL|'@',   setmark,         //  0
  CNTL|'A',   gotobol,         //  1 KEY_HOME:        /* HOME key */       c = (CNTL | 'A')
  CNTL|'B',   backchar,        //  2 KEY_LEFT:        /* left arrow */     c = (CNTL | 'B')
  CNTL|'C',   quit,            //  3
  CNTL|'D',   forwdel,         //  4 KEY_DC:          /* delete key */     c = (CNTL | 'D')
  CNTL|'E',   gotoeol,         //  5 KEY_END:         /* END key */        c = (CNTL | 'E')
  CNTL|'F',   forwchar,        //  6 KEY_RIGHT:       /* right arrow */    c = (CNTL | 'F')
  CNTL|'G',   undo,            //  7
  CNTL|'H',   backdel,         //  8 KEY_BACKSPACE:   /* backspace */      c = (CNTL | 'H')
  CNTL|'I',   tab,             //  9 KEY_NEXT:        /* tabulator */      c = (CNTL | 'I')
  CNTL|'J',   tnewline,        // 10
  CNTL|'K',   killtxt,         // 11
  CNTL|'L',   refresh,         // 12
  CNTL|'M',   tnewline,        // 13
  CNTL|'N',   forwline,        // 14 KEY_DOWN:        /* down arrow */     c = (CNTL | 'N')
  CNTL|'O',   openline/IC!,    // 15 KEY_IC:          /* Ins toggle */     c = (CNTL | 'O')
  CNTL|'P',   backline,        // 16 KEY_UP:          /* up arrow */       c = (CNTL | 'P')
  CNTL|'Q',   TERMINAL START   // 17
  CNTL|'R',   backpage,        // 19 KEY_PPAGE:       /* PGUP key */       c = (CNTL | 'V')
  CNTL|'S',   TERMINAL STOP    // 18
  CNTL|'T',   twiddle,         // 20
  CNTL|'U',   MULTIPLE,        // 21
  CNTL|'V',   forwpage,        // 22 KEY_NPAGE:       /* PGDN key */       c = (CNTL | 'V')
  CNTL|'W',   killregion,      // 23
  CNTL|'X',   META2,           // 24
  CNTL|'Y',   yank,            // 25
  CNTL|'Z',   quickexit,       // 26 KEY_F(8):        /* F8 save/exit */   c = (CNTL | 'Z')
  ESC         META,            // 27

KEY_IC:          /* Ins toggle */     c = (META | 'I')
KEY_F(1):        /* F1 help */        c = (META | '?')
KEY_F(2):        /* F2 new window */  c = (CTLX | '2')
KEY_F(3):        /* F3 new file */    c = (CTLX | CNTL | 'V')
KEY_F(4):        /* F4 one window */  c = (CTLX | '1')
KEY_F(5):        /* F5 search forw */ c = (META | 'S')
KEY_F(6):        /* F6 search back */ c = (META | 'R')
KEY_F(7):        /* F7 sea bracket */ c = (META | '{')
KEY_F(9):        /* F9 save/exit */   c = (CNTL | 'Z')
KEY_F(10):       /* F10 save/exit */  c = (CNTL | 'Z')
#endif
