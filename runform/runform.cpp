/* runform.cpp main and os interfaces except curses and odbc */

#define USAGE "runform-(%02d) %s\nusage: runform [-3abcdhikmpqwxz] [-n lg]\n" \
  " [-f formid ] [-g logfile] [-l driverlib] [-t totpkey ] form.frm [user[:pass]@][sq3|dsn]...\n"

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <locale.h>
#include "runform.h"

/* fast keys for different keyboard layouts for -n option */
char shiftedus[] = "/!@#$%^&*().,";
char shifteduk[] = "/!\"£$%^&*().,";
char shiftedde[] = "-!\"§$%&/()=.,";
char shiftedfr[] = "!&é\"'(-è_çá;,";
char *shiftednum = shiftedus;

/* options are global vars */
char *lclocale;
int  firststart  = 1;
int  insertmode  = 1;
int  useodbcve3  = 0;             // -3
int  autocommit  = 1;             // -a
int  usebindvar  = 1;             // -b
int  usedefault  = 0;             // -c
int  deleprompt  = 0;             // -d
int  querycharm  = 1;             // -h
int  squerymode  = 0;             // -i
int  globalpkid  = 1;             // -j
int  monochrome  = 0;             // -k
int  matchnocas  = 1;             // -m
int  pwdencrypt  = 0;             // -p
int  queryonlym  = 0;             // -q
int  redirected  = 0;             // -r
int  watchmacro  = 0;             // -w
int  updatemode  = 0;             // -x
int  noentermac  = 0;             // -z
char *ypassword  = NULL;
char *username;
int  screenclos  = 1;
int  callinguid  = -1;
char about[SMLSIZE];

/* global form dbs screen functions and logger */
Logger g;
Record dbconn[5];
Screen y;
Form *f;
Function u;
int lastgetch = 256;

static char b64pwd[65];
char runningsignature[85];

/* local primary errors before any processing */
static void usage(int ecd) {
const char *est[] = {
  "command line options wrong",   //  1
  "command line arguments wrong", //  2
  "frm-file not found",           //  3
  "open frm-file failed",         //  4
  "read form failed",             //  5
  "run form failed",              //  6
  "form not found",               //  7
  "open database failed",         //  8
  "read database failed",         //  9
  "SQL syntax error",             // 10
  "SQL execute error",            // 11
  "SQL columns error",            // 12
  "no more memeory",              // 13
  "SQL getdata error",            // 14
  "SQL bind error",               // 15
  "logging failed",               // 16
  "screen setup failed",          // 17
  "need new key for encryption",  // 18
  "wrong version of form",        // 19
  "signature does not match",     // 20
  "form signed with signature",   // 21
  "feature was disabled",         // 22
};
fprintf(stderr, USAGE, ecd, est[ecd-1]);
exit(ecd);
}

/* disassemble the username:password@dsn connection string
 * and do the decryption when appropriate
 */
static void parsedsn(char *dsn, char *drv, char *dsn0) {
char *pwd;
char *dsn1;
FILE *filesq3;
if (strchr(dsn0, ';') == NULL && (filesq3 = fopen(dsn0, "r+"))) {
                                            fclose(filesq3);
    snprintf(dsn, MEDSIZE, "Driver=%s;Database=%s", drv, dsn0);
} else if ((dsn1 = strchr(dsn0, '@'))) {
  *dsn1++ = '\0';
  if ((pwd = strchr(dsn0, ':'))) {
    *pwd++ = '\0';
    let(b64pwd, pwd);
#ifndef NOUSECURITY
    if (pwdencrypt) xdecrypt(b64pwd, 1);
#endif
    snprintf(dsn, MEDSIZE, "DSN=%s;UID=%s;PWD=%s", dsn1, dsn0, b64pwd);
  } else {
    snprintf(dsn, MEDSIZE, "DSN=%s;UID=%s", dsn1, dsn0);
  }
} else {
    snprintf(dsn, MEDSIZE, "DSN=%s", dsn0);
} }

