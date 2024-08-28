#include <assert.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define T0 0
#define DIGITS 6
#define VALIDITY 30
#define TS 30

static const int8_t base32_vals[256] = {
 // 00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x00
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x10
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x20
    14, 11, 26, 27, 28, 29, 30, 31,  1, -1, -1, -1, -1,  0, -1, -1, // 0x30
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, // 0x40
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // 0x50
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, // 0x60
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, -1, -1, -1, -1, // 0x70
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x80
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x90
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xA0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xB0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xC0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xD0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xE0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xF0
};

static int validate_b32key(char *k, size_t len) {
size_t pos;
if (((len & 0xF) != 0) && ((len & 0xF) != 8)) return 1;
for (pos = 0; (pos < len); pos++) {
    if (base32_vals[(int)k[pos]] == -1)
        return 1;
    if (k[pos] == '=') {
        if (((pos & 0xF) == 0) || ((pos & 0xF) == 8))
            return(1);
        if ((len - pos) > 6)
            return 1;
        switch (pos % 8) {
        case 2:
        case 4:
        case 5:
        case 7:
            break;
        default:
            return 1;
        }
        for ( ; (pos < len); pos++) {
            if (k[pos] != '=')
                return 1;
        }
    }
}
return 0;
}

static size_t decode_b32key(uint8_t **k, size_t len) {
size_t keylen;
size_t pos;
keylen = 0;
for (pos = 0; pos <= (len - 8); pos += 8) {
  (*k)[keylen+0]  = (base32_vals[(*k)[pos+0]] << 3) & 0xF8; // 5 MSB
  (*k)[keylen+0] |= (base32_vals[(*k)[pos+1]] >> 2) & 0x07; // 3 LSB
  if ((*k)[pos+2] == '=') {
      keylen += 1;
      break;
  }
  (*k)[keylen+1]  = (base32_vals[(*k)[pos+1]] << 6) & 0xC0; // 2 MSB
  (*k)[keylen+1] |= (base32_vals[(*k)[pos+2]] << 1) & 0x3E; // 5  MB
  (*k)[keylen+1] |= (base32_vals[(*k)[pos+3]] >> 4) & 0x01; // 1 LSB
  if ((*k)[pos+4] == '=') {
      keylen += 2;
      break;
  }
  (*k)[keylen+2]  = (base32_vals[(*k)[pos+3]] << 4) & 0xF0; // 4 MSB
  (*k)[keylen+2] |= (base32_vals[(*k)[pos+4]] >> 1) & 0x0F; // 4 LSB
  if ((*k)[pos+5] == '=') {
  keylen += 3;
      break;
  }
  (*k)[keylen+3]  = (base32_vals[(*k)[pos+4]] << 7) & 0x80; // 1 MSB
  (*k)[keylen+3] |= (base32_vals[(*k)[pos+5]] << 2) & 0x7C; // 5  MB
  (*k)[keylen+3] |= (base32_vals[(*k)[pos+6]] >> 3) & 0x03; // 2 LSB
  if ((*k)[pos+7] == '=') {
      keylen += 4;
      break;
  }
  (*k)[keylen+4]  = (base32_vals[(*k)[pos+6]] << 5) & 0xE0; // 3 MSB
  (*k)[keylen+4] |= (base32_vals[(*k)[pos+7]] >> 0) & 0x1F; // 5 LSB
  keylen += 5;
}
(*k)[keylen] = 0;
return keylen;
}

void hmacsha1(void *d, void *k, int kl, void *msg, int msgl);

static uint8_t *hmac(unsigned char *key, int kl, uint64_t interval) {
static uint8_t d[20];
unsigned char *md;
md = (unsigned char *)&interval;
hmacsha1(d, key, kl, (unsigned char *)&interval, sizeof(interval)); 
return d;
}

static uint32_t DT(uint8_t *digest) {
uint64_t offset;
uint32_t bin_code;
// dynamically truncates hash
offset   = digest[19] & 0x0f;
bin_code = (digest[offset]  & 0x7f) << 24
    | (digest[offset+1] & 0xff) << 16
    | (digest[offset+2] & 0xff) <<  8
    | (digest[offset+3] & 0xff);
// truncates code to 6 digits
return bin_code;
}

uint32_t mod_hotp(uint32_t bin_code, int digits) {
int power = pow(10, digits);
uint32_t otp = bin_code % power;
return otp;
}

static uint32_t gethotp(uint8_t *key, size_t kl, uint64_t interval, int digits) {
uint8_t *digest;
uint32_t result;
uint32_t endianness;
endianness = 0xdeadbeef;
if ((*(const uint8_t *)&endianness) == 0xef) {
    interval = ((interval & 0x00000000ffffffffULL) << 32) | ((interval & 0xffffffff00000000ULL) >> 32);
    interval = ((interval & 0x0000ffff0000ffffULL) << 16) | ((interval & 0xffff0000ffff0000ULL) >> 16);
    interval = ((interval & 0x00ff00ff00ff00ffULL) <<  8) | ((interval & 0xff00ff00ff00ff00ULL) >>  8);
};
//First Phase, get the digest of the message using the provided key ...
digest = (uint8_t *)hmac(key, kl, interval);
//Second Phase, get the dbc from the algorithm
uint32_t dbc = DT(digest);
//Third Phase: calculate the mod_k of the dbc to get the correct number
result = mod_hotp(dbc, digits);
return result;
}

static uint32_t gettotp(uint8_t *key, size_t kl, uint64_t time, int digits) {
uint32_t totp;
totp = gethotp(key, kl, time, digits);
return totp;
}

static uint32_t totp(uint8_t *k, size_t keylen) {
time_t t = floor((time(NULL) - T0) / VALIDITY);
return gettotp(k, keylen, t, DIGITS);
}

uint32_t res4key(char *key) {
uint32_t result;
size_t len;
size_t keylen;
uint8_t *k;
len = strlen(key);
if (validate_b32key(key, len) == 1) return 9999999;
k = (uint8_t *)key;
keylen = decode_b32key(&k, len);
result = totp(k, keylen);
return result;
}

#ifdef WITHMAIN
int main(int argc, char *argv[]) {
int result;
result = res4key(argv[1]);
printf(":%06d:\n", result);
return 0;
}
#endif
