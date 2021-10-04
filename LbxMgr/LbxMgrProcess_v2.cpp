#include <iostream>             // std:: cout, endl; FILE;
// #include <stdio.h>          /* fclose(), fopen(), fread(), fwrite(), printf(); FILE */
#include <cstdio>
// #include <string.h>         /* memcpy(), strcat(), strcpy(), strncpy(), strlen(); */
#include <cstring>
// #include <malloc.h>         /* malloc(), realloc(); */
#include <cstdlib>              // malloc()

#include <lib_lbx_record_display.h>
//#include <lib_lbx_record_palette.h>
#include <lib_lbx_process_records.h>

// #include "LbxMgrProcess_v1.h"
#include "LbxMgrProcess_v2.h"
#include "LbxMgr.h"  // LBXMGR_DATA
#include "LbxMgrExport.h"
#include "LbxMgrPathsNames.h"
#include "ATS/ats_filenames.h"
#include "lib_lbx.h"
#include "lib_lbx_display.h"
#include "lib_lbx_meta_data.h"
#include "ATS/ats_filesystem.h"
#include "lib_lbx_load.h"
#include "lib_lbx_process_lbx.h"
#include "LbxMgrLoadLbxData.h"
#include "LbxMgrExportCsv.h"

/*
 * Process File Name(s) in FILE_NAMES
 *
 * Populate LBX_DATA
 *
 */

void lbxmgr_process_directory_mode(LBXMGR_DATA * lbxmgr_data)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_process_directory_mode()\n");

    populate_filenames_from_directory_path(lbxmgr_data->filenames, lbxmgr_data->directory_path);

    if (LBXMGR_DEBUG_MODE)
    {
        printf("\n");
        printf("File Names:\n");
        print_filenames(lbxmgr_data->filenames);
        printf("\n");
    }


    lbxmgr_process_lbx_files(lbxmgr_data);


    // destroy_filenames(filenames);

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_process_directory_mode()\n");
}

/*
 * All-in-One (AiO)
 * Load & Populate - LBX Header, LBX Data, and LBX Meta-Data
 *
 */
void lbxmgr_process_file_mode(LBXMGR_DATA * lbxmgr_data)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_process_file_mode()\n");

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: lbxmgr_data->file_path: %s\n", lbxmgr_data->file_path);
    ats_file_name(lbxmgr_data->file_path, &lbxmgr_data->file_name);
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: lbxmgr_data->file_name: %s\n", lbxmgr_data->file_name);
    ats_file_name_base(lbxmgr_data->file_name, &lbxmgr_data->file_name_base);
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: lbxmgr_data->file_name_base: %s\n", lbxmgr_data->file_name_base);

    populate_filenames_from_file_path(lbxmgr_data->filenames, lbxmgr_data->file_path);

    if (LBXMGR_DEBUG_MODE)
    {
        printf("\n");
        printf("File Names:\n");
        print_filenames(lbxmgr_data->filenames);
        printf("\n");
    }


    lbxmgr_process_lbx_files(lbxmgr_data);


    // destroy_filenames(filenames);

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_process_file_mode()\n");
}

void lbxmgr_process_entry_mode(LBXMGR_DATA * lbxmgr_data)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_process_entry_mode()\n");

    // std::cout << "This feature not yet implemented." << std::endl;

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: lbxmgr_data->file_path: %s\n", lbxmgr_data->file_path);
    ats_file_name(lbxmgr_data->file_path, &lbxmgr_data->file_name);
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: lbxmgr_data->file_name: %s\n", lbxmgr_data->file_name);
    ats_file_name_base(lbxmgr_data->file_name, &lbxmgr_data->file_name_base);
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: lbxmgr_data->file_name_base: %s\n", lbxmgr_data->file_name_base);

    populate_filenames_from_file_path(lbxmgr_data->filenames, lbxmgr_data->file_path);

    if (LBXMGR_DEBUG_MODE)
    {
        printf("\n");
        printf("File Names:\n");
        print_filenames(lbxmgr_data->filenames);
        printf("\n");
    }


    lbxmgr_process_lbx_files(lbxmgr_data);


    // destroy_filenames(filenames);

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_process_entry_mode()\n");
}

void lbxmgr_process_lbx_files(LBXMGR_DATA * lbxmgr_data)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_process_lbx_files()\n");

    int file_name_count;
    int itr_filenames;
    LBX_DATA * tmp_lbx_data;


    lbxmgr_prepare_lbx_data_array(lbxmgr_data);



    file_name_count = get_file_name_count(lbxmgr_data->filenames);

    for (itr_filenames = 0; itr_filenames < file_name_count; itr_filenames++)
    {
        tmp_lbx_data = lbxmgr_data->lbx_data_array[itr_filenames];



        liblbx_process_lbx_file(lbxmgr_data->lbx_data_array[itr_filenames]);



        // if (have_loaded_metadata = 0) liblbx_load_metadata()
        display_lbx_meta_data(lbxmgr_data->lbx_data_array[itr_filenames]);
        display_lbx_file_meta_data(lbxmgr_data->lbx_data_array[itr_filenames]);
        display_lbx_record_meta_data(lbxmgr_data->lbx_data_array[itr_filenames]);

        int itr_entry_index;
        for (itr_entry_index = 0; itr_entry_index < lbxmgr_data->lbx_data_array[itr_filenames]->header->entry_count; itr_entry_index++)
        {
            printf("%s (%d of %d) [%s]\n", lbxmgr_data->lbx_data_array[itr_filenames]->file->file_name_base, itr_entry_index + 1, lbxmgr_data->lbx_data_array[itr_filenames]->meta->entry_count, lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].entry_name);
            //display_lbx_flic_header(lbxmgr_data->lbx_data_array[itr_filenames], itr_entry_index);
            //display_lbx_image_custom_palette_header(lbxmgr_data->lbx_data_array[itr_filenames], itr_entry_index);
        }

        if (LBXMGR_OUTPUT_CSV == 1)
        {
            // EXPORT/MAIN/CSV/MAIN.CSV
            lbxmgr_output_to_csv(lbxmgr_data->lbx_data_array[itr_filenames]);
            // EXPORT/LBX_FILE_INFO.CSV
            lbxmgr_output_csv_lbx_files(lbxmgr_data);
            // EXPORT/LBX_RECORD_INFO.CSV
            lbxmgr_output_csv_lbx_records(lbxmgr_data);
        }

        if (LBX_EXPORT_BIN == 1)
        {
            lbxmgr_export_records_to_bin(tmp_lbx_data);
        }

// 2021-10-04 10:54
// Commenting out everything that should have been in Graphics/FLIC/BMP/Palette/Font
//        if (LBX_EXPORT_BMP == 1)
//        {
//            lbxmgr_export_records_to_bmp(tmp_lbx_data);
//        }

    }



    lbxmgr_unload_lbx_data_array(lbxmgr_data);

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_process_lbx_files()\n");
}
