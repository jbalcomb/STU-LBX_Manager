#include <malloc.h>		/* malloc() */
#include <stdio.h>		/* printf() */
#include <string.h>		/* strcpy(), strlen() */
#include <windows.h>	/* FindFirstFile, FindNextFile, HANDLE, INVALID_HANDLE_VALUE, WIN32_FIND_DATA */

#include "ats_filenames.h"
#include "ats.h"
#include "ats_filesystem.h"


struct s_FILE_NAMES
{
    char * directory_path;
    int file_name_count;
    int file_name_array_element_count;
    char ** file_name_array;
};

static const size_t InitialElementCount = 1024;  /* initial capacity of array elements */
static const float GrowthRate = 1.6f;  /* geometric growth of array capacity */

/*
	Creation and Destruction Operations
*/

FILE_NAMES * create_filenames()
{

    FILE_NAMES * filenames;
    filenames = malloc(sizeof(FILE_NAMES));

    filenames->file_name_count = 0;
    filenames->file_name_array_element_count = InitialElementCount;

    /* filenames->file_name_array = malloc(sizeof(char*) * filenames->file_name_array_element_count); */
    filenames->file_name_array = malloc(sizeof(*filenames->file_name_array) * filenames->file_name_array_element_count);

    return filenames;
}

void destroy_filenames(FILE_NAMES * filenames)
/* Release memory owned by the array. */
{

    // for (int itr = 0; itr <= filenames->file_name_array_element_count; itr++)
    // {
    // 	free(filenames->file_name_array[itr]);
    // }
    free(filenames->file_name_array);
    free(filenames);

}

/*
	Access Operations
*/

char ** get_filenames(FILE_NAMES * filenames)
{
    return filenames->file_name_array;
}

char * get_filename(FILE_NAMES * filenames, size_t index)
{

    /* if (DEBUG_VERBOSE_MODE) printf("DEBUG: index: %lu\n", index); */

    /* if (DEBUG_VERBOSE_MODE) printf("DEBUG: filenames->file_name_array[%lu]: %s\n", index, filenames->file_name_array[index]); */

    return filenames->file_name_array[index];
}

char * get_file_path(FILE_NAMES * filenames, size_t index)
{

    char * file_path_name;
    file_path_name = malloc(sizeof(char) * (strlen(filenames->directory_path) + strlen("\\") + strlen(filenames->file_name_array[index]) + 1));

    strcpy(file_path_name, filenames->directory_path);
    strcat(file_path_name, "\\");
    strcat(file_path_name, filenames->file_name_array[index]);

    return file_path_name;
}

char * get_directory_path(FILE_NAMES * filenames)
{
    return filenames->directory_path;
}

/*
	Size Operations
*/

int get_filenames_element_count(FILE_NAMES * filenames)
{
    return filenames->file_name_array_element_count;
}

int set_filenames_element_count(FILE_NAMES * filenames, size_t element_count)
{
    filenames->file_name_array_element_count = element_count;
    filenames->file_name_array = realloc(filenames->file_name_array, filenames->file_name_array_element_count);
    return filenames->file_name_array_element_count;
}

int get_file_name_count(FILE_NAMES * filenames)
{
    return filenames->file_name_count;
}

/*
	Populate Operations
*/

void populate_filenames_from_file_path(FILE_NAMES * filenames, char * file_path)
{

    char ** tmp_filenames_array;
    char * tmp_directory_path;
    char * tmp_file_name;

    ats_directory_path(file_path, &filenames->directory_path);

    filenames->file_name_count = 0;
    /*
    filenames->file_name_array_element_count = 1;
    tmp_filenames_array = realloc(filenames->file_name_array, sizeof(*filenames->file_name_array) * filenames->file_name_array_element_count);
    if (tmp_filenames_array == NULL)
    {
        printf("FAILURE: tmp_filenames_array = realloc(filenames->file_name_array, sizeof(*filenames->file_name_array) *  filenames->file_name_array_element_count)\n");
    }
    else
    {
        filenames->file_name_array = tmp_filenames_array;
    }
    */

    // ats_file_name(file_path, &filenames->file_name_array[filenames->file_name_count]);
    ats_file_name(file_path, &tmp_file_name);

    filenames->file_name_array[filenames->file_name_count] = malloc(sizeof(char) * (strlen(tmp_file_name) + 1));
    strcpy(filenames->file_name_array[filenames->file_name_count], tmp_file_name);

    filenames->file_name_count = filenames->file_name_count + 1;

}

void populate_filenames_from_directory_path(FILE_NAMES * filenames, char * directory_path)
{

    char * tmp_directory_path;
    char ** tmp_filenames_array;
    WIN32_FIND_DATAA ffdFindData;
    HANDLE hdlFindFile;
    DWORD dwError=0;

    filenames->directory_path = directory_path;


    /* filenames->file_name_array = malloc(sizeof(char*) * filenames->file_name_array_element_count); */

    if (strlen(filenames->directory_path) > (MAX_PATH - 3))
    {
        printf("\nDirectory path is too long.\n");
        exit(1);
    }

    tmp_directory_path = malloc(sizeof(char) * (strlen(filenames->directory_path) + 3));
    strcpy(tmp_directory_path, filenames->directory_path);
    strcat(tmp_directory_path, "\\*");

    hdlFindFile = FindFirstFile(tmp_directory_path, &ffdFindData);

    if (hdlFindFile == INVALID_HANDLE_VALUE)
    {
        printf("FAILURE: FindFirstFile: hdlFindFile == INVALID_HANDLE_VALUE\n");
        exit(1);
    }

    do
    {
        /* if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) */
        if (ffdFindData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)  /* winnt.h: #define FILE_ATTRIBUTE_DIRECTORY 0x00000010 */
        {

            if (filenames->file_name_count > filenames->file_name_array_element_count)
            {
                /* TODO(JWB): add code to calculate, realloc, test, and assign */

                filenames->file_name_array_element_count = (int)(filenames->file_name_array_element_count * GrowthRate + 1);

                tmp_filenames_array = realloc(filenames->file_name_array, sizeof(*filenames->file_name_array) * filenames->file_name_array_element_count);
                if (tmp_filenames_array == NULL)
                {
                    printf("FAILURE: tmp_filenames_array = realloc(filenames->file_name_array, sizeof(*filenames->file_name_array) *  filenames->file_name_array_element_count)\n");
                }
                else
                {
                    filenames->file_name_array = tmp_filenames_array;
                }
            }

            filenames->file_name_array[filenames->file_name_count] = malloc(sizeof(char) * (strlen(ffdFindData.cFileName)) + 1);
            strcpy(filenames->file_name_array[filenames->file_name_count], ffdFindData.cFileName);

            filenames->file_name_count = filenames->file_name_count + 1;
        }
    }
    while (FindNextFile(hdlFindFile, &ffdFindData) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        printf("dwError: %lu", dwError);
    }

    free(tmp_directory_path);
    FindClose(hdlFindFile);

}


/*
	Output||Display||Print Operations
*/

void print_filenames(FILE_NAMES * filenames)
{

    for (int itr = 0; itr <= filenames->file_name_count - 1; itr++)
    {
        printf("filenames->file_name_array[%d]: %s\n", itr, filenames->file_name_array[itr]);
    }

}
