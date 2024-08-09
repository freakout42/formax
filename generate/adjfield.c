/* adjust field position and length in form from screen map */
#include <stdio.h>
#include <stdlib.h>

#define NXT getc(stdin); col++

int main() {
  int line=1, col=0, act, len;
  char t;
  char f[3];
  f[2] = '\0';
  while(!feof(stdin)) {
    t = NXT;
    if (t == '$') {
      act = col;
      f[0] = NXT;
      f[1] = t = NXT;
      len = 2;
      while(t != '\n' && (t == '_' || (t >= '0' && t <= '9'))) { t = NXT; len++; }
      printf("UPDATE fields set line = %d, col = %d, dlen = %d where id = %d;\n", line, act, len, atoi(f));
    }
    if (t == '\n') {
      line++;
      col = 0;
    }
  }
  return 0;
}