int main(int argc, char *argv[]) { //, char **envp
int i, j, s, form_id;
char dsn0[SMLSIZE];
char dsn[MEDSIZE];
#ifndef NOUSECURITY
char totpdigest[8];
char totpresult[8];
#endif
Form *rootform;
const char *ds;

/* version information and about and other runtime information */
#ifdef WIN32
callinguid = 202;
#else
signal(SIGTSTP, SIG_IGN);
callinguid = getuid();
#endif
ds = __DATE__; /* :Oct 16 2024: https://formax.toarx.de/ */
letf(t(about), "v%s %s " CCOMPILER " ODBC-%s CURS-%s %2.2s%3.3s%2.2s-%5.5s",
  VERSION, CHARSET, odbcversion+2, cursesversion, ds+4, ds, ds+9, __TIME__);
/* search for the sqlite3 driver */
#ifdef WIN32
char drv[SMLSIZE] = "SQLite3 ODBC Driver";
#else
char drv[SMLSIZE] = "libsqlite3odbc.so";
FILE *filesq3;
const char *drvs[] = {
  "/opt/arx/lib/libsqlite3odbc.so",
  "/opt/sqlite/lib/libsqlite3odbc.so",
  "/usr/lib/x86_64-linux-gnu/odbc/libsqlite3odbc.so",
  "/usr/lib64/libsqlite3odbc.so"
};
for (i=0; i<3; i++) {
  if ((filesq3 = fopen(drvs[i], "r"))) {
    fclose(filesq3);
    strcpy(drv, drvs[i]);
  }
}
#endif

/* user and charset environment */
#ifdef WIN32
username = getenv("USERNAME");
#else
username = getenv("USER");
setenv("LC_ALL", CHARSET, 1);
#endif
lclocale = setlocale(LC_ALL, CHARSET);

form_id = 1;

/* command-line arguments and options check and process */
while ((i = getopt(argc, argv, "3abcdf:g:hij:kl:mn:pqrt:Vwxy:z")) != -1) {
  switch (i) {
    case 'V': fprintf(stderr, "runform %s\n  (%d) [%s]\n", about, (int)sizeof(Form), GITCOMMIT); exit(2);
    case 'y': ypassword = optarg; break;
    case 't':
#ifndef NOUSECURITY
      fputs("TOTP: ", stdout);
      if (!fgets(totpdigest, 8, stdin)) usage(1);
      snprintf(totpresult, 8, "%06d\n", res4key(optarg));
      if (strcmp(totpdigest, totpresult)) usage(1);
      break;
#else
      usage(22);
#endif
    case 'f': form_id = atoi(optarg); break;
    case 'g':
#ifdef USELOGGING
      if (g.setlogfile(optarg)) { usage(16); } break;
#else
      usage(22);
#endif
    case 'l': let(drv, optarg); break;
    case 'n':
      #define shiftedlang(lang) if (!strcmp(optarg, #lang)) shiftednum = shifted ## lang
      shiftedlang(us);
      shiftedlang(uk);
      shiftedlang(de);
      shiftedlang(fr);
      break;
    case '3': useodbcve3 = 1; break;
    case 'k': monochrome = 1; break;
    case 'c': usedefault = 1; break;
    case 'p':
#ifndef NOUSECURITY
              pwdencrypt = 1; break;
#else
              usage(22);
#endif
    case 'i': squerymode = 1; break;
    case 'x': updatemode = 1; break;
    case 'b': usebindvar = 0; break;
    case 'h': querycharm = 0; break;
    case 'a': autocommit = 0; break;
    case 'd': deleprompt = 1; break;
    case 'q': queryonlym = 1; break;
    case 'w': watchmacro = 1; break;
    case 'm': matchnocas = 0; break;
    case 'z': noentermac = 1; break;
    case 'r': redirected = 1; break;
    case 'j': globalpkid = atoi(optarg); break;
    default: usage(1);
  }
}

/* generate encrypted passwords - must be root */
#ifndef NOUSECURITY
if (ypassword) {
  if (callinguid) usage(1);
  let(b64pwd, ypassword);
  printf("%s\n", xdecrypt(b64pwd, 0));
  printf("%s\n", xdecrypt(b64pwd, 1));
  exit(99);
}

/* build the signature of the form-file */
if (pwdencrypt && (i = genxorkey(argv[optind], runningsignature))) usage(i);
#endif

/* initialize the logger with first dsn */
g.init(argv[optind+1]);

/* open the form database - sqlite3 file named .frm */
snprintf(dsn, sizeof(dsn), "Driver=%s;Database=%s;", drv, argv[optind]);
if (dbconn[0].connect(dsn)) usage(4);

/* check and open the database connections
 * if simple rw-filepath use sqlite
 */
j = argc - optind;
if (j < 2 || j > 5) usage(2);
for (i=0; i<4; i++) {
  if (j > i + 1) {
    let(dsn0, argv[optind+i+1]);
    parsedsn(dsn, drv, dsn0);
    if (dbconn[i+1].connect(dsn)) usage(8);
        dbconn[i+1].ropen();
  } else {
    dbconn[i+1].connect(NULL);
  }
}
switch(dbconn[1].drv) {
 case ODR_SQLITE: querycharm = 2; break;
 case ODR_ADS:    querycharm = 0; break;
 default: ;
}
memset(dsn, 'y', MEDSIZE); // remove pw and key from ram
#ifndef NOUSECURITY
genxorkey(NULL, NULL);
#endif

rootform = new Form();
  if ((s = rootform->fill(form_id))) usage(s<19 ? 5 : s);
    if (!redirected) if ((screenclos = y.init())) usage(17);
      if ((s = rootform->run()) < -1) usage(6); /* returns notrunning 0..goon -1..quit <-1..error >0..form_id */
    y.closedisplay();
    screenclos = 1;
  rootform->clear();
delete(rootform);

for (i=0; i<5; i++) {
  dbconn[i].rclose();
  dbconn[i].disconnect();
}
g.lclose();

exit(s==-1 ? 0 : abs(s));
}

