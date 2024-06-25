const char VERSION[] = "0.9.0";
#define XORKEY1 "qTYaqw8zhU3UYT73hiohenUSsio85uibEWd72672JdQwfnytomHomU0Ny6eu"
#define CHARSET "en_US.iso885915"
#define STATUSL 0
#define SMLSIZE 256
#define MEDSIZE 1024
#define BIGSIZE 65536
#define HUGSIZE 1024*1024
#define NLINES  100
#define NBLOCKS 20
#include "qdata.h"
#include "record.h"
#include "rblock.h"
#include "rpage.h"
#include "rmap.h"
#include "block.h"
#include "screen.h"
#include "page.h"
#include "form.h"
#define let(target,source) strncpy(target, source, sizeof(target))
#define t(target) target, sizeof(target)
extern int letf(char *target, size_t maxlen, char *format, ...);
extern char *xencrypt(char *toe, int rev);
extern int monochrome;
