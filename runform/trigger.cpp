#include "runform.h"

int Trigger::init(Qdata *trg, int rix) {
let(name,trg->v(rix, 1));
let(body,trg->v(rix, 2));
return 0;
}

