/* cq.c
 * colquery main
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "lego.h"
#include "colquery.h"

#define MAXPATH 256

#ifdef DB
#define colquery dbquery
#endif

static char *copyright = COPYRIGHT;
static char *rcsinfo = "$Id: cq.c,v 1.56 2005/03/24 14:43:56 areinhol Exp $";

#ifdef YYDEBUG
	extern int yydebug;
#endif
char *program_name;

static int usage()
	{
	fprintf (stderr, "%s: Usage: %s [option...] column [prompt]\n",
		program_name, program_name);
	fprintf (stderr,
		" available options\n"
		"  -c         do NOT use curses for full editing\n"
		"  -i         not interactive, return immediately\n"
		"  -o         no prompting\n"
		"  -r         no added blank to prompt\n"
		"  -b         brackets around query\n"
		"  -q         quote everything\n"
		"  -a [cnd]   allow only: c=CHAR n=NUMBER d=DATE values\n"
		"  -y [^]c... valid chars ^:unvalid chars\n"
		"  -u         convert to uppercase\n"
		"  -l         convert to lowercase\n"
		"  -s         no shell quoting\n"
		"  -d value   default value for empty query\n"
		"  -D         accept default value if not null\n"
		"  -f file    messagefile for column and input value\n"
		"  -t name    resource name for value/default\n"
		"  -e         erase the message file\n"
		"  -m mess    alternate column name for message\n"
		"  -n         new line after message\n"
		"  -x         suppress message on null value\n"
#ifdef YYDEBUG
		"  -v         print debugging information\n"
#endif
		"  -p         print programmers help\n"
		"  -h         print user help screen\n"
		"  -?         this help message and version info\n"
		);
	exit (1);
	}

static int progusage()
	{
	printf ("Usage in /bin/sh-scripts:\n"
		"\n"
		"script.sh:\n"
		"\tWHERE=`colquery -q col7 \"Spalte 7\"`\n"
		"\tsqlplus uid/pw @query \"$WHERE\"\n"
		"\n"
		"query.sql:\n"
		"\tselect col2, col4 from tab6\n"
		"\twhere &1\n"
		"\t/\n"
		"\texit\n\n"
		);
	exit (1);
	}

static int type = 0;		/* type mode         -a option		*/
static int nonum = 0;		/* no numbers mode   -q option		*/

static int userusage(out)
	FILE *out;
{
fprintf (out,
"------------------- Beispiele fuer Benutzung -------------------\n"
"================================================================\n"
"Eingabe             Auswahl\n"
"-------             --------------------------------------------\n");

if (type==0 || type==NUMBER || type==STRING && nonum) fprintf (out,
"10                  genau ein bestimmter Wert\n");

if (type==0 || type==STRING) fprintf (out,
"hallo               Bezeichnung, Name\n"
"\"abc DEF\"           Wert mit Leerzeichen\n"
"1_                  Ersatzzeichen _=ein Zeichen %%=viele Zeichen\n"
"*hallo              suche nach Zeichenkette\n"
"~meier              aehnlich\n");

if (type==0 || type==DATE) fprintf (out,
"10.03.94            Datum\n"
"12.94               Monat\n");

fprintf (out,
"10,12,15            Liste verschiedener Werte\n"
"10-14               von-bis Bereich\n"
">12                 Vergleich, moeglich sind: = != <> > < >= <=\n"
"%%                   alle Werte\n"
"#                   NULL-Wert\n"
"!10                 ungleich eines Wertes oder Bereichs\n"
"10-12 17,19,23 37   mehere verschiedene Abfragen ODER-Bedingung\n"
"10-17&!12           UND-Bedingung: 10 bis 17 ohne 12\n"
"(10-17&!12)37       Klammern fuer komplexe Abfragen\n"

"[land='DE']         beliebige SQL-Abfrage\n"
"================================================================\n");
}

