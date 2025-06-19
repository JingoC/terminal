#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <fcntl.h>
#include <signal.h>

#include "terminal_config.h"
#include "terminal.h"

#include "test_commands/test_commands.h"

char dbgbuffer[256];

struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void set_raw_mode()
{
    struct termios tattr;

    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(reset_terminal_mode);

    tcgetattr(STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON|ECHO);
    tattr.c_cc[VMIN] = 1;
    tattr.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}

void sigio_handler(int signo)
{
    char c;
    while (read(STDIN_FILENO, &c, 1) > 0) {
        CLI_EnterChar(c);
    }
}

void setup_keyboard_signal()
{
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (flags == -1) perror("fcntl F_GETFL");

    if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK | O_ASYNC) == -1) {
        perror("fcntl F_SETFL");
    }

    if (fcntl(STDIN_FILENO, F_SETOWN, getpid()) == -1) {
        perror("fcntl F_SETOWN");
    }

    signal(SIGIO, sigio_handler);
}

void InitSW()
{
    set_raw_mode();
    setup_keyboard_signal();

    CLI_Init(TDC_Time);	
	TestCmds_Init();
}

int main() {
    InitSW();
    
    while(1) {
        CLI_Execute();
    }
    
    return 0;
}

