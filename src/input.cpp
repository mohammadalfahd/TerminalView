#include "../include/input.h"
#include <termios.h>
#include <unistd.h>
#include <cstdio>

static struct termios original_terminal;

void enable_raw_mode(){

    tcgetattr(STDIN_FILENO,&original_terminal);
    struct termios raw=original_terminal;
    raw.c_lflag&= ~(ICANON | ECHO);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw);

}
void disable_raw_mode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal);
}
#include <unistd.h>

char get_key(){
    char c;

    ssize_t n = read(STDIN_FILENO, &c, 1);

    if (n == 1)
        return c;

    return '\0';
}