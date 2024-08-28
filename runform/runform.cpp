#define USAGE "runform-(%02d) %s\nusage: runform [-3abcdhikpqx] [-n lg]\n" \
  "  [-g logfile] [-l driverlib] [-t totpkey ] form.frm [user[:pass]@][sq3|dsn]...\n"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include "runform.h"

char *lclocale;
int  firststart  = 1;
int  insertmode  = 1;
int  useodbcve3  = 0;             // -3
int  monochrome  = 0;             // -k
int  usedefault  = 0;             // -c
int  pwdencrypt  = 0;             // -p
int  squerymode  = 0;             // -i
int  updatemode  = 0;             // -x
int  usebindvar  = 1;             // -b
int  querycharm  = 1;             // -h
int  autocommit  = 1;             // -a
int  deleprompt  = 0;             // -d
int  queryonlym  = 0;             // -q
char *shiftednum = "`!@#$%^&*()"; // -n us
char *ypassword  = NULL;
char *username;
Form f;
Logger g;
Function u;

static char b64pwd[65];

static void usage(int ecd) {
char *est[] = {
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
};
fprintf(stderr, USAGE, ecd, est[ecd-1]);
exit(ecd);
}

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
    if (pwdencrypt) xdecrypt(b64pwd, 1);
    snprintf(dsn, MEDSIZE, "DSN=%s;UID=%s;PWD=%s", dsn1, dsn0, b64pwd);
  } else {
    snprintf(dsn, MEDSIZE, "DSN=%s;UID=%s", dsn1, dsn0);
  }
} else {
    snprintf(dsn, MEDSIZE, "DSN=%s", dsn0);
}
}

int main(int argc, char *argv[]) { //, char **envp
int i, s;
char dsn0[SMLSIZE];
char dsn[MEDSIZE];
char totpdigest[8];
char totpresult[8];

// search for the sqlite3 driver
char drv[SMLSIZE] = "libsqlite3odbc.so";
FILE *filesq3;
char *drvs[] = { "/opt/sqlite/lib/libsqlite3odbc.so",
                 "/usr/lib/x86_64-linux-gnu/odbc/libsqlite3odbc.so",
                 "/usr/lib64/libsqlite3odbc.so"
               };
for (i=0; i<3; i++) {
  if ((filesq3 = fopen(drvs[i], "r"))) {
    fclose(filesq3);
    strcpy(drv, drvs[i]);
  }
}

username = getenv("USER");
setenv("LC_ALL", CHARSET, 1);
lclocale = setlocale(LC_ALL, CHARSET);

// command-line arguments and options check and process
while ((i = getopt(argc, argv, "3abcdg:hikl:n:pqt:Vxy:")) != -1) {
  switch (i) {
    case 'V': fprintf(stderr, "runform %s\n", VERSION); exit(2);
    case 'y': ypassword = optarg; break;
    case 't':
      fputs("TOTP: ", stdout);
      if (!fgets(totpdigest, 8, stdin)) usage(1);
      snprintf(totpresult, 8, "%06d\n", res4key(optarg));
      if (strcmp(totpdigest, totpresult)) usage(1);
      break;
    case 'g': if (g.setlogfile(optarg)) usage(16); break;
    case 'l': let(drv, optarg); break;
    case 'n':
      if (!strcmp(optarg, "us")) shiftednum = "`!@#$%^&*()";
      if (!strcmp(optarg, "uk")) shiftednum = "\\!\"�$%^&*()";
      if (!strcmp(optarg, "de")) shiftednum = "<!\"�$%&/()=";
      if (!strcmp(optarg, "fr")) shiftednum = "<&�\"'(-�_��";
      break;
    case '3': useodbcve3 = 1; break;
    case 'k': monochrome = 1; break;
    case 'c': usedefault = 1; break;
    case 'p': pwdencrypt = 1; break;
    case 'i': squerymode = 1; break;
    case 'x': updatemode = 1; break;
    case 'b': usebindvar = 0; break;
    case 'h': querycharm = 0; break;
    case 'a': autocommit = 0; break;
    case 'd': deleprompt = 1; break;
    case 'q': queryonlym = 1; break;
    default: usage(1);
  }
}

if ((i = genxorkey(argv[optind], XORKEY1))) usage(i);

if (ypassword) {
  if (getuid()) usage(1);
  let(b64pwd, ypassword);
  printf("%s\n", xdecrypt(b64pwd, 0));
  printf("%s\n", xdecrypt(b64pwd, 1));
  exit(99);
}

// check and open the database connection - if simple rw-filepath use sqlite
switch(argc - optind) {
 case 2:
  let(dsn0, argv[optind+1]);
  parsedsn(dsn, drv, dsn0);
  break;
 case 3:
 case 4:
 default: usage(2);
}
if (F(b[0]).connect(dsn)) usage(8);
memset(dsn, 'y', MEDSIZE);
genxorkey(NULL, NULL);

g.init(argv[optind+1]);
if (F(b[0]).drv == ODR_SQLITE) querycharm = 2;
for (i=1; i<NBLOCKS; i++) F(b[i]).connect(F(b[0]));

// open and read the form - sqlite3 file named .frm
snprintf(dsn, sizeof(dsn), "Driver=%s;Database=%s;", drv, argv[optind]);
if (F(connect)(dsn)) usage(4);
F(rconnect)();

// load and run the form
s = 1;
while(s) {
  if ((F(fill)(s))) usage(5);
    if ((s = F(run)()) < 0) usage(6);
  F(clear)();
}

// cleanup end exit
F(disconnect)();
F(b[0]).disconnect();
g.lclose();
exit(-s);
}
