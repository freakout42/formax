const char VERSION[] = "0.9.0";
#define CHARSET "en_US.iso885915"
#define STATUSL 0
#define NBLOCKS 20
#define SMLSIZE 256
#define MEDSIZE 1024
#define BIGSIZE 65536
#define HUGSIZE 1024*1024
#define NLINES  100
#define let(target,source) strncpy(target, source, sizeof(target))
#define t(target) target, sizeof(target)
extern int letf(char *target, size_t maxlen, char *format, ...);
extern int monochrome;
