// #include <stdio.h>          /* fclose(), fopen(), fread(), fwrite(), printf(); FILE */
#include <cstdio>          /* fclose(), fopen(), fread(), fwrite(), printf(); FILE */
// #include <string.h>         /* memcpy(), strcat(), strcpy(), strncpy(), strlen(); */
#include <cstring>
// #include <malloc.h>         /* malloc(), realloc(); */
#include <cstdlib>              // malloc()

#include <iomanip>
#include <iostream>
#include <fstream>


#include "LbxMgrExport.h"
#include "LbxMgr.h"
#include "LbxMgrPathsNames.h"  // create_export_directory_path();
#include "lib_lbx.h"
//#include "lib_lbx_record_image.h"
#include <lib_lbx_record_display.h>
//#include <lib_lbx_record_palette.h>
//#include <lib_lbx_unbrun.h>
#include "ATS/ats_filesystem.h"
#include "ATS/Util_Generic.h"
//#include "ATS/ats_file_type_bmp.h"


void lbxmgr_export_records_to_bin(LBX_DATA * lbx)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbx_export_entries_to_bin()\n");

    create_export_directory_path();
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: lbx->file->file_name_base: %s\n", lbx->file->file_name_base);
    create_export_directory_path_bin(lbx->file->file_name_base);

    int record_size;
    FILE * ptr_stream_file_out;
    char * export_file_path;
    int itr_record_bytes;
    for (itr_record_bytes = 0; itr_record_bytes < lbx->header->entry_count; itr_record_bytes++)
    {
        record_size = lbx->record->entry[itr_record_bytes].size;

        // ats_build_file_path_and_name(&export_file_path, lbx->file->export_directory_path_bin, lbx->meta->records->entry[itr_record_bytes].record_file_name_base, export_file_extension_bin);
        ats_build_file_path_and_name(&export_file_path, lbxmgr_data->export_directory_path_bin, lbx->record->entry[itr_record_bytes].record_file_name_base, export_file_extension_bin);

        if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: export_file_path: %s\n", export_file_path);

        ptr_stream_file_out = fopen(export_file_path, "wb");

        if (ptr_stream_file_out == nullptr) {
            printf("FAILURE: could not open file\n");
            printf("DEBUG: errno: %d strerror: %s\n", errno, strerror(errno));
        }

        fseek(lbx->file->file_stream, lbx->header->offset_table->entry[itr_record_bytes].begin, SEEK_SET);

        char * lbx_read_buffer;
        lbx_read_buffer = (char *)malloc(sizeof(char) * (record_size + 1));
        fread(lbx_read_buffer, record_size, 1, lbx->file->file_stream);
        fwrite(lbx_read_buffer, record_size, 1, ptr_stream_file_out);
        fclose(ptr_stream_file_out);
    }

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbx_export_entries_to_bin()\n");
}

