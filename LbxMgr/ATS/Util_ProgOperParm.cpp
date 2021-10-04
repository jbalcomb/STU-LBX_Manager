/*
 * Program:
 *   Create, Configure, Destroy
 * Configuration:
 *   Command-Line Arguments <- Environment Variables <- Configuration Files
 */
#include <iostream>

#include "Util_ProgOperParm.h"
#include "Util_CmdLnArg.h"

PROGRAM_OPERATIONAL_PARAMETERS * PrOpPa;

int create_program(int argc, char * argv[], char * env[])
{
    if (PrOpPa_Debug_Mode) std::cout << "DEBUG: BEGIN: create_program()" << std::endl;

    std::cout << "Creating Program..." << std::endl;

    PrOpPa = (PROGRAM_OPERATIONAL_PARAMETERS *)malloc(sizeof(PROGRAM_OPERATIONAL_PARAMETERS));

    PrOpPa->DEBUG_MODE = 0;
    PrOpPa->DEBUG_VERBOSE_MODE = 0;
    PrOpPa->DEBUG_STRUGGLE_MODE = 0;

    if (PrOpPa_Debug_Mode == 1)
    {
        set_debug_mode();
    }
    if (PrOpPa_Debug_Verbose_Mode == 1)
    {
        set_debug_verbose_mode();
    }

    PrOpPa->operation_mode = FAILURE_MODE;

    // SIGSEGV ats_get_current_working_directory(PrOpPa->current_working_directory);

    process_configuration_file();

    process_environment_variables();

    process_command_line_arguments(argc, argv);

    std::cout << "Program Created." << std::endl;

    if (PrOpPa_Debug_Mode) std::cout << "DEBUG: END: create_program()" << std::endl;
    return 0;
}

int destroy_program()
{
    if (PrOpPa_Debug_Mode) std::cout << "DEBUG: BEGIN: destroy_program()" << std::endl;

    std::cout << "Destroying Program..." << std::endl;

    free(PrOpPa);

    std::cout << "Program Destroyed." << std::endl;

    if (PrOpPa_Debug_Mode) std::cout << "DEBUG: END: destroy_program()" << std::endl;
    return 0;
}

int process_configuration_file()
{
    if (PrOpPa_Debug_Mode) std::cout << "DEBUG: BEGIN: process_configuration_file()" << std::endl;

    if (PrOpPa_Debug_Mode) std::cout << "DEBUG: END: process_configuration_file()" << std::endl;
    return 0;
}

int process_environment_variables()
{
    if (PrOpPa_Debug_Mode) std::cout << "DEBUG: BEGIN: process_environment_variables()" << std::endl;

    if (PrOpPa_Debug_Mode) std::cout << "DEBUG: END: process_environment_variables()" << std::endl;
    return 0;
}

void set_debug_mode()
{
    if (PrOpPa_Debug_Mode) std::cout << "DEBUG: BEGIN: set_debug_mode()" << std::endl;

    PrOpPa->DEBUG_MODE = 1;

    std::cout << "DEBUG: Debug Mode Enabled" << std::endl;

    if (PrOpPa_Debug_Mode) std::cout << "DEBUG: END: set_debug_mode()" << std::endl;
}

void set_debug_verbose_mode()
{
    if (PrOpPa_Debug_Mode) std::cout << "DEBUG: BEGIN: set_debug_verbose_mode()" << std::endl;

    PrOpPa->DEBUG_VERBOSE_MODE = 1;

    std::cout << "DEBUG: Debug Verbose Mode Enabled" << std::endl;

    if (PrOpPa_Debug_Mode) std::cout << "DEBUG: END: set_debug_verbose_mode()" << std::endl;
}

void set_debug_struggle_mode()
{
    if (PrOpPa_Debug_Mode) std::cout << "DEBUG: BEGIN: set_debug_struggle_mode()" << std::endl;

    PrOpPa->DEBUG_STRUGGLE_MODE = 1;

    std::cout << "DEBUG: Debug Struggle Mode Enabled" << std::endl;

    if (PrOpPa_Debug_Mode) std::cout << "DEBUG: END: set_debug_struggle_mode()" << std::endl;
}
