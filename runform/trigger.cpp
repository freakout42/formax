/* triggers are sets of processing commands associated with event points,
 * such as when a particular function key is pressed by the operator
 */
#include "runform.h"

int Trigger::init(Qdata *trg, int rix) {
let(name,trg->v(rix, 1));
trgblk = trg->n(rix, 2);
trgfld = trg->n(rix, 3);
trgtyp = trg->n(rix, 4);
let(body,trg->v(rix, 5));
return 0;
}

