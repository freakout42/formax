/* https://en.wikipedia.org/wiki/HMAC
 * hmac = hash ( k^opad || hash( k^ipad || msg ))
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define IPAD 0x36
#define OPAD 0x5C

int sha1(uint8_t *digest, const uint8_t *data, size_t databytes);

void hmacsha1(void *d, void *k, int kl, void *msg, int msgl) {
	int i;
	char *buf;
  uint8_t hash[20];
  if (kl > 64) return;
  if ((buf = malloc(64 + 20 + msgl)) == NULL) return;
  memcpy(buf, k, kl);
  memset(buf+kl, 0, 64 - kl);
	for (i=0; i<64; ++i) buf[i] ^= IPAD;
  memcpy(buf+64, msg, msgl);
	sha1(hash, buf, 64 + msgl);
  memcpy(buf, k, kl);
  memset(buf+kl, 0, 64 - kl);
	for (i=0; i<64; ++i) buf[i] ^= OPAD;
  memcpy(buf+64, hash, 20);
	sha1(d, buf, 64 + 20);
  free(buf);
}
