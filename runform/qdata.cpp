/* query data handling */
#include <stdlib.h>
#include <string.h>
#include "runform.h"

void Qdata::init() {
rows = 0;
cols = 0;
d = NULL;
}

/* factorize by 10 for performance */
int Qdata::alloc(int coln) {
freed();
rows = 0;
cols = coln;
allocaterows = 1;
if (!(d = (char*(*)[])malloc(allocaterows * cols * (sizeof(void*))))) return 13;
return 0;
}

void Qdata::freed() {
int i;
for (i=0; i<rows*cols; i++) {
//  debugf("qda: %d %d %d %s",rows,cols,i,(*d)[i]);
  free((*d)[i]);
}
free(d);
init();
}

/* return a valid pointer for new query-column data */
char **Qdata::w(int row, int col) {
if (row > allocaterows) {
  allocaterows *= 10;
  if (!(d = (char*(*)[])realloc(d, allocaterows * cols * (sizeof(void*))))) return NULL;
}
return &((*d)[(row - 1) * cols + col - 1]);
}

/* value */
char *Qdata::v(int row, int col) {
return *w(row, col);
}

/* value and dereferencing */
char *Qdata::c(int row, int col) {
char *p;
p = *w(row, col);
*w(row, col) = NULL;
return p;
}

/* int value */
int Qdata::n(int row, int col) {
int i;
i = atoi(v(row, col));
*w(row, col) = NULL;
return i;
}
