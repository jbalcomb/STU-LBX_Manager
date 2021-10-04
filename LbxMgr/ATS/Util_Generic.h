#ifndef UTIL_GENERIC_H
#define UTIL_GENERIC_H


#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* SEE: Borland C++ 3.0: .\BORLANDC\INCLUDE\WINDOWS.H  */
#ifndef BYTE
typedef unsigned char       BYTE;
#endif
#ifndef CHAR
typedef char				CHAR;
#endif
#ifndef SHORT
typedef short				SHORT;
#endif
#ifndef WORD
typedef unsigned short      WORD;
#endif
#ifndef DWORD
/* typedef unsigned long       DWORD; */
typedef unsigned int       DWORD;
#endif

/* SEE: libpng-1.5.4 pngconf.h */
#if defined(INT_MAX) && (INT_MAX > 0x7ffffffeL)
typedef unsigned int ats_uint_32;
typedef int ats_int_32;
#else
typedef unsigned long ats_uint_32;
typedef long ats_int_32;
#endif
typedef unsigned short ats_uint_16;
typedef short ats_int_16;
typedef unsigned char ats_byte;
#ifdef LBX_NO_SIZE_T
typedef unsigned int ats_size_t;
#else
typedef size_t ats_size_t;
#endif
#define ats_sizeof(x) (sizeof (x))


char const hexadecimal_characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
// for (int i = data; i < data_length; ++i)
// {
// 	char const byte = data[i];
//
// 	entry += hex_chars[(byte & 0xF0) >> 4];
// 	entry += hex_chars[(byte & 0x0F) >> 0];
// }

void display_lbx_uint8_array_as_hexadecimal(lbx_uint8 * lbx_uint8_buffer, lbx_uint8 lbx_uint8_buffer_length, lbx_uint8 lbx_uint8_buffer_element_size, lbx_uint8 lbx_uint8_buffer_element_count);
void display_unsigned_char_buffer_as_hexadecimal(unsigned char unsigned_char_buffer[512]);
/* void * cnts_malloc_for_dummies(char * str); */
void * str_malloc_for_dummies(char * str);
unsigned long hash_ascii_value_summation(char * str);

#endif  // UTIL_GENERIC_H
