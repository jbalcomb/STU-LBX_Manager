
#include <stdio.h>  /* fclose(), fopen(), fread(), fwrite(), printf(); FILE */
#include <stdint.h>  /* uint32_t */

#include "ats_endian.h"



/*
** Get a 16-bit word in either big- or little-endian byte order.
*/
WORD ats_GetWordLsb(FILE *fp)
{
    register WORD w;
    w = (WORD)(fgetc(fp) & 0xFF);
    w |= ((WORD)(fgetc(fp) & 0xFF) << 0x08);
    return(w);
}

/*
** Get a 32-bit word in either big- or little-endian byte order.
*/
DWORD ats_GetDwordLsb(FILE *fp)
{
    register DWORD w;
    w |= (DWORD)(fgetc(fp) & 0xFF);
    w |= (((DWORD)(fgetc(fp) & 0xFF)) << 0x08);
    w |= (((DWORD)(fgetc(fp) & 0xFF)) << 0x10);
    w |= (((DWORD)(fgetc(fp) & 0xFF)) << 0x18);
    return(w);
}

uint32_t ats_GetBE32(FILE* fp)
{
    uint32_t result;
    result  = fgetc(fp) << 24;
    result |= fgetc(fp) << 16;
    result |= fgetc(fp) <<  8;
    result |= fgetc(fp);
    return result;
}

uint32_t ats_GetLE32(FILE* fp)
{
    uint32_t result;
    result  = fgetc(fp);
    result |= fgetc(fp) <<  8;
    result |= fgetc(fp) << 16;
    result |= fgetc(fp) << 24;
    return result;
}


/*
** Get a 16-bit word in either big- or little-endian byte order.
*/
/*
WORD GetWordLsb(FILE * file_stream)
{
    register WORD w;
    w = (WORD)(fgetc(file_stream) & 0xFF);
    w |= ((WORD)(fgetc(file_stream) & 0xFF) << 0x08);
    return(w);
}
*/

/*
** Get a 32-bit word in either big- or little-endian byte order.
*/
/*
DWORD GetDwordLsb(FILE * file_stream)
{
    register DWORD w;
    w |= (DWORD)(fgetc(file_stream) & 0xFF);
    w |= (((DWORD)(fgetc(file_stream) & 0xFF)) << 0x08);
    w |= (((DWORD)(fgetc(file_stream) & 0xFF)) << 0x10);
    w |= (((DWORD)(fgetc(file_stream) & 0xFF)) << 0x18);
    return(w);
}
*/
/*
uint16_t lbx_read_2byte_le(FILE * file_stream)
{
    uint16_t result;
    result  = fgetc(file_stream);
    result |= fgetc(file_stream) << 8;
    return result;
}

uint32_t lbx_read_4byte_le(FILE * file_stream)
{
    uint32_t result;
    result  = fgetc(file_stream);
    result |= fgetc(file_stream) << 8;
    result |= fgetc(file_stream) << 16;
    result |= fgetc(file_stream) << 24;
    return result;
}
*/

void read_two_bytes(FILE * fp)
{
    /*
     * https://developer.ibm.com/technologies/systems/articles/au-endianc/
     *
     */
    char c1 = fgetc(fp);
    char c2 = fgetc(fp);

}

void read_four_bytes(FILE * fp)
{
    int fread_result;
    int num;
    char buf[4];

    fread_result = fread(buf, 1, 3, fp);

    if (fread_result == 3)
    {
        num = buf[2] << 8 | buf[1];
        /* num = buf[1] << 8 | buf[2]; */
    }
}
