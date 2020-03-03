//
// Created by Luoxs on 2018-02-02
// Last modification on 2018-02-02
//
#pragma once

#include <stdio.h>

#ifdef _MSC_VER
#include <windows.h>

#define INNER_CONSOLE_COLOR_DEBUG (FOREGROUND_BLUE | FOREGROUND_RED)
#define INNER_CONSOLE_COLOR_INFO  (FOREGROUND_GREEN)
#define INNER_CONSOLE_COLOR_WARN  (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define INNER_CONSOLE_COLOR_ERROR (FOREGROUND_RED | FOREGROUND_INTENSITY)

#define INNER_CONSOLE_IMPL(COLOR, Fmt, ...) \
    for(;;) {\
        HANDLE h_ = GetStdHandle(STD_OUTPUT_HANDLE);\
        CONSOLE_SCREEN_BUFFER_INFO obi_;\
        GetConsoleScreenBufferInfo(h_, &obi_);\
        WORD bcolor_ = obi_.wAttributes;\
        bcolor_ &= (WORD)(~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY));\
        SetConsoleTextAttribute(h_, COLOR | bcolor_);\
        fprintf(stdout, Fmt, ##__VA_ARGS__);\
        SetConsoleTextAttribute(h_, obi_.wAttributes);\
        break;\
    }

#else

#define INNER_CONSOLE_COLOR_DEBUG "36m"
#define INNER_CONSOLE_COLOR_INFO  "32m"
#define INNER_CONSOLE_COLOR_WARN  "1;33m"
#define INNER_CONSOLE_COLOR_ERROR "1;31m"

#define INNER_CONSOLE_IMPL(COLOR, Fmt, ...) fprintf(stdout, "\033[" COLOR Fmt "\033[0m", ##__VA_ARGS__)

#endif

#ifdef _DEBUG

#define CONSOLE(Fmt, ...)           fprintf(stdout, Fmt, ##__VA_ARGS__)
#define CONSOLE_DEBUG(Fmt, ...)     INNER_CONSOLE_IMPL(INNER_CONSOLE_COLOR_DEBUG, Fmt, ##__VA_ARGS__)
#define CONSOLE_INFO(Fmt, ...)      INNER_CONSOLE_IMPL(INNER_CONSOLE_COLOR_INFO, Fmt, ##__VA_ARGS__)
#define CONSOLE_WARN(Fmt, ...)      INNER_CONSOLE_IMPL(INNER_CONSOLE_COLOR_WARN, Fmt, ##__VA_ARGS__)
#define CONSOLE_ERROR(Fmt, ...)     INNER_CONSOLE_IMPL(INNER_CONSOLE_COLOR_ERROR, Fmt, ##__VA_ARGS__)

#else

#define CONSOLE(Fmt, ...)      
#define CONSOLE_DEBUG(Fmt, ...)
#define CONSOLE_INFO(Fmt, ...) 
#define CONSOLE_WARN(Fmt, ...) 
#define CONSOLE_ERROR(Fmt, ...)

#endif
