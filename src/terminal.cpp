#include "../include/terminal.h"
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <sys/ioctl.h>


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

char get_key(){
    char c;

    ssize_t n = read(STDIN_FILENO, &c, 1);

    if (n == 1)
        return c;

    return '\0';
}

std::pair<int,int> get_terminal_dimension(){
    struct winsize w;
    ioctl(STDOUT_FILENO,TIOCGWINSZ,&w);

    int rows=w.ws_row;
    int cols=w.ws_col;

    return {rows,cols};
}
