#include "lib_lbx_file_type_bmp_nonindexed.h"


/*
 * MS BMP/DIB
 * Non-Indexed = 24-bit, 32-bit, etc. (>=24-bit)
 * if bits_per_pixel >= 24
 * Does NOT contain Color Map / Palette data
 * Bytes Per Pixel >= 3
 */

/*
 * Read image_line_data
 */

/*
 *
 * int image_width;
 * int image_height;
 * image_width = bmp_file->bmp_bitmap_info_header->biWidth;
 * image_height = bmp_file->bmp_bitmap_info_header->biHeight;
 *
 * int bytes_per_pixel;
 * bytes_per_pixel = bmp_file->bmp_bitmap_info_header->biBitCount / 8;
 *
 * unsigned int image_data_buffer_size;
 * image_data_buffer_size = image_width * image_height * bytes_per_pixel;
 *
 * unsigned char * image_data_buffer;
 * image_data_buffer = (unsigned char *)malloc(image_data_buffer_size);
 *
 * int image_line_size;
 * image_line_size = image_width * bytes_per_pixel;
 * ?
 * if ((image_line_size % 4) != 0)
 * {
 *     image_line_size = int((image_line_size + (4 - 1)) / 4) * 4;  / * ...4-byte alignment... * /
 * }
 * ?
 *
 * unsigned char * image_line_data_buffer;
 * image_line_data_buffer = (unsigned char *)malloc(image_line_size);
 *
 */