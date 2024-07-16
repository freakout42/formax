/* #include <stdio.h> */
/* query data handling */
#include <assert.h>
#include <stdlib.h>
#include "runform.h"

void Qdata::init() {
rows = 0;
cols = 0;
allocatedrows = 0;
d = NULL;
}

/* factorize by 10 for performance */
int Qdata::alloc(int coln) {
freed();
rows = 0;
cols = coln;
allocatedrows = 2;
if (!(d = (char*(*)[])malloc(allocatedrows * cols * (sizeof(void*))))) return 13;
memset(d, 0, allocatedrows * cols * (sizeof(void*)));
return 0;
}

int Qdata::splice(int rown) {
int i;
char **clr;
if (rown > 0) {
  if (allocatedrows == rows) w(rows+1, 1);
  memmove(w(rown+2,1), w(rown+1,1), (rows-rown) * cols * (sizeof(void*)));
  memset(w(rown+1,1), 0, cols * (sizeof(void*)));
  rows++;
} else {
  rown *= -1;
  clr = w(rown,1);
  for (i=0; i<cols; i++) free(*(clr+i));
  memmove(w(rown,1), w(rown+1,1), (rows-rown) * cols * (sizeof(void*)));
  memset(w(rows,1), 0, cols * (sizeof(void*)));
  rows--;
}
return 0;
}

void Qdata::freed() {
int i;
for (i=0; i<rows*cols; i++) free((*d)[i]);
free(d);
init();
}

/* return a valid pointer for new query-column data */
char **Qdata::w(int row, int col) {
int i;
if (row > allocatedrows) {
  i = allocatedrows;
  if (allocatedrows) allocatedrows *= 10; else allocatedrows = 2;
  if (!(d = (char*(*)[])realloc(d, allocatedrows * cols * (sizeof(void*))))) return NULL;
  memset(w(i+1,1), 0, (allocatedrows - i) * cols * (sizeof(void*)));
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
int Qdata::m(int row, int col) {
int i;
i = atoi(v(row, col));
return i;
}

/* int value and dereferencing */
int Qdata::n(int row, int col) {
int i;
i = atoi(v(row, col));
free(*w(row, col));
*w(row, col) = NULL;
return i;
}

