#ifndef GIF_UNISTD_H
#define GIF_UNISTD_H

#if defined(_MSC_VER)         // Compiler MS Visual Studio (no proper POSIX)
#else
#include <unistd.h>     // POSIX
#endif

#endif // GIF_UNISTD_H
