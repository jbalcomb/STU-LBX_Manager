/*
 * C:\Development\STU-LBX_Manager\cmake-build-debug
 * CD \Development\STU-LBX_Manager\cmake-build-debug
 *
 * File Mode:
 * LbxMgr.exe -f F:\devel_data\STU\MoM131_LBX\INTRO.LBX --dump -vvvvv
 * LbxMgr.exe -f F:\devel_data\STU\MoM131_LBX\SOUNDFX.LBX --dump -vvvvv > LbxMgr.log
 *
 * Dump EVERY THING; Directory Mode - Test Directory (3 files):
 * LbxMgr.exe -d F:\devel_data\STU\MoM131_LBX\_tmp3 --dump -vvvvv
 *
 * Export BMPs; File Mode
 * LbxMgr.exe -f F:\devel_data\STU\MoM131_LBX\MAIN.LBX --export-bmp -vvvvv > LbxMgr.log
 *
 * LbxMgr.exe -f F:\devel_data\STU\MoM131_LBX\MAIN.LBX --export-bmp
 *
 */

#include <iostream>             // std:: cout, endl; FILE;
// #include <string.h>          // memcpy(), strcat(), strcpy(), strncpy(), strlen();
#include <cstring>
#include <iomanip>              // std:: setfill, setw;
// #include <malloc.h>          // malloc(), realloc();
#include <cstdlib>              // malloc();

#include "LbxMgr.h"  // LBXMGR_DATA; LBXMGR_DEBUG_MODE;
// #include "LbxMgrProcess_v1.h"
#include "LbxMgrProcess_v2.h"
#include "LbxMgrTest.h"
#include "ATS/Util_ProgOperParm.h"
#include "ATS/ats_filenames.h"  // FILE_NAMES;



LBXMGR_DATA * lbxmgr_data;


char TEST_DIRECTORY_PATH[] = "F:\\devel_data\\STU\\MoM131_LBX\\_test";
char TEST_FILE_PATH[] = "F:\\devel_data\\STU\\MoM131_LBX\\MAIN.LBX";
int TEST_ENTRY_INDEX = 0;
char TARGET_DIRECTORY_PATH[] = "F:\\devel_data\\STU\\MoM131_LBX\\_test";
char TARGET_FILE_PATH[] = "F:\\devel_data\\STU\\MoM131_LBX\\MAIN.LBX";
char TARGET_FILE_NAME[] = "MAIN.LBX";
char TARGET_FILE_NAME_BASE[] = "MAIN";
int TARGET_ENTRY_INDEX = 0;


char path_separator[] = "\\";
char export_directory_name[] = "EXPORT";
char export_directory_name_bin[] = "BIN";
char export_directory_name_bmp[] = "BMP";
char export_directory_name_c[] = "C";
char export_directory_name_csv[] = "CSV";
char export_directory_name_gif[] = "GIF";
char export_directory_name_hex[] = "HEX";
char export_directory_name_voc[] = "VOC";
char export_directory_name_xmi[] = "XMI";
char export_file_extension_bin[] = "BIN";
char export_file_extension_bmp[] = "BMP";
char export_file_extension_c[] = "C";
char export_file_extension_csv[] = "CSV";
char export_file_extension_gif[] = "GIF";
char export_file_extension_hex[] = "HEX";
char export_file_extension_voc[] = "VOC";
char export_file_extension_xmi[] = "XMI";

int LBXMGR_DEBUG_MODE = 0;
int LBXMGR_DEBUG_VERBOSE_MODE = 0;
int LBXMGR_DEBUG_STRUGGLE_MODE = 0;
int PrOpPa_Debug_Mode = 0;
int PrOpPa_Debug_Verbose_Mode = 0;
int PrOpPa_Debug_Struggle_Mode = 0;
/*
int LBX_DEBUG_MODE = 1;
int LBX_DEBUG_VERBOSE_MODE = 0;
int LBX_DEBUG_STRUGGLE_MODE = 0;
int LBX_DEBUG_LEVEL = 0;
*/

int main(int argc, char *argv[], char *env[])
{
    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: BEGIN: main()" << std::endl;

    initialize_program(argc, argv, env);

    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: END: main()" << std::endl;

    return 0;
}

/*
 * Build-Up & Tear-Down
 */
