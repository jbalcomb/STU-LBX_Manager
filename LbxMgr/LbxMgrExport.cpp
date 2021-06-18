// #include <stdio.h>          /* fclose(), fopen(), fread(), fwrite(), printf(); FILE */
#include <cstdio>
// #include <string.h>         /* memcpy(), strcat(), strcpy(), strncpy(), strlen(); */
#include <cstring>
// #include <malloc.h>         /* malloc(), realloc(); */
#include <cstdlib>              // malloc()

#include "LbxMgrExport.h"
#include "LbxMgr.h"
#include "LbxMgrPathsNames.h"
#include "lib_lbx.h"
#include "ATS/ats_filesystem.h"



void lbxmgr_export_to_csv(LBX_DATA * lbx)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_export_to_csv()\n");

    create_export_directory_path();
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: lbx->meta->file_name_base: %s\n", lbx->meta->meta_file_name_base);
    create_export_directory_path_csv(lbx->meta->meta_file_name_base);


    char * export_file_path;

    /* lbx_create_directory(lbx->file->export_directory_path); */
    // char strCSV[] = "CSV";
    // ats_build_file_path_and_name(&export_file_path, lbxmgr_data->export_directory_path, lbx->meta->file_name_base, strCSV);
    ats_build_file_path_and_name(&export_file_path, lbxmgr_data->export_directory_path_csv, lbx->meta->meta_file_name_base, export_file_extension_csv);

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

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_export_to_csv()\n");
}

void lbxmgr_export_records_to_bin(LBX_DATA * lbx)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbx_export_entries_to_bin()\n");

    create_export_directory_path();
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: lbx->meta->file_name_base: %s\n", lbx->meta->meta_file_name_base);
    create_export_directory_path_bin(lbx->meta->meta_file_name_base);

    int record_size;
    FILE * ptr_stream_file_out;
    char * export_file_path;
    int itr_record_bytes;
    for (itr_record_bytes = 0; itr_record_bytes < lbx->header->entry_count; itr_record_bytes++)
    {
        record_size = lbx->meta->records->entry[itr_record_bytes].meta_size;

        // ats_build_file_path_and_name(&export_file_path, lbx->file->export_directory_path_bin, lbx->meta->records->entry[itr_record_bytes].record_file_name_base, export_file_extension_bin);
        ats_build_file_path_and_name(&export_file_path, lbxmgr_data->export_directory_path_bin, lbx->meta->records->entry[itr_record_bytes].meta_record_file_name_base, export_file_extension_bin);

        if (LBXMGR_DEBUG_STRUGGLE_MODE) printf("DEBUG: export_file_path: %s\n", export_file_path);

        ptr_stream_file_out = fopen(export_file_path, "wb");

        if (ptr_stream_file_out == nullptr) {
            printf("FAILURE: could not open file\n");
            printf("DEBUG: errno: %d strerror: %s\n", errno, strerror(errno));
        }

        fseek(lbx->file_stream, lbx->header->offset_table->entry[itr_record_bytes].begin, SEEK_SET);

        char * lbx_read_buffer;
        lbx_read_buffer = (char *)malloc(sizeof(char) * (record_size + 1));
        fread(lbx_read_buffer, record_size, 1, lbx->file_stream);
        fwrite(lbx_read_buffer, record_size, 1, ptr_stream_file_out);
        fclose(ptr_stream_file_out);
    }

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbx_export_entries_to_bin()\n");
}

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
    create_export_directory_path_c(lbx->meta->meta_file_name_base);

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

        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: lbx->records->entry[%d].record_file_name_base: %s\n", itr_records, lbx->meta->records->entry[itr_records].meta_record_file_name_base);

        int record_size = lbx->meta->records->entry[itr_records].meta_size;
        if (LBX_DEBUG_VERBOSE_MODE) printf("DEBUG: record_size: %d\n", record_size);

        if (record_size == 0)
        {
            /* unsigned char raw_data[0] = {}; */
            continue;
        }

        strcpy(export_file_path, lbxmgr_data->export_directory_path);
        strcat(export_file_path, lbx->meta->records->entry[itr_records].meta_record_file_name_base);
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
        fseek(lbx->file_stream, lbx->header->offset_table->entry[itr_records].begin, SEEK_SET);
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
        fread(lbx_read_buffer, record_size, 1, lbx->file_stream);
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
    create_export_directory_path_hex(lbx->meta->meta_file_name_base);

    int record_size;
    FILE * ptr_stream_file_out;
    char export_file_path[MAX_PATH];
    int itr_record_bytes;
    for (itr_record_bytes = 0; itr_record_bytes < lbx->header->entry_count; itr_record_bytes++)
    {
        record_size = lbx->meta->records->entry[itr_record_bytes].meta_size;

        strcpy(export_file_path, lbxmgr_data->export_directory_path);
        strcat(export_file_path, lbx->meta->records->entry[itr_record_bytes].meta_record_file_name_base);
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

        fseek(lbx->file_stream, lbx->header->offset_table->entry[itr_record_bytes].begin, SEEK_SET);

        char * lbx_read_buffer;
        lbx_read_buffer = (char*)malloc(record_size);
        fread(lbx_read_buffer, record_size, 1, lbx->file_stream);
        fwrite(lbx_read_buffer, record_size, 1, ptr_stream_file_out);
        fclose(ptr_stream_file_out);
    }

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_export_records_to_hex()\n");
}
