#include <cstdlib>              // malloc();
#include <cstring>              // strlen();

#include "LbxMgrExportCsv.h"
#include "LbxMgr.h"             // LBXMGR_DATA; LBXMGR_DEBUG_MODE;
#include "LbxMgrPathsNames.h"   // create_export_directory_path();

#include "lib_lbx.h"  // LBX_DATA

void lbxmgr_output_csv_lbx_files(LBXMGR_DATA * lbxmgr_data)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_output_csv_lbx_files()\n");



    int file_name_count;
    file_name_count = get_file_name_count(lbxmgr_data->filenames);
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: file_name_count: %d\n", file_name_count);

    int itr_filenames;

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



    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_output_csv_lbx_files()\n");
}

void lbxmgr_output_csv_lbx_records(LBXMGR_DATA * lbxmgr_data)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_output_csv_lbx_records()\n");



    int file_name_count;
    file_name_count = get_file_name_count(lbxmgr_data->filenames);
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: file_name_count: %d\n", file_name_count);

    int itr_filenames;
    int itr_entry_index;

    create_export_directory_path();

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
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->meta_file_name, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->meta_file_name), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);
            /* Entry Count */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->entry_count_string_dec, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->entry_count_string_dec), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);
            /* LBX Type */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->type_string_dec, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->type_string_dec), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);
            /* File Size */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_size_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_size_string), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);
            /* File Date */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->earliest_timestamp_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->earliest_timestamp_string), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);

            /* Has String Table */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->has_string_table_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->has_string_table_string), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);
            /* Header Length */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->header_length_string_dec, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->header_length_string_dec), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);
            /* Offset BOD */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->offset_bod_string_hex, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->offset_bod_string_hex), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);

            /* Checksums - CRC32 */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_crc32_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_crc32_string), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);
            /* Checksums - MD5 */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_md5_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_md5_string), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);
            /* Checksums - SHA1 */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_sha1_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_sha1_string), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);
            /* Checksums - SHA256 */
            fwrite(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_sha256_string, strlen(lbxmgr_data->lbx_data_array[itr_filenames]->meta->file_checksum_sha256_string), 1, export_lbx_all_files_all_records_csv);
            fwrite(",", strlen(","), 1, export_lbx_all_files_all_records_csv);

            fwrite("\n", strlen("\n"), 1, export_lbx_all_files_all_records_csv);

        }
    }

    fclose(export_lbx_all_files_all_records_csv);



    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_output_csv_lbx_records()\n");
}

void lbxmgr_output_to_csv(LBX_DATA * lbx)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_output_to_csv()\n");

    create_export_directory_path();
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: lbx->file->file_name_base: %s\n", lbx->file->file_name_base);
    create_export_directory_path_csv(lbx->file->file_name_base);


    char * export_file_path;

    /* lbx_create_directory(lbx->file->export_directory_path); */
    // char strCSV[] = "CSV";
    // ats_build_file_path_and_name(&export_file_path, lbxmgr_data->export_directory_path, lbx->meta->file_name_base, strCSV);
    ats_build_file_path_and_name(&export_file_path, lbxmgr_data->export_directory_path_csv, lbx->file->file_name_base, export_file_extension_csv);

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: export_file_path: %s\n", export_file_path);


    FILE * ptr_stream_file_out;

    ptr_stream_file_out = fopen(export_file_path, "wb");

    char csv_info_headings[] = "File_Name,Entry_Count,LBX_Type,File_Size,File_Date,String_Table,Header_Length,Offset_BOD,CRC32\n";

    fwrite(csv_info_headings, strlen(csv_info_headings), 1, ptr_stream_file_out);

    /*
     * Name
     */

    fwrite(lbx->meta->meta_file_name, strlen(lbx->meta->meta_file_name), 1, ptr_stream_file_out);

    fwrite(",", strlen(","), 1, ptr_stream_file_out);

    /*
     * Type
     */

    int len_lbx_type = snprintf(nullptr, 0, "%d", lbx->header->type);
    char* str_lbx_type = (char *)malloc(len_lbx_type + 1);
    snprintf(str_lbx_type, len_lbx_type + 1, "%d", lbx->header->type);
    fwrite(str_lbx_type, strlen(str_lbx_type), 1, ptr_stream_file_out);

    fwrite(",", strlen(","), 1, ptr_stream_file_out);

    /*
     * Entries
     */

    fwrite(lbx->meta->entry_count_string_dec, strlen(lbx->meta->entry_count_string_dec), 1, ptr_stream_file_out);

    fwrite(",", strlen(","), 1, ptr_stream_file_out);

    /*
     * Size,Timestamp,Offset_BOD,Offset_EOD,Has_String_Table,Header_Length,Header_Padding_Length,CRC32,MD5,SHA1
     */

    fwrite(lbx->meta->file_checksum_crc32_string, strlen(lbx->meta->file_checksum_crc32_string), 1, ptr_stream_file_out);

    fwrite(",", strlen(","), 1, ptr_stream_file_out);

    fwrite("\n", strlen("\n"), 1, ptr_stream_file_out);

    fclose(ptr_stream_file_out);

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_output_to_csv()\n");
}
