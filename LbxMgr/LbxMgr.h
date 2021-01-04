#ifndef LBXMGR_H
#define LBXMGR_H

#include "lib_lbx.h"
#include "ATS/ats_filesystem.h"

extern int LBXMGR_DEBUG_MODE;
extern int LBXMGR_DEBUG_VERBOSE_MODE;
extern int LBXMGR_DEBUG_STRUGGLE_MODE;

extern char path_separator[];
extern char export_directory_name[];
extern char export_directory_name_bin[];
extern char export_directory_name_c[];
extern char export_directory_name_csv[];
extern char export_directory_name_hex[];
extern char export_file_extension_bin[];
extern char export_file_extension_c[];
extern char export_file_extension_csv[];
extern char export_file_extension_hex[];

static const int lbx_data_array_initial_element_count = 1024;  /* initial capacity of array elements */
static const float lbx_data_array_growth_rate = 1.6f;  /* geometric growth of array capacity */

struct s_LBXMGR_DATA
{
    char * directory_path;
    char * file_path;
    char * file_name;
    char * file_name_base;

    char * export_directory_path;
    char * export_directory_path_bin;
    char * export_directory_path_c;
    char * export_directory_path_csv;
    char * export_directory_path_hex;

    int lbx_file_count;
    int lbx_data_element_count;
    // LBX_DATA ** lbx_data_array;
    LBX_DATA * lbx_data_array[1024];
};
typedef struct s_LBXMGR_DATA LBXMGR_DATA;
extern LBXMGR_DATA * lbxmgr_data;

void initialize_program(int argc, char *argv[], char *env[]);

#endif  // LBXMGR_H
