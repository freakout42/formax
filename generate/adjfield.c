/* adjust field position and length in form from screen map */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NXT getc(stdin); col++

int main(int argc, char *argv[]) {
  int line=1, col=0, act, len;
  int border;
  border = argc==2 && !strcmp(argv[1], "-1");
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
      while(t == '_' || t == '.' || (t >= '0' && t <= '9')) { len = t=='.' ? 1 : len+1; t = NXT; }
      printf("UPDATE fields set line = %d, col = %d, dlen = %d where id = %d;\n", line-1+border, act-1+border, len, atoi(f));
    }
    if (t == '\n') {
      line++;
      col = 0;
    }
  }
  return 0;
}
