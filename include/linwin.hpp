#pragma once

#if defined(__linux__) || defined(__linux)
# define _LINWIN_LIN
# include <termios.h>
#elif defined(_WIN32) || defined(_WIN64)
# define _LINWIN_WIN
# include <windows.h>
#else
# error "Unsupported OS"
#endif


#define _ANSI_RESET     "\033[0m"
#define _ANSI_ESC       "\033"
#define ANSI_RED        1
#define ANSI_GREEN      2
#define ANSI_YELLOW     3
#define ANSI_BLUE       4
#define ANSI_MAGENTA    5
#define ANSI_CYAN       6
#define ANSI_WHITE      7
#define ANSI_FG         3
#define ANSI_BG         4
#define ANSI(ansi_def_macro0, ansi_def_macro1) \
    "\33[" __LWCSTR(ansi_def_macro0) __LWCSTR(ansi_def_macro1) "m"


#define __LWCSTR_(x) #x
#define __LWCSTR(x) __LWCSTR_(x)


namespace linwin {
    void enable_buf();
    void disable_buf();
    void set_color(const char *ansi_entry);
    void reset_color();
    void clear();
    int getch();
};

