#ifndef ATS_ENDIAN_H
#define ATS_ENDIAN_H

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

WORD ats_GetWordLsb(FILE *fp);
DWORD ats_GetDwordLsb(FILE *fp);
uint32_t ats_GetBE32(FILE* fp);
uint32_t ats_GetLE32(FILE* fp);
/*
uint16_t lbx_read_2byte_le(FILE * file_stream);
uint32_t lbx_read_4byte_le(FILE * file_stream);
*/

#endif  /* LBXMGR_ATS_ENDIAN_H */
