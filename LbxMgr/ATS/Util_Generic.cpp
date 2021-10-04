
#include <iostream>     /* std::cout; FILE */
#include <unistd.h> /* getcwd() */
#include <cstring>
#include <sys/stat.h>
#include <limits>  /* std::numeric_limits */
#include <exception>  /* std::exception */
/* std::static_cast */
/* #include <direct.h> */  /* getcwd(), mkdir() */ /* NOTE: There are two direct.h - Digital Mars and Microsoft Windows
/* #include <direct.h>		// chdir(), chdrive(), getcwd(), getdcwd(), getdiskfree(), getdrive(), getdrives(), mkdir(), rmdir() */

// #include <io.h>  /* getcwd() */
// #include <stdint.h>  /* uint16_t, etc. */

#include "Util_Generic.h"


void display_lbx_uint8_array_as_hexadecimal(lbx_uint8 * lbx_uint8_buffer, lbx_uint8 lbx_uint8_buffer_length, lbx_uint8 lbx_uint8_buffer_element_size, lbx_uint8 lbx_uint8_buffer_element_count)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: display_lbx_uint8_array_as_hexadecimal()\n");

    /*
     * lbx_uint8_buffer
     * lbx_uint8_buffer_length
     * lbx_uint8_buffer_element_size
     * lbx_uint8_buffer_element_count
     */

    int itr_lbx_uint8_buffer;
    int itr_lbx_uint8_buffer_max;
    int itr_lbx_uint8_buffer_element_size;

    itr_lbx_uint8_buffer_max = lbx_uint8_buffer_length / lbx_uint8_buffer_element_size;

    if (itr_lbx_uint8_buffer_max != lbx_uint8_buffer_element_count)
    {
        exit(EXIT_FAILURE);
    }

    for (itr_lbx_uint8_buffer = 0; itr_lbx_uint8_buffer < itr_lbx_uint8_buffer_max; itr_lbx_uint8_buffer++)
    {
        for (itr_lbx_uint8_buffer_element_size = 0; itr_lbx_uint8_buffer_element_size < lbx_uint8_buffer_element_size; itr_lbx_uint8_buffer_element_size++)
        {
            std::cout << ar[i] << " ";
        }
    }
    std::cout << std::endl;

    for (itr_lbx_uint8_buffer = 0; itr_lbx_uint8_buffer < itr_lbx_uint8_buffer_max; itr_lbx_uint8_buffer++)
        for (itr_lbx_uint8_buffer_element_size = 0; itr_lbx_uint8_buffer_element_size < lbx_uint8_buffer_element_size; itr_lbx_uint8_buffer_element_size++)
        {
            std::cout << std::hex << std::setfill('0') << std::setw(2) << ar[i] << " ";
        }
    std::cout << std::endl;

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: display_lbx_uint8_array_as_hexadecimal()\n");
}

void display_unsigned_char_buffer_as_hexadecimal(unsigned char unsigned_char_buffer[512])
{

    // convert to WORD from 2 bytes of char buffer

    // Hexadecimal Representation: "%02x", "0x%02x", "0x%02X"
    // ? cast to int ... sign-extended issue ?

    printf("\nunsigned char rawData[512] = {\n");
    for (int itr_unsigned_char_buffer = 0; itr_unsigned_char_buffer < 512; itr_unsigned_char_buffer++)
    {
        printf("0x%02X, ", unsigned_char_buffer[itr_unsigned_char_buffer]);
    }
    printf("}\n");
}

/* void * cnts_malloc_for_dummies(char * str) */
void * str_malloc_for_dummies(char * str)
{
    return malloc( sizeof(char) * ((strlen(str) + 1)) );
}

#define CAPACITY 50000 // Size of the Hash Table

/* https://www.journaldev.com/35238/hash-table-in-c-plus-plus#:~:text=A%20Hash%20Table%20in%20C,value%20at%20the%20appropriate%20location. */
unsigned long hash_ascii_value_summation(char * str)
{
    unsigned long i = 0;

    for (int j=0; str[j]; j++)
    {
        i = i + str[j];
    }

    return i % CAPACITY;
}

/*
 * https://stackoverflow.com/questions/22184403/how-to-cast-the-size-t-to-double-or-int-c
 *
 * https://stackoverflow.com/questions/5129498/how-to-cast-or-convert-an-unsigned-int-to-int-in-c
 * |-> https://stackoverflow.com/a/64842306
 *
 */
int size_t_to_int(size_t number)
{
    int size_t_int;

    if ( number > INT_MAX )
    {
        throw std::overflow_error("size_t number is larger than supported by int, given INT_MAX");
    }

    if ( number > std::numeric_limits<int>::max() )
    {
        /* throw std::exception("Invalid cast."); */
        throw std::exception();
    }

    /* size_t_int = std::static_cast<int>(number); */
    size_t_int = static_cast<int>(number);

    return size_t_int;
}
