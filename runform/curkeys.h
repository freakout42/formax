#ifndef CURSES_H
typedef struct _win_st WINDOW;
#define KEY_ENTER 0527
#define KEY_CANCEL 0543
#define KEY_BACKSPACE 0407
#define KEY_NEXT 0557
#define KEY_BTAB 0541
#define KEY_UP 0403
#define KEY_DOWN 0402
#define KEY_RIGHT 0405
#define KEY_LEFT 0404
#define KEY_PPAGE 0523
#define KEY_NPAGE 0522
#define KEY_LL 0533
#define KEY_END 0550
#define KEY_HOME 0406
#define KEY_IC 0513
#define KEY_DC 0512
#define KEY_F0 0410
#define KEY_F(n) (0410+(n))
#endif
#define KEY_TAB '\t'
#define KEY_ESC '\033'
#define KEY_CTRL(c) (c-'@')
