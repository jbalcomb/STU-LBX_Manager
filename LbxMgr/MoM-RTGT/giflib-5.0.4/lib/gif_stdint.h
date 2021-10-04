#ifndef GIF_STDINT_H
#define GIF_STDINT_H

// Specify integer types with specific sizes
#if defined(_MSC_VER) || defined(SWIG)         // Compiler MS Visual Studio or Swig (no proper POSIX)
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
#else
#include <stdint.h>     // POSIX
#endif

#endif // GIF_STDINT_H