// 2021-10-04 10:54
// Commenting out everything that should have been in Graphics/FLIC/BMP/Palette/Font
//int lbxmgr_export_records_to_bmp(LBX_DATA * lbx)
//{
//    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_export_records_to_bmp()\n");
//
//    create_export_directory_path();
//
//    create_export_directory_path_bmp(lbx->file->file_name_base);
//
////    /*LBX_PALETTE * lbx_palette;*/
////    lbx_uint8 * lbx_palette;
////    /* lbx_palette = (LBX_PALETTE*)malloc(LBX_PALETTE_SIZE); */
////    lbx_palette = lbx_load_palette_by_number(2);
////    if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: lbx_palette: %x\n", lbx_palette);
//
//    unsigned char * lbx_palette;
//    lbx_palette = liblbx_load_palette(g_palette_file_path, EMPERATO);
//
//
//
//    char * export_file_path;
//    int itr_entry_index;
//
//    for (itr_entry_index = 0; itr_entry_index < lbx->header->entry_count; itr_entry_index++)
//    {
//        printf("%s (%d of %d) [%s]\n", lbx->file->file_name, itr_entry_index + 1, lbx->header->entry_count, lbx->header->string_table->entry[itr_entry_index].name);
//        // display_lbx_image_header(lbx, itr_entry_index);
//        // display_lbx_image_custom_palette_header(lbx, itr_entry_index);
//
//        // ats_build_file_path_and_name(&export_file_path, lbx->file->export_directory_path_bin, lbx->record->entry[itr_entry_index].record_file_name_base, export_file_extension_bin);
//        ats_build_file_path_and_name(&export_file_path, lbxmgr_data->export_directory_path_bmp, lbx->record->entry[itr_entry_index].record_file_name_base, export_file_extension_bmp);
//
//        if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: export_file_path: %s\n", export_file_path);
//
//        FILE * ptr_stream_file_out;
//        ptr_stream_file_out = fopen(export_file_path, "wb");
//
//        if (ptr_stream_file_out == nullptr) {
//            printf("FAILURE: could not open file\n");
//            printf("DEBUG: errno: %d strerror: %s\n", errno, strerror(errno));
//        }
//
//        /*
//        char l_LBX_Record_Type_Description[13][34] = {
//                "EMPTY",
//                "Array",
//                "XMIDI",
//                "VOC",
//                "Sound",
//                "Palette",
//                "Image",
//                "Font",
//                "AIL Driver",
//                "DIGPAK Driver",
//                "Custom",
//                "UNKNOWN",
//                "Animation"
//        };
//        if (LBXMGR_DEBUG_VERBOSE_MODE) printf("DEBUG: Record Type: (%d) %s\n", lbx->record->entry[itr_entry_index].type, l_LBX_Record_Type_Description[lbx->record->entry[itr_entry_index].type]);
//        */
//        if (LBXMGR_DEBUG_VERBOSE_MODE) printf("DEBUG: Record Type: (%d) %s\n", lbx->record->entry[itr_entry_index].type, LBX_Record_Type_Description[lbx->record->entry[itr_entry_index].type]);
//
//        // if (lbx->record->entry[itr_entry_index].type == LBX_RECORD_TYPE_IMAGE)
//        if (lbx->record->entry[itr_entry_index].type == LBX_RECORD_TYPE_FLIC)
//        {
//            fseek(lbx->file->file_stream, lbx->header->offset_table->entry[itr_entry_index].begin, SEEK_SET);
//
//            char * image_record_data;
//            image_record_data = (char *)malloc(sizeof(char) * (lbx->record->entry[itr_entry_index].size + 1));
//            fread(image_record_data, lbx->record->entry[itr_entry_index].size, 1, lbx->file->file_stream);
//
//            /* TODO(JWB): maybe, figure out how to just have pointers to image_record_data for the key locations - header and data starting positions */
//
//
//
//
//            // void lbx_load_image_record(const lbx_uint8 * dst, lbx_uint8 * src)
//            LBX_FLIC_RECORD_HEADER * lbx_image_record;
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: BEGIN: lbx_image_record = (LBX_IMAGE_RECORD *)malloc(sizeof *lbx_image_record);\n");
//            lbx_image_record = (LBX_FLIC_RECORD_HEADER *)malloc(sizeof *lbx_image_record);
//            if (lbx_image_record == nullptr)
//            {
//                printf("FAILURE: lbx_image_record = (LBX_IMAGE_RECORD *)malloc(%u);\n", sizeof *lbx_image_record);
//                exit(EXIT_FAILURE);
//            }
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: END: lbx_image_record = (LBX_IMAGE_RECORD *)malloc(sizeof *lbx_image_record);\n");
//
//            lbx_load_image_record(lbx_image_record, image_record_data);
//
//
//
//
//
//            int bmp_image_size;
//            bmp_image_size = lbx_image_record->lbx_flic_info_header->frame_width * lbx_image_record->lbx_flic_info_header->frame_height;
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: bmp_image_size: %d\n", bmp_image_size);
//
//            int bmp_color_data_length;
//            bmp_color_data_length = BMP_PALETTE_6BPP_COLOR_COUNT * BMP_PALETTE_6BPP_BYTES_PER_COLOR;  /* 256 colors */
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: bmp_color_data_length: %d\n", bmp_color_data_length);
//
//            int bmp_header_length_total;
//            bmp_header_length_total = BMP_BITMAP_FILE_HEADER_LENGTH + BMP_BITMAP_INFO_HEADER_LENGTH;
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: bmp_header_length_total: %d\n", bmp_header_length_total);
//
//            int bmp_file_size;
//            bmp_file_size = bmp_image_size + bmp_color_data_length + bmp_header_length_total; /* BMP File Size = Pixel image_data_buffer(Width * Height) + Color image_data_buffer(RGB * 4) + Header(14 + 40)*/
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: bmp_file_size: %d\n", bmp_file_size);
//
//
//
//            BMP_FILE * bmp_file;
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: BEGIN: bmp_file = (BMP_FILE *)malloc(sizeof(BMP_FILE));\n");
//            /* TODO(JWB): research this (sizeof *bmp_file) approach more; figure out how to apply it to structure member structures */
//            bmp_file = (BMP_FILE *)malloc(sizeof *bmp_file);
//            if (bmp_file == nullptr)
//            {
//                printf("FAILURE: bmp_file = (BMP_FILE *)malloc(sizeof *bmp_file);\n", sizeof *bmp_file);
//                exit(EXIT_FAILURE);
//            }
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: END: bmp_file = (BMP_FILE *)malloc(sizeof(BMP_FILE));\n");
//
//
//
//            BMP_PALETTE * bmp_palette_bytes_buffer;
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: BEGIN: bmp_palette_bytes_buffer = (BMP_PALETTE *)malloc(BMP_PALETTE_SIZE);\n");
//            bmp_palette_bytes_buffer = (BMP_PALETTE *)malloc(BMP_PALETTE_SIZE);
//            if (bmp_file == nullptr)
//            {
//                printf("FAILURE: bmp_palette_bytes_buffer = (BMP_PALETTE *)malloc(%u);\n", BMP_PALETTE_SIZE);
//                exit(EXIT_FAILURE);
//            }
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: END: bmp_palette_bytes_buffer = (BMP_PALETTE *)malloc(BMP_PALETTE_SIZE);\n");
//
//
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: BEGIN: bmp_palette_bytes_buffer[ ] = lbx_palette[ ]\n");
//            int itr_bmp_bgrx_quad;
//            for (itr_bmp_bgrx_quad = 0; itr_bmp_bgrx_quad < 256; itr_bmp_bgrx_quad++)
//            {
//                bmp_palette_bytes_buffer[((itr_bmp_bgrx_quad * 4) + 0)] = 0x00;
//                bmp_palette_bytes_buffer[((itr_bmp_bgrx_quad * 4) + 1)] = lbx_palette[((itr_bmp_bgrx_quad * 4) + (-itr_bmp_bgrx_quad + 2))];
//                bmp_palette_bytes_buffer[((itr_bmp_bgrx_quad * 4) + 2)] = lbx_palette[((itr_bmp_bgrx_quad * 4) + (-itr_bmp_bgrx_quad + 1))];
//                bmp_palette_bytes_buffer[((itr_bmp_bgrx_quad * 4) + 3)] = lbx_palette[((itr_bmp_bgrx_quad * 4) + (-itr_bmp_bgrx_quad + 0))];
//                /*
//                if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: bmp_palette_bytes_buffer[%u] = %u\n", ((itr_bmp_bgrx_quad * 4) + 0), 0x00);
//                if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: bmp_palette_bytes_buffer[%u] = lbx_palette[%u]\n", ((itr_bmp_bgrx_quad * 4) + 1), ((itr_bmp_bgrx_quad * 4) + (-itr_bmp_bgrx_quad + 2)));
//                if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: bmp_palette_bytes_buffer[%u] = lbx_palette[%u]\n", ((itr_bmp_bgrx_quad * 4) + 2), ((itr_bmp_bgrx_quad * 4) + (-itr_bmp_bgrx_quad + 1)));
//                if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: bmp_palette_bytes_buffer[%u] = lbx_palette[%u]\n", ((itr_bmp_bgrx_quad * 4) + 3), ((itr_bmp_bgrx_quad * 4) + (-itr_bmp_bgrx_quad + 0)));
//                 */
//            }
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: END: bmp_palette_bytes_buffer[ ] = lbx_palette[ ]\n");
//
//
//
//            char * bmp_image_bytes_buffer;
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: BEGIN: bmp_image_bytes_buffer = (char *)malloc(bmp_image_size);\n");
//            bmp_image_bytes_buffer = (char *)malloc(bmp_image_size);
//            if (bmp_file == nullptr)
//            {
//                printf("FAILURE: bmp_image_bytes_buffer = (char *)malloc(%u);\n", bmp_image_size);
//                exit(EXIT_FAILURE);
//            }
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: END: bmp_image_bytes_buffer = (char *)malloc(bmp_image_size);\n");
//
//
//
//            /* ******************* *
//             *                     *
//             *   END: MS BMP/DIB   *
//             *                     *
//             * ******************* */
//
//
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: BEGIN: fwrite(&(bmp_file->bmp_bitmap_file_header->), sizeof(bmp_file->bmp_bitmap_file_header->), 1, ptr_stream_file_out);\n");
//
//            fwrite(&(bmp_file->bmp_bitmap_file_header->bfType), sizeof(bmp_file->bmp_bitmap_file_header->bfType), 1, ptr_stream_file_out);
//            fwrite(&(bmp_file->bmp_bitmap_file_header->bfSize), sizeof(bmp_file->bmp_bitmap_file_header->bfSize), 1, ptr_stream_file_out);
//            fwrite(&(bmp_file->bmp_bitmap_file_header->bfReserved1), sizeof(bmp_file->bmp_bitmap_file_header->bfReserved1), 1, ptr_stream_file_out);
//            fwrite(&(bmp_file->bmp_bitmap_file_header->bfReserved2), sizeof(bmp_file->bmp_bitmap_file_header->bfReserved2), 1, ptr_stream_file_out);
//            fwrite(&(bmp_file->bmp_bitmap_file_header->bfOffBits), sizeof(bmp_file->bmp_bitmap_file_header->bfOffBits), 1, ptr_stream_file_out);
//
//            fwrite(&(bmp_file->bmp_bitmap_info_header->biSize), sizeof(bmp_file->bmp_bitmap_info_header->biSize), 1, ptr_stream_file_out);
//            fwrite(&(bmp_file->bmp_bitmap_info_header->biWidth), sizeof(bmp_file->bmp_bitmap_info_header->biWidth), 1, ptr_stream_file_out);
//            fwrite(&(bmp_file->bmp_bitmap_info_header->biHeight), sizeof(bmp_file->bmp_bitmap_info_header->biHeight), 1, ptr_stream_file_out);
//            fwrite(&(bmp_file->bmp_bitmap_info_header->biPlanes), sizeof(bmp_file->bmp_bitmap_info_header->biPlanes), 1, ptr_stream_file_out);
//            fwrite(&(bmp_file->bmp_bitmap_info_header->biBitCount), sizeof(bmp_file->bmp_bitmap_info_header->biBitCount), 1, ptr_stream_file_out);
//            fwrite(&(bmp_file->bmp_bitmap_info_header->biCompression), sizeof(bmp_file->bmp_bitmap_info_header->biCompression), 1, ptr_stream_file_out);
//            fwrite(&(bmp_file->bmp_bitmap_info_header->biSizeImage), sizeof(bmp_file->bmp_bitmap_info_header->biSizeImage), 1, ptr_stream_file_out);
//            fwrite(&(bmp_file->bmp_bitmap_info_header->biXPelsPerMeter), sizeof(bmp_file->bmp_bitmap_info_header->biXPelsPerMeter), 1, ptr_stream_file_out);
//            fwrite(&(bmp_file->bmp_bitmap_info_header->biYPelsPerMeter), sizeof(bmp_file->bmp_bitmap_info_header->biYPelsPerMeter), 1, ptr_stream_file_out);
//            fwrite(&(bmp_file->bmp_bitmap_info_header->biClrUsed), sizeof(bmp_file->bmp_bitmap_info_header->biClrUsed), 1, ptr_stream_file_out);
//            fwrite(&(bmp_file->bmp_bitmap_info_header->biClrImportant), sizeof(bmp_file->bmp_bitmap_info_header->biClrImportant), 1, ptr_stream_file_out);
//
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: END: fwrite(&(bmp_file->bmp_bitmap_file_header->), sizeof(bmp_file->bmp_bitmap_file_header->), 1, ptr_stream_file_out);\n");
//
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: BEGIN: fwrite(bmp_palette_bytes_buffer, sizeof(BMP_PALETTE), BMP_PALETTE_SIZE, ptr_stream_file_out);\n");
//            fwrite(bmp_palette_bytes_buffer, sizeof(BMP_PALETTE), BMP_PALETTE_SIZE, ptr_stream_file_out);
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: END: fwrite(bmp_palette_bytes_buffer, sizeof(BMP_PALETTE), BMP_PALETTE_SIZE, ptr_stream_file_out);\n");
//
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: BEGIN: fwrite(bmp_image_bytes_buffer, sizeof(lbx_uint8), bmp_image_size, ptr_stream_file_out);\n");
//            fwrite(bmp_image_bytes_buffer, sizeof(char), bmp_image_size, ptr_stream_file_out);
//            if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: END: fwrite(bmp_image_bytes_buffer, sizeof(lbx_uint8), bmp_image_size, ptr_stream_file_out);\n");
//
//
//
//            /* J:\STU-EduMat\_FileFormats_OPC\pilrc-master\bitmap.c */
//            /*
//              // allocate memory for image data (word aligned)
//              cbRow = ((dx * cbitsPel + 15) & ~15) >> 3;
//              rcbmp->cbDst = (cbRow * dy) + colorDat;
//              rcbmp->pbBits = (unsigned char *)malloc(rcbmp->cbDst);
//              memset(rcbmp->pbBits, 0, rcbmp->cbDst);
//              ...
//              ...
//              ...
//              // do we need to consider transparency?
//              switch (transparencyData[0])
//              {
//                case rwTransparency:
//                  // rcbmp->ff |= 0x2000;
//                  rcbmp->flags.hasTransparency = fTrue;
//
//                  rcbmp->transparentIndex =
//                    BMP_RGBToColorIndex(transparencyData[1],
//                                        transparencyData[2],
//                                        transparencyData[3],
//                                        dstPalette, dstPaletteSize);
//                  break;
//
//                case rwTransparencyIndex:
//                  // rcbmp->ff |= 0x2000;
//                  rcbmp->flags.hasTransparency = fTrue;
//
//                  rcbmp->transparentIndex = transparencyData[1];
//                  break;
//
//                default:
//                  break;
//              }
//              break;
//             */
//        }
//        else
//        {
//            /* ptr += frameOffsets[frameNr]; */
//            printf("Apologies, but we are not yet able to process an 'Animation'.\n");
//        }
//        fclose(ptr_stream_file_out);
//    }
//
//    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_export_records_to_bmp()\n");
//    return 0;
//}

