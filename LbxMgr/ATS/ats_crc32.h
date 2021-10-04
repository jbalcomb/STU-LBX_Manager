#ifndef ATS_CRC32_H
#define ATS_CRC32_H

#include <stdio.h>          /* fclose(), fopen(), fread(), fwrite(), printf(); FILE */
#include <stdlib.h>         /* itoa(), splitpath(); EXIT_FAILURE, EXIT_SUCCESS; */

#ifdef __cplusplus
extern "C" {
#endif

int ats_compute_crc32_char_buffer(char * char_buffer_in, size_t char_buffer_length, unsigned long int * crc32_out);
/* static int Crc32_ComputeFile(FILE * file, unsigned long * outCrc32); */
/* int Crc32_ComputeFile(FILE * file, unsigned long * outCrc32); */
/* int Crc32_ComputeFile(FILE * file, unsigned long int * outCrc32); */
int ats_compute_crc32_file(FILE * stream_file_in, unsigned long int * crc32_out);
/*static unsigned long ats_compute_crc32(unsigned long crc32_in, const void * buf, size_t bufLen);*/
unsigned long ats_compute_crc32(unsigned long crc32_in, const void * buf, size_t bufLen);

#ifdef __cplusplus
}
#endif

#endif  /* ATS_CRC32_H */
