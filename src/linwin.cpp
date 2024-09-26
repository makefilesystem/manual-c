#include "linwin.hpp"

#ifdef _LINWIN_LIN
# include <unistd.h>
#else
# include <conio.h>
#endif

#include <cstdio>
#include <cstdlib>


namespace linwin {
    #ifdef _LINWIN_LIN
    bool _isStdinBufDisabled = false;
    #endif
 

    void enable_buf() {
        #ifdef _LINWIN_LIN
        if (!_isStdinBufDisabled) return;
        termios _t;
        tcgetattr(STDIN_FILENO, &_t);
        _t.c_lflag |= ICANON;
        tcsetattr(STDIN_FILENO, TCSANOW, &_t);
        _isStdinBufDisabled = false;
        #endif
    }

    void disable_buf() {
        #ifdef _LINWIN_LIN
        if (_isStdinBufDisabled) return;
        termios _t;
        tcgetattr(STDIN_FILENO, &_t);
        _t.c_lflag &= ~(ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &_t);
        _isStdinBufDisabled = true;
        #endif
    }

    void set_color(const char *ansi_entry) {
        std::fputs(ansi_entry, stdout);
    }

    void reset_color() {
        set_color(_ANSI_RESET);
    }

    void clear() {
        #ifdef _LINWIN_LIN
        std::system("clear");
        #else
        std::system("cls");
        #endif
    }

    int getch() {
        int ch = 0;
        #ifdef _LINWIN_LIN
        bool once = false;
        if (!_isStdinBufDisabled) {
            disable_buf();
            once = true;
        }
        ch = std::getchar();
        if (once) enable_buf();
        #else
        ch = _getch();
        #endif
        return ch;
    }
};

