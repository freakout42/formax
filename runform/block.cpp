#include <string.h>
#include "runform.h"

int Block::init(Qdata pag, int rix) {
//if (open(NULL)) return 1;
name = pag.c(rix, 1);
return 0;
}
