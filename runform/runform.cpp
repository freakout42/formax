#define USAGE "runform-(%02d) %s\nusage: runform [-k] [-l driverlib] form.frm sq3|dsn [username] [password]\n"
#define FORMFRM argv[optind+ /* command-line arguments */            0]       //       //         //
#define FORMDSN argv[optind+                                                  1]       //         //
#define FORMUID argv[optind+                                                           2]         //
#define FORMPWD argv[optind+                                                                      3]

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "runform.h"

static void usage(int ecd) {
char *est[] = {
  "command line options wrong",   //  1
  "command line arguments wrong", //  2
  "frm-file not found",           //  3
  "open frm-file failed",         //  4
  "read form failed",             //  5
  "run form failed (display)",    //  6
  "form not found",               //  7
  "open database failed",         //  8
  "read database failed",         //  9
  "SQL syntax error",             // 10
  "SQL execute error",            // 11
  "SQL columns error",            // 12
  "no more memeory",              // 13
  "SQL getdata error"             // 14
};
fprintf(stderr, USAGE, ecd, est[ecd-1]);
exit(ecd);
}

// global
int monochrome = 0;
Form f;

int main(int argc, char *argv[]) { //, char **envp
int i;
char dsn[SMLSIZE];
char drv[SMLSIZE] = "libsqlite3odbc.so";
FILE *filesq3;

// command-line arguments and options check and process
while ((i = getopt(argc, argv, "l:kVy:")) != -1) {
  switch (i) {
    case 'V': fprintf(stderr, "runform %s\n", VERSION); exit(2);
    case 'y': printf("%s\n", xencrypt(optarg,0));
              printf("%s\n", xencrypt(optarg,1)); exit(99);
    case 'l': let(drv, optarg); break;
    case 'k': monochrome = 1; break;
    default: usage(1);
  }
}

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
  xencrypt(FORMPWD,1);
  snprintf(dsn, sizeof(dsn), "DSN=%s;UID=%s;PWD=%s", FORMDSN, FORMUID, FORMPWD);
  break;
 default: usage(2);
}
if (f.b[0].connect(dsn)) usage(8);
for (i=1; i<NBLOCKS; i++) f.b[i].dbc = f.b[0].dbc;

// check, open and read the form - sqlite3 file named .frm
if ((filesq3 = fopen(FORMFRM, "r")) == NULL) usage(3);
fclose(filesq3); // check for file existence because sqlite creates empty db
snprintf(dsn, sizeof(dsn), "Driver=%s;Database=%s;", drv, FORMFRM);
f.locale(CHARSET);
if (f.connect(dsn)) usage(4);
f.rblock.dbc = f.dbc;
f.rpage.dbc = f.dbc;
f.rmap.dbc = f.dbc;
if (f.init()) usage(5);

// load and run the form
if ((i = f.fill(1))) usage(i);
  if ((i = f.run())) usage(i);
f.close();

f.disconnect();
f.b[0].disconnect();
exit(i);
}
