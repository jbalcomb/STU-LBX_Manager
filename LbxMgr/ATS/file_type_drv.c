/*

http://faydoc.tripod.com/structures/16/1646.htm

https://github.com/microsoft/MS-DOS/blob/master/v2.0/bin/DEVDRIV.DOC

+--------------------------------------+
| DWORD Pointer to next device         |
| (Must be set to -1)                  |
+--------------------------------------+
| WORD Attributes                      |
|  Bit 15 = 1 if char device 0 if blk  |
|  if bit 15 is 1                      |
|      Bit 0 = 1 if Current sti device |
|      Bit 1 = 1 if Current sto output |
|      Bit 2 = 1 if Current NUL device |
|      Bit 3 = 1 if Current CLOCK dev  |
|      Bit 4 = 1 if SPECIAL            |
|  Bit 14 is the IOCTL bit (see below) |
|  Bit 13 is the NON IBM FORMAT bit    |
+--------------------------------------+
| WORD Pointer to Device strategy      |
|      entry point                     |
+--------------------------------------+
| WORD Pointer to Device interrupt     |
|      entry point                     |
+--------------------------------------+
| 8-BYTE character device name field   |
| Character devices set a device name  |
| For block devices the first byte is  |
| The number of units                  |
+--------------------------------------+
*/

#include "file_type_drv.h"
