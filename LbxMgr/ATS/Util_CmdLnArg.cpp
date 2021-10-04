#include <iostream>   // std::cerr, cin, cout, endl
#include <string>     // std::entry // overload for << that supports std::cout << "Some String: " << std::entry strSearchPath = "" << std::endl;
// #include <string.h>    // strcpy_s, strcat_s
#include <cstring>

#include "Util_ProgOperParm.h"
#include "../LbxMgr.h"

int LBXMGR_OUTPUT_CSV;
int LBX_EXPORT_MODE;
int LBX_EXPORT_GRAPHICS;
int LBX_EXPORT_SOUNDS;
int LBX_EXPORT_ANIMATION;
int LBX_EXPORT_IMAGE;
int LBX_EXPORT_MUSIC;
int LBX_EXPORT_SOUNDFX;
int LBX_EXPORT_BIN;
int LBX_EXPORT_C;
int LBX_EXPORT_HEX;
int LBX_EXPORT_BMP;
int LBX_EXPORT_FLI;
int LBX_EXPORT_FLC;
int LBX_EXPORT_GIF;
int LBX_EXPORT_PNG;
int LBX_EXPORT_COL;
int LBX_EXPORT_PAL;
int LBX_EXPORT_FNT;
int LBX_EXPORT_MID;
int LBX_EXPORT_VOC;
int LBX_EXPORT_WAV;
int LBX_EXPORT_XMI;


void display_program_usage_help()
{
	if (PrOpPa->DEBUG_MODE) std::cout << "DEBUG: BEGIN: display_program_usage_help()" << std::endl;

	const char* program_usage_help_text = "Usage: LbxMgr.exe -f <file_path_and_name>\n";
	std::cout << program_usage_help_text << std::endl;

	if (PrOpPa->DEBUG_MODE) std::cout << "DEBUG: END: display_program_usage_help()" << std::endl;
	exit(1);
}

