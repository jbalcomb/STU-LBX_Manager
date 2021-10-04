#ifndef ATS_FILE_TYPE_BMP_H
#define ATS_FILE_TYPE_BMP_H


/*
 * There are functional/procedural differences between BMP File Types.
 *
 * For MS BMP/DIB, these are all/only difference between indexed-color and non-indexed-color.
 *
 * For both, image_line_data MUST be 4-byte Aligned.
 *
 */

/*
 * FWIW, MS Paint has the following default values:
 *   Bitmap File Header:
 *     42 4D            0 00   Type
 *     00 00            6 06   Reserved1
 *     00 00            8 08   Reserved2
 *     36 04 00 00     10 0A   OffBits
 *   Bitmap Info Header:
 *                     14 0E   Size
 *                     18 12   Width
 *                     22 16   Height
 *     01 00           26 1A   Planes
 *     08 00           38 1C   BitCount
 *     00 00 00 00     30 1E   Compression
 *                     34 22   SizeImage
 *     00 00 00 00     38 26   XPelsPerMeter
 *     00 00 00 00     42 2A   YPelsPerMeter
 *     00 00 00 00     46 2E   ClrUsed
 *     00 00 00 00     50 32   ClrImportant
 *
 */
/*
 * MS BMP / Windows DIB (Device-Independent Bitmap)
 *
 */

/*
 * PS. Clasically, differentiating between MS Windows 3.x and IBM OS/2 1.x and IBM OS/2 2
 *       was done by checking the size of the Bitmap Info Header -> Size.
 * PPS. That meant you had to read the 4 bytes immediately following the Bitmap File Header,
 *        before you could actually /load/ the Bitmap Info Header. :/
 */

/*
 * https://engineering.purdue.edu/ece264/17au/hw/HW15
 * For purposes of the BMP image format, the pixels start in the lower-left, and progress left-to-right then bottom-to-top.
 *
 */

/*
 * So, Pixels Per Meter, eh? Right...
 * ...
 * DPI?
 * PPI?
 * PPM?
 * ?PPC?
 * ...
 *
 * PelsPerMeter = Pixels Per Meter = PPM  NOTE: "Pels" seems to be some IBM abbreviation oddity from way back when ... (P)ix(els)
 * PPI = Pixels Per Inch
 * DPI = Dots Per Inch
 * ?PPC = Pixels per Centimeter?
 * ...
 * Meters To Inches: pixels_per_inch = (pixels_per_meter / 0.0254)
 * Inches to Meters: pixels_per_meter = (pixels_per_inch * 39.3700787) (~=39.3701 ~=39.37) (1 / 0.0254))
 *
 * Crazy-Pants - 3-D to 2-D PPM calculations...
 * https://stackoverflow.com/questions/50408347/how-to-find-pixel-per-meter
 * ppm = ImageWidth (in pixels) / Field of view (in meters)
 * ...
 *
 * 20210222::
 * GIMP - Create New Image
 * 300 pixels/inch
 * 11.8110 pixels/millimeter
 * 118.110 pixels/centimeter
 * 11811.024 pixels/meter
 * .:. "standard" is 300PPI = 11811.024 PPM (XPelsPerMeters & YPelsPerMeter)
 *
 */

/*
 * 'Image Data'
 * ? ...either indrect - represents an index into a palette - or direct - explicit RGB: {R,G,B} / RGBA: {R,G,B,A} values... ?
 * ...
 * "bits per pixel" vs. color count?
 * ...
 * 2 Color Palette (~= White & Black AKA Monochrome)
 * {{0,0,0}, ..., {1,1,1}}
 * 4 Color Palette
 * {{0,0,0}, ..., {3,3,3}}
 * 8 Color Palette
 * {{0,0,0}, ..., {7,7,7}}
 * 16 Color Palette
 * {{0,0,0}, ..., {15,15,15}}
 * 256 Color Palette
 * {{0,0,0}, ..., {63,63,63}}
 * 65,535 Color Palette
 * {{0,0,0}, ..., {255,255,255}}
 *
*/

