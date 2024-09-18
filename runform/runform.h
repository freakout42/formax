/* constants macros and central procedures */
enum odrvr         { ODR_SQLITE, ODR_ORACLE, ODR_PG, ODR_MYSQL, ODR_SQLSRVR, ODR_ADS, ODR_UNKNOWN };
enum fmode         { MOD_INSERT, MOD_QUERY, MOD_UPDATE, MOD_DELETE };
enum ftype         { FTY_ALL, FTY_CHAR, FTY_INT, FTY_FLOAT, FTY_DATE, FTY_BOOL };
enum upage         { PGE_STATUS, PGE_MAIN, PGE_KEYHELP, PGE_EDITOR, PGE_EXTRA };
#define RMODENAMES { "Insert",   "Query ",  "Update",   "Delete" }
#include "../version.h"

/* global resource configuration */
#define STATUSL 0
#define SMLSIZE 256
#define MEDSIZE 1024
#define BIGSIZE 65536
#define HUGSIZE 1024*1024
#define NLINES  100
#define NBLOCKS 8
#define NFIELD1 40
#define NFIELDS NBLOCKS*NFIELD1
#define NTRIGGERS 80
#define NPRIKEY 4
#define NBINDPA NFIELD1

#include <assert.h>
#include <string.h>
#include "regex/re.h"
#include "elk/elk.h"
#include "logger.h"
#include "qdata.h"
#include "record.h"
#include "rerror.h"
#include "rfield.h"
#include "rblock.h"
#include "rpage.h"
#include "rmap.h"
#include "rtrigger.h"
#include "field.h"
#include "block.h"
#include "screen.h"
#include "page.h"
#include "trigger.h"
#include "form.h"
#include "function.h"

/* misc helper macros */
#define max(x, y)	(((x) < (y)) ? (y) : (x))
#define min(x, y)	(((x) < (y)) ? (x) : (y))
#define let(target,source) strncpy(target, source, sizeof(target)-1)
#define t(target) target, sizeof(target)
#define debugs(string) fprintf(stderr, ":%s:\n", string);

/* access to the current running form mode block field */
extern Form *f;
extern Logger g;
#define F(method) f->method
#define CM F(rmode)
#define CB F(b)[F(curblock)]
#define CF F(l)[F(curfield)]
#define LK F(lastkey)
#define MSG(n) F(p)[0].message(n, NULL)
#define MSG1(n,c) F(p)[0].message(n, c)

/* helpers from version.cpp */
extern Function u;
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
extern char  *shiftednum;
extern char  *username;
