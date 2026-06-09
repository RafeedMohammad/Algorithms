#include "utf8_console.h"

#ifdef _WIN32
#include <Windows.h>
#include <fcntl.h>
#include <cstdio>
#include <io.h>
#endif

void enable_utf8_console()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}