/*
 * Strctures:
 *   BITMAPFILEHEADER
 *   BITMAPINFO
 *   BITMAPCOREINFO
 *   BITMAPINFOHEADER
 *   RGBQUAD
 *   BITMAPCOREHEADER
 *   RGBTRIPLE
 *
 * When to use RGB_TRPL vs. RGB_QUAD?
 *
 * Bit /Depth/, Color Depth, Color Count:
 *  1-bit       2-color     (AKA Monochrome)    (2^1 = 2)
 *  4-bit       16-color                        (2^4 = 16)
 *  8-bit       256-color                       (2^8 = 256)
 * 16-bit                   (AKA High-Color)    (2^16 = 65,536)
 * 24-bit                   (AKA True-Color)    (2^24 = 16,777,216)
 * 32-bit                   (?AKA Deep-Color?)  (2^32 = 4,294,967,296)
 */

/* J:\STU-Old\SimTexUni-Dnld_OLD\#Downloads\_files.mpoli.fi\software\PROGRAMM\GENERAL\BMP */
/*
 * The BITMAPFILEHEADER data structure contains information about the type, size, and layout of a device-independent bitmap (DIB) file.
 *
 * A BITMAPINFO or BITMAPCOREINFO data structure immediately follows the BITMAPFILEHEADER structure in the DIB file.
 *
 *
 BITMAP_INFO_HEADER -> biCompression
Specifies the type of compression for a compressed bitmap.
It can be one of the following values:
BI_RGB
    Specifies that the bitmap is not compressed.
BI_RLE8
    Specifies a run-length encoded format for bitmaps with 8 bits per pixel.
    The compression format is a two-byte format consisting of a count byte followed by a byte containing a color index.
    See the following 'Comments' section for more information.
BI_RLE4
    Specifies a run-length encoded format for bitmaps with 4 bits per pixel.
    The compression format is a two-byte format consisting of a count byte followed by two word-length color indexes.
    See the following 'Comments' section for more information.

 */

/* J:\STU-EduMat\_FileFormats_OPC\_BMP\people.math.sc.edu_Burkardt_cpp_src_bmp_io_bmp_io_html\bmp_io.cpp */
/*
//    The BMP format requires that each horizontal line be a multiple of 4 bytes.
//    If the data itself does not have a WIDTH that is a multiple of 4, then
//    the file must be padded with a few extra bytes so that each line has the
//    appropriate length.
*/


#define BMP_MAGIC 0x4D42                    /* "BM" (0x42, 0x4D; "B", "M") */

#define BMP_PALETTE_6BPP_COLOR_COUNT 256
#define BMP_PALETTE_6BPP_BYTES_PER_COLOR 4  /* 8 bits_per_pixel, {x,B,G,R} */

#define BMP_PALETTE_8BPP_COLOR_COUNT 256
#define BMP_PALETTE_8BPP_BYTES_PER_COLOR 4  /* 8 bits_per_pixel, {A,B,G,R} */

/* J:\STU-EduMat\_FileFormats\_FileFormats_OPC\BMP\EasyBMP\EasyBMP_1.06\EasyBMP.h */
#ifndef BMP_XPELS_PER_METER_DEFAULT
/*#define _BMP_XPELSPERMETER_DEFAULT_*/
#define BMP_XPELS_PER_METER_DEFAULT 3780      /* set to a default of 96 dpi */
#endif
#ifndef BMP_YPELS_PER_METER_DEFAULT
/*#define _BMP_YPELSPERMETER_DEFAULT_*/
#define BMP_YPELS_PER_METER_DEFAULT 3780      /* set to a default of 96 dpi */
#endif

typedef unsigned char BMP_BYTE;     /* 8-bit, unsigned integer */
#define BMP_BYTE_LENGTH 1
typedef unsigned short BMP_WORD;    /* 16-bit, unsigned integer */
#define BMP_WORD_LENGTH 2
typedef unsigned int BMP_DWORD;     /* 32-bit, unsigned integer */
#define BMP_DWORD_LENGTH 4

