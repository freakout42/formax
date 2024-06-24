#include <cstdarg>
#include <stdio.h>
#include "version.h"

// avoid include <stdio.h> need
int letf(char *target, size_t maxlen, char *format, ...) {
va_list args;
int n;
va_start (args, format);
n = vsnprintf (target, maxlen, format, args);
va_end (args);
return n;
}

//static char xorder[] = "%^&-_=+@?.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static char xorkey[] = XORKEY1;
static int ordnum(int ch) {
int on;
     if (ch>=48 && ch<=57 ) on = ch - 48; /* 0-9   0- 9 */
else if (ch>=65 && ch<=90 ) on = ch - 55; /* A-Z  10-29 */
else if (ch>=97 && ch<=122) on = ch - 61; /* a-z  30-61 */
else                        on = 0;
return on;                                      /* 0-61 */
}
static char numord(int on) {
unsigned char ch;
     if (on<=9 ) ch = on + 48;
else if (on<=35) ch = on + 55;
else             ch = on + 61;
return (char)ch;
}
char *xencrypt(char *toe, int rev) {
int i, j, k;
for (i=0; toe[i] && i < 60; i++) {
  j = ordnum((unsigned char)toe[i]);
  k = ordnum((unsigned char)xorkey[i]);
  if (j) {
    toe[i] = numord((j + (rev ? 62 - k : k) ) % 62);
  }
}
return toe;
}
