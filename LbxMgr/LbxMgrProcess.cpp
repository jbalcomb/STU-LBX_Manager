// #include <stdio.h>          /* fclose(), fopen(), fread(), fwrite(), printf(); FILE */
#include <cstdio>
// #include <string.h>         /* memcpy(), strcat(), strcpy(), strncpy(), strlen(); */
#include <cstring>
// #include <malloc.h>         /* malloc(), realloc(); */
#include <cstdlib>              // malloc()

#include "LbxMgrProcess.h"
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
    char * file_path;

    for (itr_filenames = 0; itr_filenames < file_name_count; itr_filenames++)
    {
        // file_path = get_file_path(filenames, itr_filenames);
        // LBX_DATA * lbx;
        // lbx = create_lbx_data(file_path);
        // lbx_load_header(lbx);
        // lbx_display_header(lbx);
        // destroy_lbx_data(lbx);

        file_path = get_file_path(filenames, itr_filenames);
        lbxmgr_data->lbx_data_array[itr_filenames] = create_lbx_data(file_path);
        lbx_load_header(lbxmgr_data->lbx_data_array[itr_filenames]);
        populate_all_lbx_meta_data(lbxmgr_data->lbx_data_array[itr_filenames]);

        if (LBX_EXPORT_CSV == 1)
        {
            lbxmgr_export_to_csv(lbxmgr_data->lbx_data_array[itr_filenames]);
        }
        if (LBX_EXPORT_BIN == 1)
        {
            lbxmgr_export_records_to_bin(lbxmgr_data->lbx_data_array[itr_filenames]);
        }
        if (LBX_EXPORT_C == 1)
        {
            lbxmgr_export_records_to_c(lbxmgr_data->lbx_data_array[itr_filenames]);
        }
        if (LBX_EXPORT_HEX == 1)
        {
            lbxmgr_export_records_to_hex(lbxmgr_data->lbx_data_array[itr_filenames]);
        }

    }

    for (itr_filenames = 0; itr_filenames < file_name_count; itr_filenames++)
    {
        lbx_display_header(lbxmgr_data->lbx_data_array[itr_filenames]);
    }


    create_export_directory_path();

    /*
     * All LBX Files - CSV  (~= ./EXPORT/LBX_FILES.CSV)
     */
    // char export_file_path_lbx_all_files_csv[] = "F:\\devel_data\\STU\\MoM131_LBX\\EXPORT\\LBX_FILE_INFO.CSV";
    char * export_file_path_lbx_all_files_csv;
    export_file_path_lbx_all_files_csv = (char *)malloc(sizeof(char) * (strlen(lbxmgr_data->export_directory_path) + strlen(path_separator) + strlen("LBX_FILE_INFO.CSV") + 1));
    strcpy(export_file_path_lbx_all_files_csv, lbxmgr_data->export_directory_path);
    strcat(export_file_path_lbx_all_files_csv, path_separator);
    strcat(export_file_path_lbx_all_files_csv, "LBX_FILE_INFO.CSV");
    FILE * export_lbx_all_files_csv;
    export_lbx_all_files_csv = fopen(export_file_path_lbx_all_files_csv, "wb");
    char export_lbx_all_files_csv_headings[] = "File_Name,Entry_Count,LBX_Type,File_Size,File_Date,String_Table,Header_Length,Offset_BOD,CRC32,MD5,SHA1,SHA256\n";
    fwrite(export_lbx_all_files_csv_headings, strlen(export_lbx_all_files_csv_headings), 1, export_lbx_all_files_csv);

    for (itr_filenames = 0; itr_filenames < file_name_count; itr_filenames++)
    {

        /* File Name */
        fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->meta_file_name, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->meta_file_name), 1, export_lbx_all_files_csv);
        fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
        /* Entry Count */
        fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->entry_count_string_dec, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->entry_count_string_dec), 1, export_lbx_all_files_csv);
        fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
        /* LBX Type */
        fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->type_string_dec, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->type_string_dec), 1, export_lbx_all_files_csv);
        fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
        /* File Size */
        fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_size_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_size_string), 1, export_lbx_all_files_csv);
        fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
        /* File Date */
        fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->earliest_timestamp_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->earliest_timestamp_string), 1, export_lbx_all_files_csv);
        fwrite(",", strlen(","), 1, export_lbx_all_files_csv);

        /* Has String Table */
        fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->has_string_table_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->has_string_table_string), 1, export_lbx_all_files_csv);
        fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
        /* Header Length */
        fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->header_length_string_dec, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->header_length_string_dec), 1, export_lbx_all_files_csv);
        fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
        /* Offset BOD */
        fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->offset_bod_string_hex, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->offset_bod_string_hex), 1, export_lbx_all_files_csv);
        fwrite(",", strlen(","), 1, export_lbx_all_files_csv);

        /* Checksums - CRC32 */
        fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_crc32_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_crc32_string), 1, export_lbx_all_files_csv);
        fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
        /* Checksums - MD5 */
        fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_md5_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_md5_string), 1, export_lbx_all_files_csv);
        fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
        /* Checksums - SHA1 */
        fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_sha1_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_sha1_string), 1, export_lbx_all_files_csv);
        fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
        /* Checksums - SHA256 */
        fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_sha256_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_sha256_string), 1, export_lbx_all_files_csv);
        fwrite(",", strlen(","), 1, export_lbx_all_files_csv);

        fwrite("\n", strlen("\n"), 1, export_lbx_all_files_csv);

    }

    fclose(export_lbx_all_files_csv);


    /*
     * All LBX Files, All LBX Records - CSV  (~= ./EXPORT/LBX_RECORDS.CSV)
     */
    // char export_file_path_lbx_all_files_all_records_csv[] = "F:\\devel_data\\STU\\MoM131_LBX\\EXPORT\\LBX_RECORD_INFO.CSV";
    char * export_file_path_lbx_all_files_all_records_csv;
    export_file_path_lbx_all_files_all_records_csv = (char *)malloc(sizeof(char) * (strlen(lbxmgr_data->export_directory_path) + strlen(path_separator) + strlen("LBX_RECORD_INFO.CSV") + 1));
    strcpy(export_file_path_lbx_all_files_all_records_csv, lbxmgr_data->export_directory_path);
    strcat(export_file_path_lbx_all_files_all_records_csv, path_separator);
    strcat(export_file_path_lbx_all_files_all_records_csv, "LBX_RECORD_INFO.CSV");
    FILE * export_lbx_all_files_all_records_csv;
    export_lbx_all_files_all_records_csv = fopen(export_file_path_lbx_all_files_all_records_csv, "wb");
    char export_lbx_all_files_all_records_csv_headings[] = "Entry_Number,Entry_size,Entry_Name,Entry_Description,Entry_Type,Entry_Type_description,File_Name,Entry_Count,LBX_Type,File_Size,File_Date,String_Table,Header_Length,Offset_BOD,CRC32,MD5,SHA1,SHA256\n";
    fwrite(export_lbx_all_files_all_records_csv_headings, strlen(export_lbx_all_files_all_records_csv_headings), 1, export_lbx_all_files_all_records_csv);

    int itr_entry_index;

    for (itr_filenames = 0; itr_filenames < file_name_count; itr_filenames++)
    {
        for (itr_entry_index = 0; itr_entry_index < lbxmgr_data->lbx_data_array[itr_filenames]->header->entry_count; itr_entry_index++)
        {

            /* Record Entry Number */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].entry_number_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].entry_number_string), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);
            /* Record Size */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].size_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].size_string), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);
            /* Record Entry Name */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].entry_name, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].entry_name), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);
            /* Record Entry Description */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].entry_description, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].entry_description), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);
            /* Record Type */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].type_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].type_string), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);
            /* Record Type Description */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].type_description_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->records->entry[itr_entry_index].type_description_string), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);

            /* File Name */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->meta_file_name, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->meta_file_name), 1, export_lbx_all_files_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
            /* Entry Count */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->entry_count_string_dec, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->entry_count_string_dec), 1, export_lbx_all_files_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
            /* LBX Type */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->type_string_dec, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->type_string_dec), 1, export_lbx_all_files_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
            /* File Size */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_size_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_size_string), 1, export_lbx_all_files_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
            /* File Date */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->earliest_timestamp_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->earliest_timestamp_string), 1, export_lbx_all_files_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_csv);

            /* Has String Table */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->has_string_table_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->has_string_table_string), 1, export_lbx_all_files_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
            /* Header Length */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->header_length_string_dec, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->header_length_string_dec), 1, export_lbx_all_files_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
            /* Offset BOD */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->offset_bod_string_hex, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->offset_bod_string_hex), 1, export_lbx_all_files_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_csv);

            /* Checksums - CRC32 */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_crc32_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_crc32_string), 1, export_lbx_all_files_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
            /* Checksums - MD5 */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_md5_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_md5_string), 1, export_lbx_all_files_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
            /* Checksums - SHA1 */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_sha1_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_sha1_string), 1, export_lbx_all_files_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_csv);
            /* Checksums - SHA256 */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_sha256_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_sha256_string), 1, export_lbx_all_files_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_csv);

            fwrite("\n", strlen("\n"), 1, export_lbx_all_files_all_records_csv);

        }
    }

    fclose(export_lbx_all_files_all_records_csv);


    for (itr_filenames = 0; itr_filenames < file_name_count; itr_filenames++)
    {
        destroy_lbx_data(lbxmgr_data->lbx_data_array[itr_filenames]);
    }

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_process_lbx()\n");
}
