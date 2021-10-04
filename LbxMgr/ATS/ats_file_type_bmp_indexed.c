#include <malloc.h>         /* malloc(), realloc(); */
#include <stdlib.h>         /* exit(), itoa(), splitpath(); EXIT_FAILURE, EXIT_SUCCESS; */
#include <stdio.h>  /* fclose(), fopen(), fread(), fwrite(), printf(); FILE */

#include "lib_lbx_file_type_bmp_indexed.h"
#include "lib_lbx.h"
#include "lib_lbx_file_type_bmp.h"
#include "lib_lbx_record_palette.h"


/*
 * MS BMP/DIB
 * Indexed = 1-bit, 2-bit, 4-bit, 8-bit, 16-bit, etc. (<24-bit)
 * if bits_per_pixel < 24
 * Does contain Color Map / Palette data
 * Bytes Per Pixel <= 8
 */

/*
 * Convert MS BMP/DIB Palette from {xBGR} to {RGB,RGBx,RGBA}
 * Convert {RGB,RGBx,RGBA} to MS BMP/DIB Palette {xBGR}
 */

void read_palette(void)
{
    if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: BEGIN: bmp_palette[ ] = lbx_palette[ ]\n");


    /*LBX_PALETTE * lbx_palette;*/
    lbx_uint8 * lbx_palette;
    /* lbx_palette = (LBX_PALETTE*)malloc(LBX_PALETTE_SIZE); */
    lbx_palette = lbx_load_palette_by_number(2);
    if (LBX_DEBUG_STRUGGLE_MODE) printf("DEBUG: lbx_palette: %x\n", lbx_palette);


    BMP_FILE * bmp_file;
    if (LBX_DEBUG_STRUGGLE_MODE) printf("DEBUG: BEGIN: bmp_file = (BMP_FILE *)malloc(sizeof(BMP_FILE));\n");
    /* TODO(JWB): research this (sizeof *bmp_file) approach more; figure out how to apply it to structure member structures */
    bmp_file = (BMP_FILE *)malloc(sizeof *bmp_file);
    if (bmp_file == NULL)
    {
        printf("FAILURE: bmp_file = (BMP_FILE *)malloc(sizeof *bmp_file);\n", sizeof *bmp_file);
        exit(EXIT_FAILURE);
    }
    if (LBX_DEBUG_STRUGGLE_MODE) printf("DEBUG: END: bmp_file = (BMP_FILE *)malloc(sizeof(BMP_FILE));\n");



    if (LBX_DEBUG_STRUGGLE_MODE) printf("DEBUG: BEGIN: bmp_file->bmp_bitmap_file_header = (BMP_BITMAP_FILE_HEADER *)malloc(sizeof(BMP_BITMAP_FILE_HEADER));\n");
    bmp_file->bmp_bitmap_file_header = (BMP_BITMAP_FILE_HEADER *)malloc(sizeof(BMP_BITMAP_FILE_HEADER));
    if (bmp_file == NULL)
    {
        printf("bmp_file->bmp_bitmap_file_header = (BMP_BITMAP_FILE_HEADER *)malloc(sizeof(BMP_BITMAP_FILE_HEADER));\n", sizeof(BMP_BITMAP_FILE_HEADER));
        exit(EXIT_FAILURE);
    }
    if (LBX_DEBUG_STRUGGLE_MODE) printf("DEBUG: END: bmp_file->bmp_bitmap_file_header = (BMP_BITMAP_FILE_HEADER *)malloc(sizeof(BMP_BITMAP_FILE_HEADER));\n");



    if (LBX_DEBUG_STRUGGLE_MODE) printf("DEBUG: BEGIN: bmp_file->bmp_bitmap_info_header = (BMP_BITMAP_INFO_HEADER *)malloc(sizeof(BMP_BITMAP_FILE_HEADER));\n");
    bmp_file->bmp_bitmap_info_header = (BMP_BITMAP_INFO_HEADER *)malloc(sizeof(BMP_BITMAP_INFO_HEADER));
    if (bmp_file == NULL)
    {
        printf("FAILURE: bmp_file->bmp_bitmap_info_header = (BMP_BITMAP_INFO_HEADER *)malloc(sizeof(BMP_BITMAP_FILE_HEADER));\n", sizeof(BMP_BITMAP_FILE_HEADER));
        exit(EXIT_FAILURE);
    }
    if (LBX_DEBUG_STRUGGLE_MODE) printf("DEBUG: END: bmp_file->bmp_bitmap_info_header = (BMP_BITMAP_INFO_HEADER *)malloc(sizeof(BMP_BITMAP_FILE_HEADER));\n");

    BMP_PALETTE * bmp_palette;
    if (LBX_DEBUG_STRUGGLE_MODE) printf("DEBUG: BEGIN: bmp_palette = (BMP_PALETTE *)malloc(BMP_PALETTE_SIZE);\n");
    bmp_palette = (BMP_PALETTE *)malloc(BMP_PALETTE_SIZE);
    if (bmp_file == NULL)
    {
        printf("FAILURE: bmp_palette = (BMP_PALETTE *)malloc(%u);\n", BMP_PALETTE_SIZE);
        exit(EXIT_FAILURE);
    }
    if (LBX_DEBUG_STRUGGLE_MODE) printf("DEBUG: END: bmp_palette = (BMP_PALETTE *)malloc(BMP_PALETTE_SIZE);\n");


    int itr_bmp_bgrx_quad;
    for (itr_bmp_bgrx_quad = 0; itr_bmp_bgrx_quad < 256; itr_bmp_bgrx_quad++)
    {
        bmp_palette[((itr_bmp_bgrx_quad * 4) + 0)] = 0x00;
        bmp_palette[((itr_bmp_bgrx_quad * 4) + 1)] = lbx_palette[((itr_bmp_bgrx_quad * 4) + (-itr_bmp_bgrx_quad + 2))];
        bmp_palette[((itr_bmp_bgrx_quad * 4) + 2)] = lbx_palette[((itr_bmp_bgrx_quad * 4) + (-itr_bmp_bgrx_quad + 1))];
        bmp_palette[((itr_bmp_bgrx_quad * 4) + 3)] = lbx_palette[((itr_bmp_bgrx_quad * 4) + (-itr_bmp_bgrx_quad + 0))];
        /*
        if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: bmp_palette[%u] = %u\n", ((itr_bmp_bgrx_quad * 4) + 0), 0x00);
        if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: bmp_palette[%u] = lbx_palette[%u]\n", ((itr_bmp_bgrx_quad * 4) + 1), ((itr_bmp_bgrx_quad * 4) + (-itr_bmp_bgrx_quad + 2)));
        if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: bmp_palette[%u] = lbx_palette[%u]\n", ((itr_bmp_bgrx_quad * 4) + 2), ((itr_bmp_bgrx_quad * 4) + (-itr_bmp_bgrx_quad + 1)));
        if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: bmp_palette[%u] = lbx_palette[%u]\n", ((itr_bmp_bgrx_quad * 4) + 3), ((itr_bmp_bgrx_quad * 4) + (-itr_bmp_bgrx_quad + 0)));
         */
    }

    if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: END: bmp_palette[ ] = lbx_palette[ ]\n");
}