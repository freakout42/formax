#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "runform.h"

// avoid include <stdio.h> need
int letf(char *target, size_t maxlen, const char *format, ...) {
va_list args;
int n;
va_start (args, format);
n = vsnprintf (target, maxlen, format, args);
va_end (args);
return n;
}

static char tfp[14];
static FILE *tf;

char *tmpcreat() {
strcpy(tfp, "/tmp/fmXXXXXX");
tf = fdopen(mkstemp(tfp), "w");
return tfp;
}

int tmpopen() {
tf = fopen(tfp, "r");
return tf != NULL;
}

void tmpclose(int rm) {
fclose(tf);
if (rm) unlink(tfp);
}

int tmpread(char *buf, int siz) {
return fread(buf, 1, siz, tf);
}

char *tmpget(char *buf, int siz) { return fgets(buf, siz, tf); }
void tmput(char *v) { if (*v) { if (v[1] == '\0') fputc(*v, tf); else fputs(v, tf); } }

int isprintable(int c) {
if ((c >= 32 && c <= 126) || (c >= 160 && c <= 255)) return c;
return 0;
}

int ispunctation(int c) {
char *pc;
if (c && (pc = strchr(shiftednum, c))) return KEF_NAVI(pc - shiftednum);
return c;
}

int yesno(int chr) {
switch(chr) {
 case KEY_ENTER:
 case 'y':
 case 'j':
 case 'o':
 case 'Y':
 case 'J':
 case 'O': return 1; break;
 default:  return 0;
}
}

int cats(char *target, size_t maxlen, const char *source) {
strncat(target, source, maxlen);
return strlen(target);
}

int catc(char *target, size_t maxlen, char source) {
int i;
i = strlen(target);
if (!source) return i;
if (i + 1 >= (int)maxlen) return 1;
target[i] = source;
target[i+1] = '\0';
return 0;
}

int debugf(char *format, ...) {
va_list args;
int n;
va_start (args, format);
n = vfprintf(stderr, format, args);
fputc('\n', stderr);
va_end (args);
return n+1;
}

