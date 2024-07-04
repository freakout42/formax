// constants macros and central procedures
#include "../version.h"

#define STATUSL 0
#define SMLSIZE 256
#define MEDSIZE 1024
#define BIGSIZE 65536
#define HUGSIZE 1024*1024
#define NLINES  100
#define NBLOCKS 20
#define NFIELD1 40
#define NFIELDS NBLOCKS*NFIELD1
#define NPRIKEY 4
#define NBINDPA 8

#include <string.h>
#include "qdata.h"
#include "record.h"
#include "rerror.h"
#include "rfield.h"
#include "rblock.h"
#include "rpage.h"
#include "rmap.h"
#include "field.h"
#include "block.h"
#include "screen.h"
#include "page.h"
#include "form.h"
#include "function.h"

#define max(x, y)	(((x) < (y)) ? (y) : (x))
#define min(x, y)	(((x) < (y)) ? (x) : (y))
#define let(target,source) strncpy(target, source, sizeof(target))
#define t(target) target, sizeof(target)

#define CB f.b[f.curblock]
#define CF f.l[f.curfield]

extern int isprintable(int c);
extern int letf(char *target, size_t maxlen, char *format, ...);
extern int cats(char *target, size_t maxlen, char *source);
extern int debugf(char *format, ...);
extern char *xencrypt(char *toe, int rev);
extern int monochrome;
extern int usedefault;
extern int insertmode;
extern int squerymode;
extern int autocommit;
extern int deleprompt;
extern Form f;
