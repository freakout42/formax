/* builds curkeys.h for using keys without curses.h */
#include <stdio.h>
#include <curses.h>

#define defke3(keyname, keyvalue) printf("#define KEY_%s %s\n", keyname, #keyvalue);
#define defke2(keyname, keyvalue) defke3(keyname, keyvalue)
#define defkey(keyname)           defke2(#keyname, KEY_##keyname)

int main() {
printf("#ifndef CURSES_H\n");
defkey(BACKSPACE)
defkey(NEXT)
defkey(UP)
defkey(DOWN)
defkey(RIGHT)
defkey(LEFT)
defkey(PPAGE)
defkey(NPAGE)
defkey(LL)
defkey(END)
defkey(HOME)
defkey(IC)
defkey(DC)
defkey(F0)
defkey(F(n))
printf("#endif\n");
printf("#define KEY_BS '\\b'\n");
printf("#define KEY_RETURN '\\n'\n");
printf("#define KEY_RRETURN '\\r'\n");
printf("#define KEY_TAB '\\t'\n");
printf("#define KEY_ESC '\\033'\n");
printf("#define KEY_CTRL(c) (c-'@')\n");
printf("#ifndef KEY_END\n");
printf("#define KEY_END KEY_LL\n");
printf("#endif\n");
printf("#ifndef KEY_BTAB\n");
printf("#define KEY_BTAB 0534\n");
printf("#endif\n");
return 0;
}