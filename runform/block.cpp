#include <string.h>
#include "version.h"
#include "qdata.h"
#include "record.h"
#include "block.h"

int Block::init(Qdata pag, int rix) {
//if (open(NULL)) return 1;
name   = pag.c(rix, 1);
return 0;
}
