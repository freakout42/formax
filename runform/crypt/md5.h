#include <inttypes.h>
#define UWORD32 uint32_t

struct MD5Context {
  UWORD32 buf[4];
  UWORD32 bytes[2];
  UWORD32 in[16];
};

void MD5Init(struct MD5Context *context);
void MD5Update(struct MD5Context *context, char *buf, unsigned len);
void MD5Final(char digest[16], struct MD5Context *context);
void MD5Transform(UWORD32 buf[4], UWORD32 in[16]);

void to64frombits(char *out, char *in, int inlen);
int from64tobits(char *out, char *in, int maxlen);
