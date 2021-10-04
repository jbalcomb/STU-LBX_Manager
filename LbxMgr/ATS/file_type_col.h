/*
 * .COL
 *
 * (Autodesk) Animator (Pro) Color Palette File
 *
 * Two Versions:
 * ~= Animator Pro
 * ~= Animator (original)
 *
 * v1
 * no header
 * 768 bytes
 * 256 * 3
 * RGB, 1 byte each, range 0-255
 *
 * v2
 * 8 byte header {File Size, ID, Version}
 * Palette Entry Count = ((File Size - Header Size) / RGB) ~= 768 + 8 = 776 - 8 / = 256
 *
 */

/*
http://www.fileformat.info/format/animator-col/corion.htm

A COL file stores the rgb values for entries in the color palette. Both
Animator Pro and the original Animator produce COL files, but the formats
are different. To process a COL file for input, check the file size. If it is
exactly 768 bytes, the file is an original Animator COL file. If the file is
any other size, it is an Animator Pro COL file - which makes identification
almost impossible.

Animator Pro COL Files do have a 8-byte header :

OFFSET              Count TYPE   Description
0000h                   1 dword  File size, including this header
0004h                   1 word   ID=0B123h
0006h                   1 word   Version, currently 0

Following the file header are palette entries in rgbrgb... order. Each of
the r, g, and b components is a single byte in the range of 0-255. Generally,
there will be data for 256 palette entries, but this cannot be assumed.
The actual number of palette entries is ((size-8)/3); if this value is not
an even multiple of three, the file is corrupted.

Original Animator COL Files

A COL file created by the original Animator is exactly 768 bytes
long. There is no file header or other control information in
the file.

EXTENSION:COL
OCCURENCES:PC
PROGRAMS:Autodesk Animator, Autodesk Animator Pro
SEE ALSO:FLIc,FLT
*/

#ifndef FILE_TYPE_COL_H
#define FILE_TYPE_COL_H



#endif  /* FILE_TYPE_COL_H */