void lbxmgr_export_records_to_c(LBX_DATA * lbx)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbx_export_records_to_c()\n");

    char prefix_1of2[] = "unsigned char raw_data[";  /* 23 bytes*/
    char prefix_2of2[] = "] = {\n";  /* 6 bytes */
    char suffix_1of1[] = "};\n";  /* 3 bytes*/
    size_t len_str_buffer;
    int len_str_buffer_line_count;
    /* int bytes_per_field; */
    size_t bytes_per_field;
    int fields_per_line;
    char * tmp_field_bytes;
    size_t len_str_record_size;
    char str_record_size[256];
    bytes_per_field = 4;
    fields_per_line = 8;
    /* tmp_field_bytes = (char*)malloc(bytes_per_field + 1); */  /* +1 for "\n" from snprintf() */
    tmp_field_bytes = (char *)malloc(bytes_per_field + 1);  /* +1 for "\n" from snprintf() */
    int len_tmp_field_bytes;

    create_export_directory_path();
    create_export_directory_path_c(lbx->file->file_name_base);

    /* int record_size; */
    /* FILE * ptr_stream_file_out; */
    char export_file_path[MAX_PATH];

    unsigned char * lbx_read_buffer;
    unsigned char * tmp_lbx_read_buffer;
    lbx_read_buffer = (unsigned char *)malloc(sizeof(unsigned char) * 1);
    tmp_lbx_read_buffer = (unsigned char *)malloc(sizeof(unsigned char) * 1);
    char * str_buffer;
    char * tmp_str_buffer;
    str_buffer = (char *)malloc(sizeof(char) * 1);
    tmp_str_buffer = (char *)malloc(sizeof(char) * 1);

    int itr_records;
    int itr_record_bytes_buffer;
    int itr_record_bytes_buffer_rows;
    for (itr_records = 0; itr_records < lbx->header->entry_count; itr_records++)
    {

        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: lbx->records->entry[%d].record_file_name_base: %s\n", itr_records, lbx->record->entry[itr_records].record_file_name_base);

        int record_size = lbx->record->entry[itr_records].size;
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: record_size: %d\n", record_size);

        if (record_size == 0)
        {
            /* unsigned char raw_data[0] = {}; */
            continue;
        }

        strcpy(export_file_path, lbxmgr_data->export_directory_path);
        strcat(export_file_path, lbx->record->entry[itr_records].record_file_name_base);
        strcat(export_file_path, ".");
        strcat(export_file_path, "C");
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: export_file_path: %s\n", export_file_path);

        if (export_file_path[0] == 0)
        {
            printf("FAILURE: outFilePathName was empty");
        }

        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: fopen() BEGIN\n");
        /* ptr_stream_file_out = fopen(export_file_path, "wb"); */
        FILE * ptr_stream_file_out = fopen(export_file_path, "wb");
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: fopen() END\n");

        if (ptr_stream_file_out == nullptr) {
            printf("FAILURE: could not open file\n");
            printf("DEBUG: errno: %d strerror: %s\n", errno, strerror(errno));
        }

        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: fseek() BEGIN\n");
        fseek(lbx->file->file_stream, lbx->header->offset_table->entry[itr_records].begin, SEEK_SET);
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: fseek() END\n");


        /* unsigned char * lbx_read_buffer; */
        /* lbx_read_buffer = malloc(record_size); */
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: BEGIN: tmp_lbx_read_buffer = realloc(lbx_read_buffer, %d)\n", record_size);
        tmp_lbx_read_buffer = (unsigned char *)realloc(lbx_read_buffer, sizeof(unsigned char) * record_size);
        if (tmp_lbx_read_buffer == nullptr)
        {
            printf("FAILURE: tmp_lbx_read_buffer = realloc(lbx_read_buffer, %d)\n", record_size);
        }
        else
        {
            lbx_read_buffer = tmp_lbx_read_buffer;
        }
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: END: tmp_lbx_read_buffer = realloc(lbx_read_buffer, %d)\n", record_size);

        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: fread() BEGIN\n");
        fread(lbx_read_buffer, record_size, 1, lbx->file->file_stream);
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: fread() END\n");

        /*
         * Buffer Length:
         * Byte Count * 3; ? -> 0x??
         * (Byte Count - 1) * 2; ", "
         * "unsigned char rawData[" + ... + "] = {\n" + ... + "};\n"
         * ? digit count for size of array ?
         * Byte Count / 8; "\n"
         * e.g.,
         * MAGIC069.BIN = 80 bytes
         * MAGIC059.C = 512 bytes
         * 23 + 3 + 7 + 4 =
         * "0x00" * 80 = 320
         * ", " * (80 / 8) * 7 = 140
         * ",\n" * ((80 / 8) - 1) * 1 = 18
         * "\n" = 2
         * "};\n" = 4
         * e.g.,
         * MAGIC075.BIN = 207 bytes
         * MAGIC075.C = 1275 bytes
         * 23 + 3 + 7 + 4 =
         * 207 * 4 = 828
         * (1 * 207) + (2 * 207) - 1 =
         * e.g.,
         * MAGIC001.BIN = 49,657 bytes
         * MAGIC001.C = ?297,978? bytes
         * 23 + 5 + 7 + 4 =
         * 49657 * 4 = 198628
         * (2 * 49657) - 1 = 99313
         * e.g.,
         * MAGIC002.BIN = 1348 bytes
         * MAGIC002.C = 8123 bytes
         * 23 + 4 + 7 + 4 = 36
         * 1348 * 4 = 5392
         * (2 * 1348) - 1 = 2695
         * ...
         * byte diff = ( 1288 -  1276) = 12; row count =  26;
         * byte diff = (13673 - 13637) = 36; row count = 284;
         * byte diff = ( 2680 -  2644) = 36; row count =  55;
         * byte diff = ( 7241 -  7217) = 24; row count = 150; entry size = 1197, / 8 = 149.625
         * byte diff = ( 2248 -  2236) = 12; row count =  46; entry size = 367, / 8 = 45.875
         *
         *   0, ...,   9
         *  10, ...,  99
         * 100, ..., 255
         */

        /*
        if ((record_size >= 0) && (record_size <= 9))
        {
            record_size_digit_count = 1;
        }
        else if ((record_size >= 10) && (record_size <= 99))
        {
            record_size_digit_count = 2;
        }
        else if ((record_size >= 100) && (record_size <= 999))
        {
            record_size_digit_count = 3;
        }
        else if ((record_size >= 1000) && (record_size <= 9999))
        {
            record_size_digit_count = 4;
        }
        else if ((record_size >= 10000) && (record_size <= 99999))
        {
            record_size_digit_count = 5;
        }
        else if ((record_size >= 100000) && (record_size <= 999999))
        {
            record_size_digit_count = 6;
        }
        */
        /*
        int len_str_record_size;
        char * str_record_size;
        len_str_record_size = snprintf(NULL, 0, "%d", lbx->lbx_meta_data->record_sizes->entry[itr_record_bytes].size);
        str_record_size = malloc(len_str_record_size + 1);
        snprintf(str_record_size, len_str_record_size + 1, "%d", lbx->lbx_meta_data->record_sizes->entry[itr_record_bytes].size);
        */

        itoa(record_size, str_record_size, 10);
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: str_record_size: %s\n", str_record_size);

        len_str_record_size = strlen(str_record_size);
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: len_str_record_size: %d\n", len_str_record_size);

        len_str_buffer_line_count = (record_size / fields_per_line) + 1;
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: len_str_buffer_line_count: %d\n", len_str_buffer_line_count);


        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: strlen(prefix_1of2): %d\n", strlen(prefix_1of2));
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: strlen(str_record_size): %d\n", strlen(str_record_size));
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: strlen(prefix_2of2): %d\n", strlen(prefix_2of2));
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: (record_size * 4): %d\n", (record_size * 4));
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: ((record_size * 2) - 1): %d\n", ((record_size * 2) - 1));
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: strlen(suffix_1of1): %d\n", strlen(suffix_1of1));
        len_str_buffer = strlen(prefix_1of2) + strlen(str_record_size) + strlen(prefix_2of2) + (record_size * 4) + ((record_size * 2) - 1) + strlen(suffix_1of1);
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: len_str_buffer: %d\n", len_str_buffer);

        /* char * str_buffer = malloc(len_str_buffer); */
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: BEGIN: tmp_str_buffer = realloc(str_buffer, %d)\n", len_str_buffer);
        /* TODO(JWB): figure out what the issue/over-run is on this call to realloc() */
        tmp_str_buffer = (char *)realloc(str_buffer, sizeof(char) * (len_str_buffer * 4));
        if (tmp_str_buffer == nullptr)
        {
            printf("FAILURE: tmp_str_buffer = realloc(str_buffer, %d)\n", len_str_buffer);
        }
        else
        {
            str_buffer = tmp_str_buffer;
        }
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: END: tmp_str_buffer = realloc(str_buffer, %d)\n", len_str_buffer);

        strcpy(str_buffer, prefix_1of2);
        strcat(str_buffer, str_record_size);
        strcat(str_buffer, prefix_2of2);

        /*
         * Column Count
         * Row Count
         * ...
         * ", " vs. ",\n" vs. "\n"
         *
         */
        /* TODO(JamesBalcomb): figure out why it lags out right after this... */
        for (itr_record_bytes_buffer = 0; itr_record_bytes_buffer < record_size - 1; itr_record_bytes_buffer++)
        {
            if (itr_record_bytes_buffer == 0)
            {
                if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: ENTER: OUTER: itr_record_bytes_buffer: %d\n", itr_record_bytes_buffer);
            }
            // if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: lbx_read_buffer[%d]: %c\n", itr_record_bytes_buffer, lbx_read_buffer[itr_record_bytes_buffer]);
            for (itr_record_bytes_buffer_rows = 0; (itr_record_bytes_buffer_rows < 7) && (itr_record_bytes_buffer < record_size - 1); itr_record_bytes_buffer_rows++)
            {
                if ((itr_record_bytes_buffer == 0) && (itr_record_bytes_buffer_rows == 0))
                {
                    if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: ENTER: INNER: itr_record_bytes_buffer: %d\n", itr_record_bytes_buffer);
                }
                if (itr_record_bytes_buffer >= record_size - 1)
                {
                    /* if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: CONTINUE: INNER: itr_record_bytes_buffer: %d\n", itr_record_bytes_buffer); */
                    continue;
                }
                snprintf(tmp_field_bytes, bytes_per_field + 1, "0x%02X", lbx_read_buffer[itr_record_bytes_buffer]);
                /* if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: [%d] %s\n", itr_record_bytes_buffer, tmp_field_bytes); */
                strncat(str_buffer, tmp_field_bytes, bytes_per_field);
                strcat(str_buffer, ", ");
                itr_record_bytes_buffer++;
            }
            /* if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: EXIT: INNER: itr_record_bytes_buffer: %d\n", itr_record_bytes_buffer); */
            if (itr_record_bytes_buffer >= record_size - 1)
            {
                if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: CONTINUE: OUTER: itr_record_bytes_buffer: %d\n", itr_record_bytes_buffer);
                continue;
            }
            snprintf(tmp_field_bytes, bytes_per_field + 1, "0x%02X", lbx_read_buffer[itr_record_bytes_buffer]);
            /* if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: [%d] %s\n", itr_record_bytes_buffer, tmp_field_bytes); */
            strncat(str_buffer, tmp_field_bytes, bytes_per_field);
            strcat(str_buffer, ",\n");
        }
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: EXIT: OUTER: itr_record_bytes_buffer: %d\n", itr_record_bytes_buffer);
        /* if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: str_buffer: %s\n", str_buffer); */
        /* if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: itr_record_bytes_buffer: %d\n", itr_record_bytes_buffer); */

        /*
        snprintf(tmp_field_bytes, bytes_per_field + 1, "0x%02X", lbx_read_buffer[itr_record_bytes_buffer]);
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: [%d] %s\n", itr_record_bytes_buffer, tmp_field_bytes);
        */

        len_tmp_field_bytes = snprintf(nullptr, 0, "0x%02X", lbx_read_buffer[itr_record_bytes_buffer - 1]);
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: len_tmp_field_bytes: %d\n", len_tmp_field_bytes);

        snprintf(tmp_field_bytes, bytes_per_field + 1, "0x%02X", lbx_read_buffer[itr_record_bytes_buffer - 1]);
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: [%d] %s\n", itr_record_bytes_buffer - 1, tmp_field_bytes);
        strncat(str_buffer, tmp_field_bytes, bytes_per_field);

        strcat(str_buffer, "\n");
        strcat(str_buffer, suffix_1of1);

        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: len_str_buffer: %d\n", len_str_buffer);
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: strlen(str_buffer): %d\n", strlen(str_buffer));
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: str_record_size: %s\n", str_record_size);

        /* if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: str_buffer:\n%s\n", str_buffer); */

        /*
        int itr_test_increment;
        for (itr_test_increment = 0; itr_test_increment < 1; itr_test_increment++)
        {
            printf("DEBUG: LOOP: itr_test_increment: %d\n", itr_test_increment);
        }
        printf("DEBUG: EXIT: itr_test_increment: %d\n", itr_test_increment);
        */

        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: fwrite() BEGIN\n");
        fwrite(str_buffer, strlen(str_buffer), 1, ptr_stream_file_out);
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: fwrite() END\n");

        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: fclose() BEGIN\n");
        fclose(ptr_stream_file_out);
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: fclose() END\n");
    }

    if (tmp_str_buffer == nullptr)
    {
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: BEGIN: free(str_buffer)\n");
        free(str_buffer);
        str_buffer = nullptr;
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: END: free(str_buffer)\n");
    }
    else
    {
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: BEGIN: free(tmp_str_buffer)\n");
        free(tmp_str_buffer);
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: END: free(tmp_str_buffer)\n");
        tmp_str_buffer = nullptr;
    }

    if (tmp_lbx_read_buffer == nullptr)
    {
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: BEGIN: free(lbx_read_buffer)\n");
        free(lbx_read_buffer);
        lbx_read_buffer = nullptr;
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: END: free(lbx_read_buffer)\n");
    }
    else
    {
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: BEGIN: free(tmp_lbx_read_buffer)\n");
        free(tmp_lbx_read_buffer);
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: END: free(tmp_lbx_read_buffer)\n");
        tmp_lbx_read_buffer = nullptr;
    }

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbx_export_records_to_c()\n");
}

