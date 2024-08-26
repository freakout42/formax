#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

#define XORKEY0 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
static char xorder[] = XORKEY0;
static char xorkey[65];

static int ordnum(int ch) {
char *po;
if ((po = strchr(xorder, ch))) {
  return po - xorder;
} else {
  return -1;
} }
static char numord(int on) {
return xorder[on];
}

char *xdecrypt(char *toe, int rev) {
int i, j, k;
unsigned char buf[256];
if (strlen(toe) < 64) {
if (!rev) { to64frombits(buf, toe, strlen(toe)+1); strcpy(toe, buf); }
for (i=0; toe[i] && i < 64; i++) {
  j = ordnum((unsigned char)toe[i]);
  if (j != -1) {
    k = ordnum((unsigned char)xorkey[i]);
    toe[i] = numord((j + (rev ? (sizeof(XORKEY0)-1) - k : k) ) % (sizeof(XORKEY0)-1));
  }
}
if (rev) { from64tobits(buf, toe, 64); strcpy(toe, buf); }
}
return toe;
}

int genxorkey(char *pat, char *key) {
  char *buf;
  int fsize;
  FILE* f;
  struct MD5Context ctx;
  unsigned char d[48];

  if ((f = fopen(pat, "r")) == NULL) return 4;
  fseek(f, 0, SEEK_END);
  fsize = ftell(f);
  fseek(f, 0, SEEK_SET);
  if ((buf = malloc(fsize + 1)) == NULL) return 13;
  if ((fread(buf, 1, fsize, f) != fsize)) return 5;
  fclose(f);

  MD5Init(&ctx);
  MD5Update(&ctx, buf, fsize);
  MD5Update(&ctx, key, 64);
  MD5Final(d, &ctx);
  memcpy(d+16, d, 16);
  memcpy(d+32, d, 16);
  to64frombits(xorkey, d, 48);
  xorkey[64] = '\0';
  free(buf);
  return 0;
}
