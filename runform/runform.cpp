#define USAGE "runform-(%02d) %s\nusage: runform [-3abcdhikpq] [-n lg]\n" \
              "  [-g logfile] [-l driverlib] form.frm sq3|dsn [username] [password]\n"
#define FORMFRM argv[optind+                 0]       //       //         //
#define FORMDSN argv[optind+                          1]       //         //
#define FORMUID argv[optind+                                   2]         //
#define FORMPWD argv[optind+                                              3]

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include "runform.h"

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

// global
char *lclocale;
int  firststart  = 1;
int  insertmode  = 1;
int  useodbcve3  = 0;             // -3
int  monochrome  = 0;             // -k
int  usedefault  = 0;             // -c
int  pwdencrypt  = 0;             // -p
int  squerymode  = 1;             // -i
int  usebindvar  = 1;             // -b
int  querycharm  = 1;             // -h
int  autocommit  = 1;             // -a
int  deleprompt  = 0;             // -d
int  queryonlym  = 0;             // -q
char *shiftednum = "`!@#$%^&*()"; // -n us
char *username   = getenv("USER");
Form f;
Logger g;

int main(int argc, char *argv[]) { //, char **envp
int i, s;
char dsn[SMLSIZE];
char drv[SMLSIZE] = "libsqlite3odbc.so";
FILE *filesq3;

setenv("LC_ALL", CHARSET, 1);
lclocale = setlocale(LC_ALL, CHARSET);

// command-line arguments and options check and process
while ((i = getopt(argc, argv, "3abcdg:hikl:n:pqVy:")) != -1) {
  switch (i) {
    case 'V': fprintf(stderr, "runform %s\n", VERSION); exit(2);
    case 'y': printf("%s\n", xdecrypt(optarg,0));
              printf("%s\n", xdecrypt(optarg,1)); exit(99);
    case 'g': if (g.setlogfile(optarg)) usage(16); break;
    case 'l': let(drv, optarg); break;
    case 'n':
      if (!strcmp(optarg, "us")) ; // shiftednum = "`!@#$%^&*()";
      if (!strcmp(optarg, "uk"))      shiftednum = "\\!\"£$%^&*()";
      if (!strcmp(optarg, "de"))      shiftednum = "<!\"§$%&/()=";
      if (!strcmp(optarg, "fr"))      shiftednum = "<&é\"'(-è_çá";
      break;
    case '3': useodbcve3 = 1; break;
    case 'k': monochrome = 1; break;
    case 'c': usedefault = 1; break;
    case 'p': pwdencrypt = 1; break;
    case 'i': squerymode = 0; break;
    case 'b': usebindvar = 0; break;
    case 'h': querycharm = 0; break;
    case 'a': autocommit = 0; break;
    case 'd': deleprompt = 1; break;
    case 'q': queryonlym = 1; break;
    default: usage(1);
  }
}
g.init(FORMDSN);

// check and open the database connection - if simple rw-filepath use sqlite
switch(argc - optind) {
 case 2:
  if (strcspn(FORMDSN,";") == strlen(FORMDSN) && (filesq3 = fopen(FORMDSN, "r+"))) {
    fclose(filesq3);
    snprintf(dsn, sizeof(dsn), "Driver=%s;Database=%s", drv, FORMDSN);
  } else snprintf(dsn, sizeof(dsn), "DSN=%s", FORMDSN);
  break;
 case 3:
  snprintf(dsn, sizeof(dsn), "DSN=%s;UID=%s", FORMDSN, FORMUID);
  break;
 case 4:
  if (pwdencrypt) xdecrypt(FORMPWD,1);
  snprintf(dsn, sizeof(dsn), "DSN=%s;UID=%s;PWD=%s", FORMDSN, FORMUID, FORMPWD);
  break;
 default: usage(2);
}
if (f.b[0].connect(dsn)) usage(8);
if (f.b[0].drv == ODR_SQLITE) querycharm = 2;
for (i=1; i<NBLOCKS; i++) f.b[i].connect(f.b[0]);

// check, open and read the form - sqlite3 file named .frm
if ((filesq3 = fopen(FORMFRM, "r")) == NULL) usage(3);
fclose(filesq3); // check for file existence because sqlite creates empty db
snprintf(dsn, sizeof(dsn), "Driver=%s;Database=%s;", drv, FORMFRM);
if (f.connect(dsn)) usage(4);
f.rconnect();

// load and run the form
s = 1;
while(s) {
  if ((f.fill(s))) usage(5);
    if ((s = f.run()) < 0) usage(6);
  f.clear();
}

// cleanup end exit
f.disconnect();
f.b[0].disconnect();
g.lclose();
exit(-s);
}