void lbxmgr_export_records_to_hex(LBX_DATA * lbx)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_export_records_to_hex()\n");

    create_export_directory_path();
    create_export_directory_path_hex(lbx->file->file_name_base);

    int record_size;
    FILE * ptr_stream_file_out;
    char export_file_path[MAX_PATH];
    int itr_record_bytes;
    for (itr_record_bytes = 0; itr_record_bytes < lbx->header->entry_count; itr_record_bytes++)
    {
        record_size = lbx->record->entry[itr_record_bytes].size;

        strcpy(export_file_path, lbxmgr_data->export_directory_path);
        strcat(export_file_path, lbx->record->entry[itr_record_bytes].record_file_name_base);
        strcat(export_file_path, ".");
        strcat(export_file_path, "HEX");
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: export_file_path: %s\n", export_file_path);

        if (export_file_path[0] == 0)
        {
            printf("FAILURE: outFilePathName was empty");
        }

        ptr_stream_file_out = fopen(export_file_path, "wb");

        if (ptr_stream_file_out == nullptr) {
            printf("FAILURE: could not open file\n");
            printf("DEBUG: errno: %d strerror: %s\n", errno, strerror(errno));
        }

        fseek(lbx->file->file_stream, lbx->header->offset_table->entry[itr_record_bytes].begin, SEEK_SET);

        char * lbx_read_buffer;
        lbx_read_buffer = (char*)malloc(record_size);
        fread(lbx_read_buffer, record_size, 1, lbx->file->file_stream);
        fwrite(lbx_read_buffer, record_size, 1, ptr_stream_file_out);
        fclose(ptr_stream_file_out);
    }

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_export_records_to_hex()\n");
}

void lbxmgr_export_records_to_gif(LBX_DATA * lbx)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_export_records_to_gif()\n");

    printf("This feature not yet implemented.\n");
    exit(EXIT_SUCCESS);

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_export_records_to_gif()\n");
}
