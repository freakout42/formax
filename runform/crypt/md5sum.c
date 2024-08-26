#include <stdlib.h>
#include <stdio.h>
#include "md5.h"

int main(int argc, char** argv)
{
  char *buf;
  char b64[512];
  int fsize, bsize;
  FILE* f;
  struct MD5Context ctx;
	unsigned char d[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	char x[32];

  f = fopen(argv[1], "r");
  fseek(f, 0, SEEK_END);
  fsize = ftell(f);
  fseek(f, 0, SEEK_SET);
  buf = malloc(fsize + 1);
  fread(buf, fsize, 1, f);
  fclose(f);

	MD5Init(&ctx);
  MD5Update(&ctx, buf, fsize);
	MD5Final(d, &ctx);
  to64frombits(b64, d, 16);
  bsize = from64tobits(buf, b64, fsize);
  buf[fsize] = '\0';

	sprintf(x, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
	       d[0],  d[1],  d[2],  d[3],
	       d[4],  d[5],  d[6],  d[7],
	       d[8],  d[9],  d[10], d[11],
	       d[12], d[13], d[14], d[15]);
	printf("%s  %s\n%s\n%s  %d %d\n", x, argv[1], b64, buf, bsize, fsize);
  to64frombits(b64, x, 32);
	printf("%s  %s\n", x, b64);

  free(buf);
  return 0;
}
