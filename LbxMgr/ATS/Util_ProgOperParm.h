// Program Operational Parameters (AKA ProgOperParm, PrOpPa, Proppa, proppa)

#ifndef __PROGRAM_OPERATIONAL_PARAMETER__
#define __PROGRAM_OPERATIONAL_PARAMETER__

extern int PrOpPa_Debug_Mode;
extern int PrOpPa_Debug_Verbose_Mode;

#define COMMAND_LINE_ARGUMENT_COUNT_MAXIMUM 3
#define COMMAND_LINE_ARGUMENT_COUNT_MINIMUM 3

/*
 * -d, --directory
 * -e, --entry
 * -f, --file
 * -m, --menu
 */
const char COMMAND_LINE_ARGUMENT_SWITCH_OPTIONS[4][3] = {"-d", "-e", "-f", "-m"};

enum operation_mode
{
	FAILURE_MODE = 0,
	DIRECTORY_MODE = 1,
	FILE_MODE = 2,
	ENTRY_MODE = 3,
	MENU_MODE = 4
};

struct s_PROGRAM_OPERATIONAL_PARAMETERS
{
	int DEBUG_MODE;
	int DEBUG_VERBOSE_MODE;
	char * current_working_directory;
	int operation_mode;
	char * directory_path;
	char * file_path;
	int entry_number;
};
typedef struct s_PROGRAM_OPERATIONAL_PARAMETERS PROGRAM_OPERATIONAL_PARAMETERS;
extern PROGRAM_OPERATIONAL_PARAMETERS* PrOpPa;

int create_program(int argc, char *argv[], char *env[]);
int destroy_program();
int process_configuration_file();
int process_environment_variables();
void set_debug_mode();
void set_debug_verbose_mode();

#endif  // __PROGRAM_OPERATIONAL_PARAMETER__
