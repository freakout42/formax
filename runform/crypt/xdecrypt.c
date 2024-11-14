#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"
#include "../arx.h"

#define XORKEY0 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
static char xorder[65] = XORKEY0;
static char xorkey[65] = XORKEY1;

/* the index of the base64 chars for shifting */
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

/* simple shift algo secure since key > pass */
char *xdecrypt(char *toe, int rev) {
int i, j, k;
char buf[256];
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

/* key is in arx.h */
int genxorkey(char *pat, char *sig) {
  char *buf;
  char *signature;
  char insignia[] = "YyformaxyY";
  int fsize;
  FILE* f;
  struct MD5Context ctx;
  char d[48];
  if (strlen(xorkey)!=64 || !strncmp(xorkey, "qT", 2)) pat = NULL;
  if (pat == NULL) memset(xorkey, 'y', 64); else {
  if ((f = fopen(pat, "r")) == NULL) return 4;
  fseek(f, 0, SEEK_END);
  fsize = ftell(f);
  fseek(f, 0, SEEK_SET);
  if ((buf = malloc(fsize + 1)) == NULL) return 13;
  if ((fread(buf, 1, fsize, f) != fsize)) return 5;
  fclose(f);
  /* the checksum should not differ when the signature is y-blanked and the header is excluded */
  if ((signature = memmem(buf, fsize, insignia, strlen(insignia)))) memset(signature + 10, 'y', 64);
  MD5Init(&ctx);
  MD5Update(&ctx, buf+256, fsize-256);
  MD5Update(&ctx, xorkey, 64);
  MD5Final(d, &ctx);
  memcpy(d+16, d, 16);
  memcpy(d+32, d, 16);
  strcpy(sig, insignia);
  to64frombits(sig+10, d, 48);
  strcpy(sig+74, insignia);
  free(buf);
  }
  return 0;
}