typedef unsigned char BMP_PALETTE;
#define BMP_PALETTE_SIZE 1024

struct s_BMP_RGB_TRPL {
    BMP_BYTE    rgbBlue;                   /* Specifies the intensity of blue in the color. */
    BMP_BYTE    rgbGreen;                  /* Specifies the intensity of green in the color. */
    BMP_BYTE    rgbRed;                    /* Specifies the intensity of red in the color. */
};
typedef struct s_BMP_RGB_TRPL BMP_RGB_TRPL;
#define BMP_RGB_TRPL_LENGTH 3

struct s_BMP_BGRX_QUAD {
    BMP_BYTE    rgbBlue;                    /* Specifies the intensity of blue in the color. */
    BMP_BYTE    rgbGreen;                   /* Specifies the intensity of green in the color. */
    BMP_BYTE    rgbRed;                     /* Specifies the intensity of red in the color. */
    BMP_BYTE    rgbReserved;                /* Is not used and must be set to zero. */
};
typedef struct s_BMP_BGRX_QUAD BMP_BGRX_QUAD;
#define BMP_RGB_QUAD_LENGTH 4

struct s_BMP_BITMAP_FILE_HEADER
{
    BMP_WORD    bfType;             /* Specifies the type of file. It must be BM. */
    BMP_DWORD   bfSize;             /* Specifies the size in DWORDs of the file.  */
    BMP_WORD    bfReserved1;        /* Is reserved and must be set to zero. */
    BMP_WORD    bfReserved2;        /* Is reserved and must be set to zero. */
    BMP_DWORD   bfOffBits;          /* Specifies, in bytes, the offset from the beginning of the file (BOF) to the image data (bitmap) */
};
typedef struct s_BMP_BITMAP_FILE_HEADER BMP_BITMAP_FILE_HEADER;
#define BMP_BITMAP_FILE_HEADER_LENGTH 14

struct s_BMP_BITMAP_INFO_HEADER{
    BMP_DWORD   biSize;              /* Specifies the number of bytes required by the BITMAPINFOHEADER structure. */
    BMP_DWORD   biWidth;             /* Specifies the width of the bitmap in pixels. */
    BMP_DWORD   biHeight;            /* Specifies the height of the bitmap in pixels. */
    BMP_WORD    biPlanes;            /* Specifies the number of planes for the target device and must be set to 1. */
    BMP_WORD    biBitCount;          /* Specifies the number of bits per pixel. This value must be 1, 4, 8, or 24. */
    BMP_DWORD   biCompression;       /* Specifies the type of compression for a compressed bitmap. */
    BMP_DWORD   biSizeImage;         /* Specifies the size in bytes of the image. */
    BMP_DWORD   biXPelsPerMeter;     /* */
    BMP_DWORD   biYPelsPerMeter;     /* */
    BMP_DWORD   biClrUsed;           /* */
    BMP_DWORD   biClrImportant;      /* */
};
typedef struct s_BMP_BITMAP_INFO_HEADER BMP_BITMAP_INFO_HEADER;
#define BMP_BITMAP_INFO_HEADER_LENGTH 40

struct s_BMP_BITMAP_INFO {
    BMP_BITMAP_INFO_HEADER    bmiHeader;    /* Specifies a BITMAPINFOHEADER data structure that contains information about the dimensions and color format of a device-independent bitmap. */
    BMP_BGRX_QUAD             bmiColors[1];  /* Specifies an array of RGBQUAD data structures that define the colors in the bitmap. */
};
typedef struct s_BMP_BITMAP_INFO BMP_BITMAP_INFO;
#define BMP_BITMAP_INFO_LENGTH 1064

struct s_BMP_FILE {
    BMP_BITMAP_FILE_HEADER * bmp_bitmap_file_header;
    BMP_BITMAP_INFO_HEADER * bmp_bitmap_info_header;

};
typedef struct s_BMP_FILE BMP_FILE;
#define BMP_FILE_LENGTH 54


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



#ifdef __cplusplus
}
#endif


#endif  /* ATS_FILE_TYPE_BMP_H */
