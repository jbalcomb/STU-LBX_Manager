# I/O with Endianness

Little Endian (Least Significant Byte) (LE (LSB))  
Big Endian (Most Significant Byte) (BE (MSB))  

NOTE: Pneumonic: "L" = Little Endian & Least Significant Byte  

NOTE: "Byte Order" (Also, "Bit Order")  


***

Binary Data Format:
Written on BE system, transferred, and read on LE system... FAILURE!

Elsewhere, Network Byte Order vs. Host Byte Order
"The Internet" ~= Big-Endian.

***

1 Byte = 8 Bits  

0xFEAD OR 0xADFE

***

Nifty Nonsense & Trickery:

Read/Write Multi-Byte, as 2, 4, 8, ..., n byte sequences...


The BMP File Format, Part 1
The BMP File Format, Part 2
Author: David Charlap
Dr. Dobb's Journal (DDJ) - 1995-03, 1995-04
http://archive.retro.co.za/CDROMs/DrDobbs/CD%20Release%2012/articles/1995/9503/9503e/9503e.htm
http://archive.retro.co.za/CDROMs/DrDobbs/CD%20Release%2012/articles/1995/9504/9504c/9504c.htm
BMP.ZIP
|-> ENDIAN.H
|-> ENDIAN.C

```C
int readINT8little(FILE *f, INT8 *i)
{
    int rc;
    rc = fgetc(f);
    if (rc == EOF)
	return rc;
    *i = (rc & 0xff);
    return 0;
}
```
```C
int readINT16little(FILE *f, INT16 *i)
{
    int rc;
    INT16 temp = 0;
    temp = (fgetc(f) & 0xff);
    rc = fgetc(f);
    if (rc == EOF)
	return rc;
    temp |= ((rc & 0xff) << 8);
    *i = temp;
    return 0;
}
```
```C
int readINT32little(FILE *f, INT32 *i)
{
    int rc;
    INT32 temp = 0;
    temp = ((long)fgetc(f) & 0xff);
    temp |= (((long)fgetc(f) & 0xff) << 8);
    temp |= (((long)fgetc(f) & 0xff) << 16);
    rc = fgetc(f);
    if (rc == EOF)
	return rc;
    temp |= (((long)rc & 0xff) << 24);
    *i = temp;
    return 0;
}
```
