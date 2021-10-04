// #include <stdio.h>          /* fclose(), fopen(), fread(), fwrite(), printf(); FILE */
#include <cstdio>
// #include <string.h>         /* memcpy(), strcat(), strcpy(), strncpy(), strlen(); */
#include <cstring>
// #include <malloc.h>         /* malloc(), realloc(); */
#include <cstdlib>              // malloc()

#include "LbxMgrPathsNames.h"
#include "LbxMgr.h"
#include "ATS/ats_filesystem.h"

void create_export_directory_path()
{
    lbxmgr_data->export_directory_path = (char *)malloc(sizeof(char) * (strlen(lbxmgr_data->directory_path) + strlen(path_separator) + strlen(export_directory_name) + 1));
    strcpy(lbxmgr_data->export_directory_path, lbxmgr_data->directory_path);
    ats_create_directory(lbxmgr_data->export_directory_path);
    strcat(lbxmgr_data->export_directory_path, path_separator);
    strcat(lbxmgr_data->export_directory_path, export_directory_name);
    ats_create_directory(lbxmgr_data->export_directory_path);
}

void create_export_directory_path_bin(char * file_name_base)
{
    lbxmgr_data->export_directory_path_bin = (char *)malloc(sizeof(char) * (strlen(lbxmgr_data->export_directory_path) + strlen(path_separator) + strlen(file_name_base) + strlen(path_separator) + strlen(export_directory_name_bin) + 1));
    strcpy(lbxmgr_data->export_directory_path_bin, lbxmgr_data->export_directory_path);
    ats_create_directory(lbxmgr_data->export_directory_path_bin);
    strcat(lbxmgr_data->export_directory_path_bin, path_separator);
    strcat(lbxmgr_data->export_directory_path_bin, file_name_base);
    ats_create_directory(lbxmgr_data->export_directory_path_bin);
    strcat(lbxmgr_data->export_directory_path_bin, path_separator);
    strcat(lbxmgr_data->export_directory_path_bin, export_directory_name_bin);
    ats_create_directory(lbxmgr_data->export_directory_path_bin);
}

void create_export_directory_path_bmp(char * file_name_base)
{
    lbxmgr_data->export_directory_path_bmp = (char *)malloc(sizeof(char) * (strlen(lbxmgr_data->export_directory_path) + strlen(path_separator) + strlen(file_name_base) + strlen(path_separator) + strlen(export_directory_name_bmp) + 1));
    strcpy(lbxmgr_data->export_directory_path_bmp, lbxmgr_data->export_directory_path);
    ats_create_directory(lbxmgr_data->export_directory_path_bmp);
    strcat(lbxmgr_data->export_directory_path_bmp, path_separator);
    strcat(lbxmgr_data->export_directory_path_bmp, file_name_base);
    ats_create_directory(lbxmgr_data->export_directory_path_bmp);
    strcat(lbxmgr_data->export_directory_path_bmp, path_separator);
    strcat(lbxmgr_data->export_directory_path_bmp, export_directory_name_bmp);
    ats_create_directory(lbxmgr_data->export_directory_path_bmp);
}

void create_export_directory_path_c(char * file_name_base)
{
    lbxmgr_data->export_directory_path_c = (char *)malloc(sizeof(char) * (strlen(lbxmgr_data->export_directory_path) + strlen(path_separator) + strlen(file_name_base) + strlen(path_separator) + strlen(export_directory_name_c) + 1));
    strcpy(lbxmgr_data->export_directory_path_c, lbxmgr_data->export_directory_path);
    ats_create_directory(lbxmgr_data->export_directory_path_c);
    strcat(lbxmgr_data->export_directory_path_c, path_separator);
    strcat(lbxmgr_data->export_directory_path_c, file_name_base);
    ats_create_directory(lbxmgr_data->export_directory_path_c);
    strcat(lbxmgr_data->export_directory_path_c, path_separator);
    strcat(lbxmgr_data->export_directory_path_c, export_directory_name_c);
    ats_create_directory(lbxmgr_data->export_directory_path_c);
}

void create_export_directory_path_csv(char * file_name_base)
{
    lbxmgr_data->export_directory_path_csv = (char *)malloc(sizeof(char) * (strlen(lbxmgr_data->export_directory_path) + strlen(path_separator) + strlen(file_name_base) + strlen(path_separator) + strlen(export_directory_name_csv) + 1));
    strcpy(lbxmgr_data->export_directory_path_csv, lbxmgr_data->export_directory_path);
    ats_create_directory(lbxmgr_data->export_directory_path_csv);
    strcat(lbxmgr_data->export_directory_path_csv, path_separator);
    strcat(lbxmgr_data->export_directory_path_csv, file_name_base);
    ats_create_directory(lbxmgr_data->export_directory_path_csv);
    strcat(lbxmgr_data->export_directory_path_csv, path_separator);
    strcat(lbxmgr_data->export_directory_path_csv, export_directory_name_csv);
    ats_create_directory(lbxmgr_data->export_directory_path_csv);
}

void create_export_directory_path_hex(char * file_name_base)
{
    lbxmgr_data->export_directory_path_hex = (char *)malloc(sizeof(char) * (strlen(lbxmgr_data->export_directory_path) + strlen(path_separator) + strlen(file_name_base) + strlen(path_separator) + strlen(export_directory_name_hex) + 1));
    strcpy(lbxmgr_data->export_directory_path_hex, lbxmgr_data->export_directory_path);
    ats_create_directory(lbxmgr_data->export_directory_path_hex);
    strcat(lbxmgr_data->export_directory_path_hex, path_separator);
    strcat(lbxmgr_data->export_directory_path_hex, file_name_base);
    ats_create_directory(lbxmgr_data->export_directory_path_hex);
    strcat(lbxmgr_data->export_directory_path_hex, path_separator);
    strcat(lbxmgr_data->export_directory_path_hex, export_directory_name_hex);
    ats_create_directory(lbxmgr_data->export_directory_path_hex);
}

void create_export_directory_path_gif(char * file_name_base)
{
    lbxmgr_data->export_directory_path_gif = (char *)malloc(sizeof(char) * (strlen(lbxmgr_data->export_directory_path) + strlen(path_separator) + strlen(file_name_base) + strlen(path_separator) + strlen(export_directory_name_gif) + 1));
    strcpy(lbxmgr_data->export_directory_path_gif, lbxmgr_data->export_directory_path);
    ats_create_directory(lbxmgr_data->export_directory_path_gif);
    strcat(lbxmgr_data->export_directory_path_gif, path_separator);
    strcat(lbxmgr_data->export_directory_path_gif, file_name_base);
    ats_create_directory(lbxmgr_data->export_directory_path_gif);
    strcat(lbxmgr_data->export_directory_path_gif, path_separator);
    strcat(lbxmgr_data->export_directory_path_gif, export_directory_name_gif);
    ats_create_directory(lbxmgr_data->export_directory_path_gif);
}