int process_command_line_arguments(int argc, char* argv[])
{
	if (PrOpPa->DEBUG_MODE) std::cout << "DEBUG: BEGIN: process_command_line_arguments()" << std::endl;

    int itr_argc;

	if (PrOpPa->DEBUG_VERBOSE_MODE)
	{
        std::cout << "DEBUG: Outputting all command-line-arguments..." << std::endl;
		for (itr_argc = 0; itr_argc < argc; ++itr_argc)
		{
			std::cout << "DEBUG: argv[" << itr_argc << "]: " << argv[itr_argc] << std::endl;
		}
	}

	if (argc < 1)
	{
		if (PrOpPa->DEBUG_VERBOSE_MODE) std::cout << "if (argc < 1)" << std::endl;
		display_program_usage_help();
	}

    if (argc < 2)
    {
        if (PrOpPa->DEBUG_VERBOSE_MODE) std::cout << "if (argc < 2)" << std::endl;
        display_program_usage_help();
    }

    if (strcmp(argv[1], "--test") == 0)
    {
        if (PrOpPa->DEBUG_VERBOSE_MODE) std::cout << "DEBUG: strcmp(argv[1], \"--test\") == 0" << std::endl;
        PrOpPa->operation_mode = TEST_MODE;
        PrOpPa->directory_path = TEST_DIRECTORY_PATH;
        PrOpPa->file_path = TEST_FILE_PATH;
        PrOpPa->entry_index = TEST_ENTRY_INDEX;
        if (PrOpPa->DEBUG_MODE) std::cout << "DEBUG: PrOpPa->directory_path: " << PrOpPa->directory_path << std::endl;
        if (PrOpPa->DEBUG_MODE) std::cout << "DEBUG: PrOpPa->file_path: " << PrOpPa->file_path << std::endl;
        if (PrOpPa->DEBUG_MODE) std::cout << "DEBUG: PrOpPa->entry_index: " << PrOpPa->entry_index << std::endl;

        LBXMGR_OUTPUT_CSV = 1;

        LBX_EXPORT_MODE = 1;

        LBX_EXPORT_BIN = 1;
        LBX_EXPORT_C = 1;
        LBX_EXPORT_HEX = 1;

        LBX_EXPORT_BMP = 1;
        LBX_EXPORT_FLC = 1;
        LBX_EXPORT_FLI = 1;
        LBX_EXPORT_GIF = 1;
        LBX_EXPORT_PNG = 1;

        LBX_EXPORT_MID = 1;
        LBX_EXPORT_VOC = 1;
        LBX_EXPORT_XMI = 1;
        LBX_EXPORT_WAV = 1;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        if (PrOpPa->DEBUG_VERBOSE_MODE) std::cout << "DEBUG: strcmp(argv[1], \"-d\") == 0" << std::endl;
        PrOpPa->operation_mode = DIRECTORY_MODE;
		PrOpPa->directory_path = argv[2];
        if (PrOpPa->DEBUG_MODE) std::cout << "DEBUG: PrOpPa->directory_path: " << PrOpPa->directory_path << std::endl;
	}
    else if (strcmp(argv[1], "-f") == 0)
	{
		if (PrOpPa->DEBUG_VERBOSE_MODE) std::cout << "DEBUG: strcmp(argv[1], \"-f\") == 0" << std::endl;
        PrOpPa->operation_mode = FILE_MODE;
        /*
		PrOpPa->file_path_and_name = (char*)malloc(strlen(argv[2]));
		strcpy(PrOpPa->file_path_and_name, argv[2]);
		*/
        if (PrOpPa->DEBUG_VERBOSE_MODE) std::cout << "DEBUG: argv[2]: " << argv[2] << std::endl;
        if (PrOpPa->DEBUG_VERBOSE_MODE) std::cout << "DEBUG: strlen(argv[2]): " << strlen(argv[2]) << std::endl;
        PrOpPa->file_path = argv[2];
		if (PrOpPa->DEBUG_MODE) std::cout << "DEBUG: PrOpPa->file_path: " << PrOpPa->file_path << std::endl;
	}
    else if (strcmp(argv[1], "-e") == 0)
    {
        if (PrOpPa->DEBUG_VERBOSE_MODE) std::cout << "DEBUG: strcmp(argv[1], \"-e\") == 0" << std::endl;
        PrOpPa->operation_mode = ENTRY_MODE;
        /*
		PrOpPa->file_path_and_name = (char*)malloc(strlen(argv[2]));
		strcpy(PrOpPa->file_path_and_name, argv[2]);
		*/
        if (PrOpPa->DEBUG_VERBOSE_MODE) std::cout << "DEBUG: argv[2]: " << argv[2] << std::endl;
        if (PrOpPa->DEBUG_VERBOSE_MODE) std::cout << "DEBUG: strlen(argv[2]): " << strlen(argv[2]) << std::endl;
        PrOpPa->file_path = argv[2];
        if (PrOpPa->DEBUG_MODE) std::cout << "DEBUG: PrOpPa->file_path: " << PrOpPa->file_path << std::endl;
        PrOpPa->entry_index = (int)argv[3];
        if (PrOpPa->DEBUG_MODE) std::cout << "DEBUG: PrOpPa->entry_index: " << PrOpPa->entry_index << std::endl;
    }
    else if (strcmp(argv[1], "-m") == 0)
	{
        if (PrOpPa->DEBUG_VERBOSE_MODE) std::cout << "DEBUG: strcmp(argv[1], \"-m\") == 0" << std::endl;
        PrOpPa->operation_mode = MENU_MODE;
	}
    else
	{
        PrOpPa->operation_mode = FAILURE_MODE;
		display_program_usage_help();
	}

    for (itr_argc = 0; itr_argc < argc; ++itr_argc)
    {


        if (strcmp(argv[itr_argc], "--dump") == 0)
        {
            LBXMGR_OUTPUT_CSV = 1;
            LBX_EXPORT_MODE = 1;
            LBX_EXPORT_BIN = 1;
            LBX_EXPORT_BMP = 1;
            LBX_EXPORT_C = 1;
            LBX_EXPORT_GIF = 1;
            LBX_EXPORT_HEX = 1;
            LBX_EXPORT_PNG = 1;
            LBX_EXPORT_VOC = 1;
            LBX_EXPORT_XMI = 1;
        }

        if (strcmp(argv[itr_argc], "--output-csv") == 0)
        {
            LBXMGR_OUTPUT_CSV = 1;
        }
        if (strcmp(argv[itr_argc], "--export-bin") == 0)
        {
            LBX_EXPORT_MODE = 1;
            LBX_EXPORT_BIN = 1;
        }
        if (strcmp(argv[itr_argc], "--export-bmp") == 0)
        {
            LBX_EXPORT_MODE = 1;
            LBX_EXPORT_BMP = 1;
        }
        if (strcmp(argv[itr_argc], "--export-c") == 0)
        {
            LBX_EXPORT_MODE = 1;
            LBX_EXPORT_C = 1;
        }
        if (strcmp(argv[itr_argc], "--export-gif") == 0)
        {
            LBX_EXPORT_MODE = 1;
            LBX_EXPORT_GIF = 1;
        }
        if (strcmp(argv[itr_argc], "--export-hex") == 0)
        {
            LBX_EXPORT_MODE = 1;
            LBX_EXPORT_HEX = 1;
        }
        if (strcmp(argv[itr_argc], "--export-png") == 0)
        {
            LBX_EXPORT_MODE = 1;
            LBX_EXPORT_PNG = 1;
        }
        if (strcmp(argv[itr_argc], "--export-voc") == 0)
        {
            LBX_EXPORT_MODE = 1;
            LBX_EXPORT_VOC = 1;
        }
        if (strcmp(argv[itr_argc], "--export-xmi") == 0)
        {
            LBX_EXPORT_MODE = 1;
            LBX_EXPORT_XMI = 1;
        }

        if (strcmp(argv[itr_argc], "-v") == 0)
        {
            printf("DEBUG: Debug Level set to 1 (App: Debug; Lib: N/A)\n");
            LBXMGR_DEBUG_MODE = 1;
            set_debug_mode();
        }
        if (strcmp(argv[itr_argc], "-vv") == 0)
        {
            printf("DEBUG: Debug Level set to 2 (App: Verbose; Lib: Debug)\n");
            LBXMGR_DEBUG_MODE = 1;
            LBXMGR_DEBUG_VERBOSE_MODE = 1;
            set_debug_mode();
            lbx_set_debug_mode();
        }
        if (strcmp(argv[itr_argc], "-vvv") == 0)
        {
            printf("DEBUG: Debug Level set to 3 (App: Verbose; Lib: Verbose)\n");
            LBXMGR_DEBUG_MODE = 1;
            LBXMGR_DEBUG_VERBOSE_MODE = 1;
            set_debug_mode();
            set_debug_verbose_mode();
            lbx_set_debug_mode();
            lbx_set_debug_verbose_mode();
        }
        if (strcmp(argv[itr_argc], "-vvvv") == 0)
        {
            printf("DEBUG: Debug Level set to 4 (App: Struggle; Lib: Verbose)\n");
            LBXMGR_DEBUG_MODE = 1;
            LBXMGR_DEBUG_VERBOSE_MODE = 1;
            LBXMGR_DEBUG_STRUGGLE_MODE = 1;
            set_debug_mode();
            set_debug_verbose_mode();
            set_debug_struggle_mode();
            lbx_set_debug_mode();
            lbx_set_debug_verbose_mode();
        }
        if (strcmp(argv[itr_argc], "-vvvvv") == 0)
        {
            printf("DEBUG: Debug Level set to 5 (App: Struggle; Lib: Struggle)\n");
            LBXMGR_DEBUG_MODE = 1;
            LBXMGR_DEBUG_VERBOSE_MODE = 1;
            LBXMGR_DEBUG_STRUGGLE_MODE = 1;
            lbx_set_debug_mode();
            lbx_set_debug_verbose_mode();
            lbx_set_debug_struggle_mode();
        }

    }

    if (PrOpPa->DEBUG_MODE) std::cout << "DEBUG: END: process_command_line_arguments()" << std::endl;
	return 0;
}
