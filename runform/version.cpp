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

int debugf(char *format, ...) {
va_list args;
int n;
va_start (args, format);
n = vfprintf(stderr, format, args);
fputc('\n', stderr);
va_end (args);
return n+1;
}

static char xorder[] = "%^&-_=+@?.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
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
    toe[i] = numord((j + (rev ? 72 - k : k) ) % 72);
  }
}
return toe;
}

