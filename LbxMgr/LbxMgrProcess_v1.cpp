// #include <stdio.h>          /* fclose(), fopen(), fread(), fwrite(), printf(); FILE */
#include <cstdio>
// #include <string.h>         /* memcpy(), strcat(), strcpy(), strncpy(), strlen(); */
#include <cstring>
// #include <malloc.h>         /* malloc(), realloc(); */
#include <cstdlib>              // malloc()
#include <lib_lbx_record_display.h>
#include <lib_lbx_record_palette.h>

#include "LbxMgrProcess_v1.h"
#include "LbxMgr.h"
#include "LbxMgrExport.h"
#include "LbxMgrPathsNames.h"
#include "ATS/ats_filenames.h"
#include "lib_lbx.h"
#include "lib_lbx_display.h"
#include "lib_lbx_meta.h"
#include "ATS/ats_filesystem.h"

/*
 * Process File Name(s) in FILE_NAMES
 *
 * Populate LBX_DATA
 *
 */

void lbxmgr_process_directory_mode(FILE_NAMES * filenames)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_process_directory_mode()\n");

    populate_filenames_from_directory_path(filenames, lbxmgr_data->directory_path);

    if (LBXMGR_DEBUG_MODE)
    {
        printf("\n");
        printf("File Names:\n");
        print_filenames(filenames);
        printf("\n");
    }


    lbxmgr_process_lbx(filenames);


    // destroy_filenames(filenames);

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_process_directory_mode()\n");
}

/*
 * All-in-One (AiO)
 * Load & Populate - LBX Header, LBX Data, and LBX Meta-Data
 *
 */
void lbxmgr_process_file_mode(FILE_NAMES * filenames)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_process_file_mode()\n");

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: lbxmgr_data->file_path: %s\n", lbxmgr_data->file_path);
    ats_file_name(lbxmgr_data->file_path, &lbxmgr_data->file_name);
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: lbxmgr_data->file_name: %s\n", lbxmgr_data->file_name);
    ats_file_name_base(lbxmgr_data->file_name, &lbxmgr_data->file_name_base);
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: lbxmgr_data->file_name_base: %s\n", lbxmgr_data->file_name_base);

    populate_filenames_from_file_path(filenames, lbxmgr_data->file_path);

    if (LBXMGR_DEBUG_MODE)
    {
        printf("\n");
        printf("File Names:\n");
        print_filenames(filenames);
        printf("\n");
    }


    lbxmgr_process_lbx(filenames);


    // destroy_filenames(filenames);

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_process_file_mode()\n");
}

void lbxmgr_process_entry_mode(FILE_NAMES * filenames)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_process_entry_mode()\n");

    printf("This feature not yet implemented.");

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_process_entry_mode()\n");
}

void lbxmgr_process_lbx(FILE_NAMES * filenames)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_process_lbx()\n");

    int file_name_count;
    file_name_count = get_file_name_count(filenames);
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: file_name_count: %d\n", file_name_count);

    int itr_filenames;
    int itr_entry_index;
    char * file_path;

    for (itr_filenames = 0; itr_filenames < file_name_count; itr_filenames++)
    {
        // file_path = get_file_path(filenames, itr_filenames);
        // LBX_DATA * lbx;
        // lbx = create_lbx_data(file_path);
        lbx_load_header(lbx);
        // lbx_display_header(lbx);
        // destroy_lbx_data(lbx);

        file_path = get_file_path(filenames, itr_filenames);
        lbxmgr_data->lbx_data_array[itr_filenames] = create_lbx_data(file_path);
        lbx_load_header(lbxmgr_data->lbx_data_array[itr_filenames]);
        populate_all_lbx_meta_data(lbxmgr_data->lbx_data_array[itr_filenames]);

        /*
        switch (EXPORT_MODE)
        {

        }
         */

        /*
        if (LBXMGR_OUTPUT_CSV == 1)
        {
            lbxmgr_export_to_csv(lbxmgr_data->lbx_data_array[itr_filenames]);
        }
        */

        /*
        if (LBX_EXPORT_BIN == 1)
        {
            lbxmgr_export_records_to_bin(lbxmgr_data->lbx_data_array[itr_filenames]);
        }
        */
        if (LBX_EXPORT_BMP == 1)
        {
            lbxmgr_export_records_to_bmp(lbxmgr_data->lbx_data_array[itr_filenames]);
        }
        /*
        if (LBX_EXPORT_C == 1)
        {
            lbxmgr_export_records_to_c(lbxmgr_data->lbx_data_array[itr_filenames]);
        }
        if (LBX_EXPORT_HEX == 1)
        {
            lbxmgr_export_records_to_hex(lbxmgr_data->lbx_data_array[itr_filenames]);
        }
        if (LBX_EXPORT_GIF == 1)
        {
            lbxmgr_export_records_to_gif(lbxmgr_data->lbx_data_array[itr_filenames]);
        }
        */

        display_lbx_record_meta_data(lbxmgr_data->lbx_data_array[itr_filenames]);
        for (itr_entry_index = 0; itr_entry_index < lbxmgr_data->lbx_data_array[itr_filenames]->header->entry_count; itr_entry_index++)
        {
            printf("%s (%d of %d) [%s]\n", lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_name_base, itr_entry_index + 1, lbxmgr_data->lbx_data_array[itr_filenames]->meta->entry_count, lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].entry_name);
            display_lbx_image_header(lbxmgr_data->lbx_data_array[itr_filenames], itr_entry_index);
            display_lbx_image_custom_palette_header(lbxmgr_data->lbx_data_array[itr_filenames], itr_entry_index);
        }
        if (LBX_EXPORT_BMP == 1)
        {
            lbxmgr_export_records_to_bmp(lbxmgr_data->lbx_data_array[itr_filenames]);
        }

    }

    /*
    for (itr_filenames = 0; itr_filenames < file_name_count; itr_filenames++)
    {
        lbx_display_header(lbxmgr_data->lbx_data_array[itr_filenames]);
        display_lbx_record_meta_data(lbxmgr_data->lbx_data_array[itr_filenames]);
    }

    for (itr_filenames = 0; itr_filenames < file_name_count; itr_filenames++) {
        for (itr_entry_index = 0; itr_entry_index < lbxmgr_data->lbx_data_array[itr_filenames]->header->entry_count; itr_entry_index++)
        {
            printf("%s (%d of %d) [%s]\n", lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_name_base, itr_entry_index + 1, lbxmgr_data->lbx_data_array[itr_filenames]->meta->entry_count, lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].entry_name);
            display_lbx_image_header(lbxmgr_data->lbx_data_array[itr_filenames], itr_entry_index);
            display_lbx_image_custom_palette_header(lbxmgr_data->lbx_data_array[itr_filenames], itr_entry_index);
        }
    }
    */





    for (itr_filenames = 0; itr_filenames < file_name_count; itr_filenames++)
    {
        destroy_lbx_data(lbxmgr_data->lbx_data_array[itr_filenames]);
    }

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_process_lbx()\n");
}
