#include "runform.h"
#include <cstdarg>
#include <stdio.h>
#include <unistd.h>

/* optimized strncpy */
char *letstrncpy(char *dest, const char *src, size_t n) {
unsigned int i;
i = strlen(src);
if (i < n-2) {
  strcpy(dest, src);
} else {
  empty(dest);
}
return dest+i;
}

/* trim right padded garbade away .00 and whitespace */
char *rtrim0white(char *str) {
char *end;
end = str + strspn(str, "-0123456789");
if (*end == '.' && strspn(end, "0.") == strlen(end)) *end = '\0';
else if (*str) {
  end = str + strlen(str) - 1;
  while (end > str && isspace(*end)) end--;
  end[1] = '\0';
}
return str;
}

/* avoid include <stdio.h> need */
int letf(char *target, size_t maxlen, const char *format, ...) {
va_list args;
int n;
va_start (args, format);
n = vsnprintf (target, maxlen, format, args);
va_end (args);
return n;
}

/* temporary file handling */
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

/* string helpers */
int isprintable(int c) {
if ((c >= 32 && c <= 126) || (c >= 160 && c <= 255)) return c;
return 0;
}
int ispunctation(int c) {
char *pc;
if (c && (pc = strchr(shiftednum, c))) return KEF_NAVI(pc-shiftednum);
return c;
}

/* international yes/no */
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
} }

/* strncat(target, source, maxlen);
 * return strlen(target);
 */
int cats(char *target, size_t maxlen, const char *source) {
unsigned int i, j;
i = strlen(target);
j = strlen(source);
if (i+j < maxlen-2) strcpy(target+i, source);
else                empty(target);
return i + j;
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

