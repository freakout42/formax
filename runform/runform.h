/* runform.h - constants macros and central procedures */
#define VERSION "2.2.6"
#define VERMSGS 23
extern char odbcversion[];
extern char odbcrun[];
extern char cursesversion[];
extern const char *cursesrun;
extern int intrigger;
extern char *macropointer;
extern char sqliteversion[];
extern const char *sqliterun;
extern int sqlitevernumber;
extern char about[];
#define gnucs1(m, i, l) #m "." #i "." #l
#define gnucs(m, i, l) gnucs1(m, i, l)
#if defined(__clang__)
#define gnucver "Clang-" gnucs(__clang_major__, __clang_minor__, __clang_patchlevel__)
#elif defined(__GNUC__)
#define gnucver "GCC-" gnucs(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
#define gnucver "cc"
#endif
#define CCOMPILER gnucver
#if __hpux
#define PLATFORM        "HP-UX"
#elif __FreeBSD__
#define PLATFORM        "FreeBSD"
#elif __APPLE_CC__
#define PLATFORM        "MacOS"
#elif linux
#define PLATFORM        "Linux"
#elif sparc
#define PLATFORM        "Solaris"
#elif _AIX
#define PLATFORM        "AIX"
#else
#define PLATFORM        "generic"
#endif

/* odbc driver provider database type */
enum odrvr         { ODR_SQLITE, ODR_ORACLE, ODR_PG, ODR_MYSQL, ODR_SQLSRVR, ODR_ADS, ODR_UNKNOWN };
/* runform modes */
enum fmode         { MOD_INSERT, MOD_QUERY, MOD_UPDATE, MOD_DELETE };
/* field types */
enum ftype         { FTY_ALL, FTY_CHAR, FTY_INT, FTY_FLOAT, FTY_DATE, FTY_BOOL };
/* pages array index */
enum upage         { PGE_STATUS, PGE_MAIN, PGE_KEYHELP, PGE_EDITOR, PGE_ABOUT, PGE_EXTRA };
/* field edit variants for pos parameter */
#define FED_FEDITOR -9999 /* full screen editor */
#define FED_TRIGGER -9998 /* trigger "editor" */
#define FED_TOGGLE  -9997 /* binary toggle */
#define FED_INCR    -9996 /* increment */
#define FED_DECR    -9995 /* decrement */
#define FED_SPECIAL -9000 /* barrior for non-standard editors */
/* runform modes display names */
#define RMODENAMES { "Insert",   "Query ",  "Update",   "Delete" }

/* global resource configuration */
#define STATUSL 0
#define TNYSIZE 80
#define SMLSIZE 256
#define NORSIZE 512
#define MEDSIZE 2048
#define BIGSIZE 65536
#define HUGSIZE 1024*1024
#define NLINES  60
#define NBLOCKS 12
#define NFIELD1 30
#define NFIELDS 60
#define NTRIGGERS 80
#define NPRIKEY 4
#define NBINDPA NFIELD1

#include "version.h"
#include "company.h"
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "logger.h"
#include "qdata.h"
#include "record.h"
#include "rerror.h"
#include "rblock.h"
#include "rfield.h"
#include "rpage.h"
#include "rmap.h"
#include "block.h"
#include "field.h"
#include "screen.h"
#include "page.h"
#include "trigger.h"
#include "form.h"
#include "function.h"

/* misc helper macros */
#define str(s) #s
#define max(x, y) (((x) < (y)) ? (y) : (x))
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define let(target,source) letstrncpy(target, source, sizeof(target)-1)
#define t(target) target, sizeof(target)
#define empty(strp) *strp = '\0'
#define debugs(string) fprintf(stderr, ":%s:\n", string);

/* access to the current running dbs form mode blocks fields */
extern Logger g;
extern Record dbconn[5];
extern Screen y;
extern Form *f;
extern Function u;
extern char a[HUGSIZE];
extern int lastgetch;
#define F(method) f->method
#define CFi F(curfield)
#define CBi F(curblock)
#define CF F(l)[CFi]
#define CB F(b)[CBi]
#define CP F(p)[CF.pageindex]
#define CR CB.currentrec
#define CM CB.rmode
#define CN CB.q->rows
#define CV *CF.valuep()
#define CK F(lastcmd)
#define LK F(lastkey)
#define MSG(n) F(p)[0].message(n, NULL)
#define MSG1(n,c) F(p)[0].message(n, c)
#define blki(i) F(b)[i]
#define fldi(i) F(l)[i]
#define trgi(i) F(r)[i]
#define forall(item) for (i=0; i<F(num ## item); i++)
/* helpers from version.cpp */
extern char *letstrncpy(char *dest, const char *src, size_t n);
extern char *rtrim0white(char *str);
extern int yesno(int c);
extern int isprintable(int c);
extern int ispunctation(int c);
extern int letf(char *target, size_t maxlen, const char *format, ...);
extern int cats(char *target, size_t maxlen, const char *source);
extern int catc(char *target, size_t maxlen, char source);
extern int debugf(char *format, ...);
extern char *tmpcreat();
extern int tmpopen();
extern void tmpclose(int rm);
extern int tmpread(char *buf, int siz);
extern char *tmpget(char *buf, int siz);
extern void tmput(char *v);

/* extern components */
extern "C" {
int genxorkey(char *frm, const char *key);
char *xdecrypt(char *toe, int rev);
int res4key(char *key);
int mainloop(char *buf, WINDOW *scr);
}

/* global varibles mostly command line options */
extern int   useodbcve3;
extern int   monochrome;
extern int   usedefault;
extern int   firststart;
extern int   insertmode;
extern int   pwdencrypt;
extern int   squerymode;
extern int   updatemode;
extern int   usebindvar;
extern int   querycharm;
extern int   autocommit;
extern int   deleprompt;
extern int   queryonlym;
extern int   matchnocas;
extern int   globalpkid;
extern int   watchmacro;
extern int   noentermac;
extern char  *shiftednum;
extern char  *username;
