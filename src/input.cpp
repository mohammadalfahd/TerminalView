#include "input.h"
#include <termios.h>
#include <unistd.h>
#include <cstdio>

static struct termios original_terminal;

void enable_raw_mode(){

    tcgetattr(STDIN_FILENO,&original_terminal);
    struct termios raw=original_terminal;
    raw.c_lflag&= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw);

}
void disable_raw_mode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal);
}
char get_key(){
    return getchar();
}