int main (argc, argv)
	int argc;
	char *argv[];
{
/* option handling getopt()-standard
 */
extern char *optarg;
extern int optind, optopt;
char *opts =
#ifdef YYDEBUG
	"cd:Dibqy:a:solut:f:em:nxrvph?";
#else
	"cd:Dibqy:a:solut:f:em:nxrph?";
#endif
char *defvalue = NULL;		/* default value for -d option		*/
char *valid = NULL;		/* valid chars for   -y option		*/
int interactive = 1;		/* interactive mode  -v option		*/
int curses = 1;			/* curses mode       -c option		*/
int brackets = 0;		/* brackets mode     -b option		*/
int nonum = 0;			/* no numbers mode   -q option		*/
int shell = 1;			/* shell quoting     -s option		*/
int prompt = 1;			/* prompting         -o option		*/
int blank = 1;			/* prompting blank   -r option		*/
char upper = 0;			/* uppercase         -u option		*/
char lower = 0;			/* lowercase         -l option		*/
char *rcname = NULL;		/* name for resource -t option		*/
FILE *resource = NULL;		/* file for message  -t option		*/
FILE *resource2 = NULL;		/* file for message  -t option		*/
char rcn[81];			/* resource name     -t option		*/
char rcv[2048];			/* resource value    -t option		*/
char *mesname = NULL;		/* name for message  -f option		*/
FILE *message = NULL;		/* file for message  -f option		*/
int erase = 0;			/* erase msg. file   -e option          */
int suppress = 0;		/* suppress message  -x option          */
char *messcol = NULL;		/* message header    -m option		*/
int newline = 0;		/* newline for msg.  -n option		*/
int acceptdef = 0;		/* accept default    -D option		*/
char *colname;			/* column name				*/
char promptstr[81];		/* prompt string			*/
char *home;			/* home path				*/
char rcpath[MAXPATH];		/* resource path			*/
char rcpath2[MAXPATH];		/* resource path			*/

int c;				/* option char				*/
int i;
int rc = 0;			/* parser return code			*/
char query[2048];		/* query output buffer			*/
char where[1024] = "";		/* input buffer				*/

/* get a clean program name from $0
 */
program_name = strrchr(argv[0],'/');
if (program_name==NULL) program_name=argv[0]; else program_name++;

while ((c = getopt(argc, argv, opts)) != -1)
	switch (c)
	 {
	 case 'd':	defvalue	= strdup(optarg);	break;
	 case 'D':	acceptdef	= 1;			break;
	 case 'i':	interactive	= 0;			/* FALLTHROUGH */
	 case 'o':	prompt		= 0;			break;
	 case 'r':	blank		= 1;			break;
	 case 'u':	upper		= 1;			break;
	 case 'l':	lower		= 1;			break;
	 case 'c':	curses		= 0;			break;
	 case 'b':	brackets	= 1;			break;
	 case 'q':	nonum		= 1;			break;
	 case 'y':	valid		= strdup(optarg);	break;
	 case 'a':	switch(tolower(*optarg))
			 {
			 case 'c': type	= STRING;	break;
			 case 'n': type	= NUMBER;	break;
			 case 'd': type	= DATE;		break;
			 default:  fprintf(stderr,
					"Falscher Datentyp \"%c\"\n", *optarg);
				   usage();
			 }					break;
	 case 'f':	mesname		= strdup(optarg);	break;
	 case 't':	rcname		= strdup(optarg);	break;
	 case 'e':	erase		= 1;			break;
	 case 'm':	messcol		= strdup(optarg);	break;
	 case 'n':	newline		= 1;			break;
	 case 'x':	suppress	= 1;			break;
	 case 's':	shell		= 0;			break;
#ifdef YYDEBUG
	 case 'v':	yydebug		= 1;			break;
#endif
	 case 'h':	userusage(stdout);			exit(0);
	 case 'p':	progusage();
	 case '?':	fprintf (stderr, "%s\n%s\n", copyright, rcsinfo);
	 default:	usage();
	 }

if (rcname) {
	if ((home = getenv("HOME")) == NULL)
		fatal ("$HOME not set", rcname);
	strcpy(rcpath, home);
	strcat(rcpath, "/.cq");
	if ((resource = fopen (rcpath, "r")) != NULL) {
		while (fscanf(resource, "%s = %[^\n]\n", rcn, rcv) == 2) {
			if (!strcmp(rcname, rcn)) {
				defvalue = strdup(rcv);
				break;			
				}
			}
		fclose(resource);
		}
	}

if (mesname && (message = fopen (mesname, erase?"w":"a")) == NULL)
	fatal ("could not open \"%s\"", mesname);
if (optind==argc-1)
	strcpy (promptstr, argv[optind]);
else if (optind==argc-2)
	strcpy (promptstr, argv[optind+1]);
else	usage();
if (blank) strcat (promptstr, " ");
colname = argv[optind];

while (rc==0)
	{
	if (*where=='\0' && defvalue!=NULL) strcpy (where, defvalue);
	if (acceptdef && where[0]) {
	  interactive = 0;
	  }
	else {
	  if (curses)
	    {
	    if (getinput(prompt?promptstr:NULL,where,0,valid)=='\033') break;
	    }
	  else
	    {
	    if (prompt)
		{
		fprintf (stderr, "%s", promptstr);
		if (defvalue!=NULL)
			{
			fprintf (stderr, "%s", defvalue);
			/* move the cursor back to start of the default value
			 */
			for (i=0; i<strlen(defvalue); i++)
				fputc ('\b', stderr);
			}
		}
	    fgets (where, sizeof(where), stdin);
	    if (where[strlen(where)-1]=='\n') where[strlen(where)-1]='\0';
	    }
	  }
#ifdef YYDEBUG
	fprintf (stderr, "w=\"%s\"\n", where);
#endif
	if (upper) strupr(where);
	if (lower) strlow(where);
	if (strcmp(where,"?"))
		rc = colquery (where, query, colname, nonum, type);
#ifdef YYDEBUG
	fprintf (stderr, "q=\"%s\" rc=%d\n", query, rc);
#endif
	if (rc==0 || (type!=0 && rc!=type && rc!=SQLNULL))
		{
		if (interactive)
			{
			userusage(stderr);
			rc = 0;
			}
		else 
			{
			sprintf (query, "\nUngueltige Eingabe: %s\n", where);
			break;
			}
		}
	}
if (rcname) {
	strcpy(rcpath2, rcpath);
	strcat(rcpath2, "~");
	if ((resource2 = fopen (rcpath2, "w")) == NULL)
		fatal ("could not open \"%s\"", rcpath2);
	if ((resource = fopen (rcpath, "r")) != NULL) {
		while (fscanf(resource, "%s = %[^\n]\n", rcn, rcv) == 2) {
			if (strcmp(rcname, rcn)) fprintf(resource2, "%s = %s\n", rcn, rcv);
			}
		}
	fprintf(resource2, "%s = %s\n", rcname, where);
	fclose(resource);
	fclose(resource2);
	unlink(rcpath);
	link(rcpath2, rcpath);
	unlink(rcpath2);
	}
if (message && (!suppress || *where))
	{
	fprintf (message, "%s %s%c",
			messcol?messcol:promptstr, where, newline?'\n':' ');
	}
if (shell)
	{
	char *pquery = query;
	char *apo;
	while ((apo = strchr (pquery, '\'')) != NULL)
		{
		strichr (apo, '\'');
		pquery = apo+2;
		}
	strichr (query, '\'');
	strichr (query+strlen(query), '\'');
	}
printf (brackets ? "(%s)\n" : "%s\n", query);
exit(!rc);
}