void initialize_program(int argc, char *argv[], char *env[])
{
    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: BEGIN: initialize_program()" << std::endl;

    // TODO(JWB): try to remember, and then properly document, why this 'setbuf' is/was needed
    setbuf(stdout, nullptr);

    // argc, argv, env, cwd
    create_program(argc, argv, env);

    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: operation_mode == " << PrOpPa->operation_mode << std::endl;

    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: LBXMGR_DEBUG_MODE == " << LBXMGR_DEBUG_MODE << std::endl;
    if (LBXMGR_DEBUG_VERBOSE_MODE) std::cout << "DEBUG: LBXMGR_DEBUG_VERBOSE_MODE == " << LBXMGR_DEBUG_VERBOSE_MODE << std::endl;
    if (LBXMGR_DEBUG_STRUGGLE_MODE) std::cout << "DEBUG: LBXMGR_DEBUG_STRUGGLE_MODE == " << LBXMGR_DEBUG_STRUGGLE_MODE << std::endl;

    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: LBXMGR_OUTPUT_CSV == " << LBXMGR_OUTPUT_CSV << std::endl;
    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: LBX_EXPORT_MODE == " << LBX_EXPORT_MODE << std::endl;
    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: LBX_EXPORT_IMAGE == " << LBX_EXPORT_IMAGE << std::endl;
    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: LBX_EXPORT_BIN == " << LBX_EXPORT_BIN << std::endl;
    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: LBX_EXPORT_BMP == " << LBX_EXPORT_BMP << std::endl;
    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: LBX_EXPORT_C == " << LBX_EXPORT_C << std::endl;
    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: LBX_EXPORT_GIF == " << LBX_EXPORT_GIF << std::endl;
    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: LBX_EXPORT_HEX == " << LBX_EXPORT_HEX << std::endl;
    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: LBX_EXPORT_PNG == " << LBX_EXPORT_PNG << std::endl;


    // FILE_NAMES * filenames;
    // filenames = create_filenames();

    lbxmgr_data = (LBXMGR_DATA *)malloc(sizeof(LBXMGR_DATA));
    // lbxmgr_data->lbx_data_array = (LBX_DATA **)malloc(sizeof(LBX_DATA));
    lbxmgr_data->lbx_file_count = 0;
    lbxmgr_data->lbx_data_element_count = lbx_data_array_initial_element_count;
    lbxmgr_data->filenames = create_filenames();

    /* filenames->file_name_array = malloc(sizeof(char*) * filenames->file_name_array_element_count); */
    // lbxmgr_data->lbx_data_array = (LBX_DATA **)malloc(sizeof(*lbxmgr_data->lbx_data_array) * lbxmgr_data->lbx_data_element_count);

    switch (PrOpPa->operation_mode)
    {
        case FAILURE_MODE:
            // EXIT_FAILURE
            break;
        case TEST_MODE:
            lbxmgr_data->file_path = (char *)malloc(sizeof(char) * (strlen(PrOpPa->file_path) + 1));
            strcpy(lbxmgr_data->file_path, PrOpPa->file_path);
            if (LBXMGR_DEBUG_VERBOSE_MODE) printf("DEBUG: lbxmgr_data->file_path: %s\n", lbxmgr_data->file_path);
            ats_directory_path(lbxmgr_data->file_path, &lbxmgr_data->directory_path);
            if (LBXMGR_DEBUG_VERBOSE_MODE) printf("DEBUG: lbxmgr_data->directory_path: %s\n", lbxmgr_data->directory_path);
            lbxmgr_data->entry_number = PrOpPa->entry_index;
            if (LBXMGR_DEBUG_VERBOSE_MODE) printf("DEBUG: lbxmgr_data->entry_index: %s\n", lbxmgr_data->entry_number);

            lbxmgr_test(lbxmgr_data);
            break;
        case DIRECTORY_MODE:
            lbxmgr_data->directory_path = (char *)malloc(sizeof(char) * (strlen(PrOpPa->directory_path) + 1));
            strcpy(lbxmgr_data->directory_path, PrOpPa->directory_path);
            if (LBXMGR_DEBUG_VERBOSE_MODE) printf("DEBUG: lbxmgr_data->directory_path: %s\n", lbxmgr_data->directory_path);

            lbxmgr_process_directory_mode(lbxmgr_data);
            break;
        case FILE_MODE:
            lbxmgr_data->file_path = (char *)malloc(sizeof(char) * (strlen(PrOpPa->file_path) + 1));
            strcpy(lbxmgr_data->file_path, PrOpPa->file_path);
            if (LBXMGR_DEBUG_VERBOSE_MODE) printf("DEBUG: lbxmgr_data->file_path: %s\n", lbxmgr_data->file_path);
            ats_directory_path(lbxmgr_data->file_path, &lbxmgr_data->directory_path);
            if (LBXMGR_DEBUG_VERBOSE_MODE) printf("DEBUG: lbxmgr_data->directory_path: %s\n", lbxmgr_data->directory_path);

            lbxmgr_process_file_mode(lbxmgr_data);
            break;
        case ENTRY_MODE:
            lbxmgr_data->file_path = (char *)malloc(sizeof(char) * (strlen(PrOpPa->file_path) + 1));
            strcpy(lbxmgr_data->file_path, PrOpPa->file_path);
            if (LBXMGR_DEBUG_VERBOSE_MODE) printf("DEBUG: lbxmgr_data->file_path: %s\n", lbxmgr_data->file_path);
            ats_directory_path(lbxmgr_data->file_path, &lbxmgr_data->directory_path);
            if (LBXMGR_DEBUG_VERBOSE_MODE) printf("DEBUG: lbxmgr_data->directory_path: %s\n", lbxmgr_data->directory_path);
            lbxmgr_data->entry_number = PrOpPa->entry_index;
            if (LBXMGR_DEBUG_VERBOSE_MODE) printf("DEBUG: lbxmgr_data->entry_index: %s\n", lbxmgr_data->entry_number);

            lbxmgr_process_entry_mode(lbxmgr_data);
            break;
        case MENU_MODE:
            // LbxMgrMenu();
            std::cout << "This feature not yet implemented." << std::endl;
            break;
        default:
            // TODO(JWB): refactor the API to make help available here
            // display_program_usage_help()
            // EXIT_FAILURE
            break;
    }

    destroy_program();

    if (LBXMGR_DEBUG_MODE) std::cout << "DEBUG: END: initialize_program()" << std::endl;
}
