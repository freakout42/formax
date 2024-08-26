#ifndef MD5_H
#define MD5_H

#include <inttypes.h>
#define UWORD32 uint32_t
#define md5byte unsigned char

struct MD5Context {
        UWORD32 buf[4];
        UWORD32 bytes[2];
        UWORD32 in[16];
};

void MD5Init(struct MD5Context *context);
void MD5Update(struct MD5Context *context, md5byte const *buf, unsigned len);
void MD5Final(unsigned char digest[16], struct MD5Context *context);
void MD5Transform(UWORD32 buf[4], UWORD32 const in[16]);

void to64frombits(unsigned char *out, const unsigned char *in, int inlen);
int from64tobits(char *out, const char *in, int maxlen);

#endif /* !MD5_H */
