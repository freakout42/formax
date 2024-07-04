#include <cstdarg>
#include <stdio.h>
#include "runform.h"

// avoid include <stdio.h> need
int letf(char *target, size_t maxlen, char *format, ...) {
va_list args;
int n;
va_start (args, format);
n = vsnprintf (target, maxlen, format, args);
va_end (args);
return n;
}

int isprintable(int c) {
return (c >= 32 && c <= 126) || (c >= 160 && c <= 255);
}

int cats(char *target, size_t maxlen, char *source) {
strncat(target, source, maxlen);
return strlen(target);
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

#define XORKEY0 "%^&-_=+@?.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
static char xorder[] = XORKEY0;
static char xorkey[] = XORKEY1;
static int ordnum(int ch) {
char *po;
if (po = strchr(xorder, ch)) {
  return po - xorder;
} else {
  return -1;
} }
static char numord(int on) {
return xorder[on];
}
char *xencrypt(char *toe, int rev) {
int i, j, k;
for (i=0; toe[i] && i < 60; i++) {
  j = ordnum((unsigned char)toe[i]);
  if (j != -1) {
    k = ordnum((unsigned char)xorkey[i]);
    toe[i] = numord((j + (rev ? (sizeof(XORKEY0)-1) - k : k) ) % (sizeof(XORKEY0)-1));
  }
}
return toe;
}

