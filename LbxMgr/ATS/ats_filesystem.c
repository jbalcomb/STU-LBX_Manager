/*
 * Directory Path: C:\devel_data\STU\MoM131_LBX
 * File Path: C:\devel_data\STU\MoM131_LBX\MAGIC.LBX
 * Directory Name: MoM131_LBX
 * File Name: MAGIC.LBX
 * File Name Base: MAGIC
 * File Name Extension: LBX
 *
 */

/*
 * getcwd()
 * char *_getcwd(char *buffer, int maxlen);
 * wchar_t *_wgetcwd(wchar_t *buffer, int maxlen);
 * Gets the current working directory.
 * Parameters
 * buffer
 * Storage location for the path.
 * maxlen
 * Maximum length of the path in characters: char for _getcwd and wchar_t for _wgetcwd.
 * Return Value
 * Returns a pointer to buffer.
 * A NULL return value indicates an error, and errno is set either to
 * ENOMEM, indicating that there is insufficient memory to allocate maxlen bytes (when a NULL argument is given as buffer), or to
 * ERANGE, indicating that the path is longer than maxlen characters.
 */

#include <malloc.h>         /* malloc(), realloc(); */
#include <stdio.h>      /* fclose(), fopen(), fread(), fwrite(), printf(); FILE */
#include <stdlib.h>     /* splitpath(); EXIT_FAILURE */
#include <sys/stat.h>   /* fstat(); stat; */
#include <time.h>  /* ctime() */
#include <locale.h>
/* #include <langinfo.h> */

#include "ats_filesystem.h"
#include "ats.h"


/*
 * getcwd()
 * ? io.h OR unistd.h ?
 * Linux (glibc), get_current_dir_name() does its own malloc
 * ...just the directory name, not the entire path, basename(get_current_dir_name())
 */
void ats_get_current_working_directory(char * current_working_directory_out)
{

    char tmp_current_working_directory_out[256];

    getcwd(tmp_current_working_directory_out, 256);

    current_working_directory_out = malloc(sizeof(char) * (strlen(tmp_current_working_directory_out) + 1));
    strncpy(current_working_directory_out, tmp_current_working_directory_out, 255);

}

int ats_create_directory(char * directory_path)
{

    mkdir(directory_path);
    /* stat.h: int	mkdir (const char *_path, mode_t __mode ); */
    /* mkdir(directory_path, S_IRWXU | S_IRWXG | S_IRWXO); */

    return 0;
}

void ats_directory_path_and_file_name(char * file_path_in, char * directory_path_out, char * file_name_out)
{

    char tmp_windows_splitpath_path[MAX_PATH];
    char tmp_windows_splitpath_drive[MAX_DRIVE];
    char tmp_windows_splitpath_dir[MAX_DIR];
    char tmp_windows_splitpath_fname[MAX_FNAME];
    char tmp_windows_splitpath_ext[MAX_EXT];
    char * tmp_linux_basename;
    char * tmp_file_path;
    char * tmp_file_name;

    _splitpath(file_path_in,
               tmp_windows_splitpath_drive,
               tmp_windows_splitpath_dir,
               tmp_windows_splitpath_fname,
               tmp_windows_splitpath_ext);


    tmp_file_path = malloc(sizeof(char) * (strlen(tmp_windows_splitpath_drive) + strlen(tmp_windows_splitpath_dir) + 1));
    strcpy(tmp_file_path, tmp_windows_splitpath_drive);
    strcat(tmp_file_path, tmp_windows_splitpath_dir);

    tmp_file_name = malloc(sizeof(char) * (strlen(tmp_windows_splitpath_fname) + strlen(tmp_windows_splitpath_ext) + 1));
    strcpy(tmp_file_name, tmp_windows_splitpath_fname);
    strcat(tmp_file_name, tmp_windows_splitpath_ext);

    directory_path_out = tmp_file_path;
    file_name_out = tmp_file_name;

}

void ats_directory_path(char * file_path_in, char ** directory_path_out)
{

    char tmp_windows_splitpath_drive[MAX_DRIVE];
    char tmp_windows_splitpath_dir[MAX_DIR];
    char tmp_windows_splitpath_fname[MAX_FNAME];
    char tmp_windows_splitpath_ext[MAX_EXT];
    char * tmp_directory_path;

    _splitpath(file_path_in,
               tmp_windows_splitpath_drive,
               tmp_windows_splitpath_dir,
               tmp_windows_splitpath_fname,
               tmp_windows_splitpath_ext);

    tmp_directory_path = malloc(sizeof(char) * (strlen(tmp_windows_splitpath_drive) + strlen(tmp_windows_splitpath_dir) + 1));
    strcpy(tmp_directory_path, tmp_windows_splitpath_drive);
    strcat(tmp_directory_path, tmp_windows_splitpath_dir);

    *directory_path_out = tmp_directory_path;

}

void ats_file_name(char * file_path_in, char ** file_name_out)
{

    char tmp_windows_splitpath_path[MAX_PATH];
    char tmp_windows_splitpath_drive[MAX_DRIVE];
    char tmp_windows_splitpath_dir[MAX_DIR];
    char tmp_windows_splitpath_fname[MAX_FNAME];
    char tmp_windows_splitpath_ext[MAX_EXT];
    char * tmp_file_name;

    _splitpath(file_path_in,
               tmp_windows_splitpath_drive,
               tmp_windows_splitpath_dir,
               tmp_windows_splitpath_fname,
               tmp_windows_splitpath_ext);

    tmp_file_name = malloc(sizeof(char) * (strlen(tmp_windows_splitpath_fname) + strlen(tmp_windows_splitpath_ext) + 1));
    strcpy(tmp_file_name, tmp_windows_splitpath_fname);
    strcat(tmp_file_name, tmp_windows_splitpath_ext);

    *file_name_out = tmp_file_name;

}

void ats_file_name_base_and_extension(char * file_name_in, char * file_name_base_out, char * file_name_extension_out)
{

    /*
    Not sure this is how this should/would be done,
      but I saw something like this logic in the disassembled C/C++ code of MAGIC.EXE from Master of Magic v1.31.
    Effectively, as I understand it, this just causes an /early exit/
      as any c-entry function will stop when it hits the /injected/ '\0'
      thereby /ignoring/ the balance of the c-entry.
    As seen here, it trips up the for-loop
      by placing the '\0' into the very next iteration of its sequence
    */

    int itr_file_name_base_out;
    int itr_file_name_extension_out;

    file_name_base_out = malloc(sizeof(char) * (strlen(file_name_in) + 1));
    strcpy(file_name_base_out, file_name_in);

    file_name_extension_out = malloc(sizeof(char) * (strlen(file_name_in) + 1));

    for (itr_file_name_base_out = 0; file_name_base_out[itr_file_name_base_out] != 0; itr_file_name_base_out++)
    {
        if (file_name_in[itr_file_name_base_out] == '.')
        {
            file_name_base_out[itr_file_name_base_out] = '\0';
            file_name_base_out[itr_file_name_base_out + 1] = '\0';
        }
    }

    for (itr_file_name_extension_out = (int)strlen(file_name_in); itr_file_name_extension_out > 0; itr_file_name_extension_out--)
    {
        if (file_name_in[itr_file_name_extension_out] != '.')
        {
            /* strcpy(file_name_extension_out[itr_file_name_extension_out], file_name_in[itr_file_name_extension_out]); */
        }
    }

}

void ats_file_name_base(char * file_name_in, char ** file_name_base_out)
{

    int itr_file_name_in;
    char * tmp_file_name_base_out;

    tmp_file_name_base_out = malloc(sizeof(char) * (strlen(file_name_in) + 1));
    strcpy(tmp_file_name_base_out, file_name_in);

    for (itr_file_name_in = 0; tmp_file_name_base_out[itr_file_name_in] != 0; itr_file_name_in++)
    {
        if (file_name_in[itr_file_name_in] == '.')
        {
            tmp_file_name_base_out[itr_file_name_in] = '\0';
            tmp_file_name_base_out[itr_file_name_in + 1] = '\0';
        }
    }

    *file_name_base_out = tmp_file_name_base_out;

}

char * ats_remove_extension_from_filename(char * file_name_out, char * file_name_in)
{

    int itr_file_name;

    strcpy(file_name_out, file_name_in);

    for (itr_file_name = 0; file_name_out[itr_file_name] != 0; itr_file_name++)
    {
        if (file_name_in[itr_file_name] == '.')
        {
            file_name_out[itr_file_name] = '\0';
            file_name_out[itr_file_name + 1] = '\0';
        }
    }

    return file_name_out;
}

void ats_build_file_path_and_name(char ** export_path_and_file_name, char * directory_path, char * file_name_base, char * file_name_extension)
{
    char * tmp_export_path_and_file_name;

    tmp_export_path_and_file_name = malloc(sizeof(char) * (strlen(directory_path) + strlen("\\") + strlen(file_name_base) + strlen(".") + strlen(file_name_extension) + 1));

    strcpy(tmp_export_path_and_file_name, directory_path);
    strcat(tmp_export_path_and_file_name, "\\");
    strcat(tmp_export_path_and_file_name, file_name_base);
    strcat(tmp_export_path_and_file_name, ".");
    strcat(tmp_export_path_and_file_name, file_name_extension);

    *export_path_and_file_name = tmp_export_path_and_file_name;
}

void ats_populate_file_name_structure(FILE_NAME * file_name_structure, char * file_path_and_name)
{

    _splitpath(file_path_and_name,
               file_name_structure->windows_splitpath_drive,
               file_name_structure->windows_splitpath_dir,
               file_name_structure->windows_splitpath_fname,
               file_name_structure->windows_splitpath_ext);

    file_name_structure->file_name = malloc(sizeof(char) * (strlen(file_name_structure->windows_splitpath_fname) + strlen(file_name_structure->windows_splitpath_ext) + 1));
    strcpy(file_name_structure->file_name, file_name_structure->windows_splitpath_fname);
    strcat(file_name_structure->file_name, file_name_structure->windows_splitpath_ext);

}

size_t ats_file_size_via_seek(FILE * stream_file_in)
{

    size_t tmp_pos;
    size_t file_size_seek;

    tmp_pos = ftell(stream_file_in);
    
    fseek(stream_file_in, 0, SEEK_END);
    file_size_seek = ftell(stream_file_in);
    
    fseek(stream_file_in, tmp_pos, SEEK_SET);
    
    return file_size_seek;
}

size_t ats_file_size_via_stat(FILE * stream_file_in)
{

    size_t file_size_stat;

    struct stat stat_buf;
    fstat(fileno(stream_file_in), &stat_buf);
    file_size_stat = stat_buf.st_size;

    return file_size_stat;
}

size_t ats_file_size(FILE * stream_file_in)
{

    size_t file_size;
    size_t tmp_file_size_via_seek;
    size_t tmp_file_size_via_stat;

    tmp_file_size_via_seek = ats_file_size_via_seek(stream_file_in);
    tmp_file_size_via_stat = ats_file_size_via_stat(stream_file_in);

    if (tmp_file_size_via_seek == tmp_file_size_via_stat)
    {
        file_size = tmp_file_size_via_seek;
    }
    else
    {
        exit(ATS_EXIT_FAILURE);
    }

    return file_size;
}

/*
 * https://en.wikibooks.org/wiki/C_Programming/POSIX_Reference/sys/stat.h/stat
 * time_t      st_atime;   / * time of last access * /
 * time_t      st_mtime;   / * time of last modification * /
 * time_t      st_ctime;   / * time of last status change * /
 *
 */
void ats_timestamps_via_stat(FILE * stream_file_in)
{

    time_t tmp_st_atime;        /* time of last access */
    time_t tmp_st_mtime;        /* time of last modification */
    time_t tmp_st_ctime;        /* time of last status change */
    time_t tmp_st_birthtime;    /* time of creation */

    struct stat file_stat_buf;

    fstat(fileno(stream_file_in), &file_stat_buf);

    tmp_st_atime = file_stat_buf.st_atime;
    tmp_st_mtime = file_stat_buf.st_mtime;
    tmp_st_ctime = file_stat_buf.st_ctime;
    /* tmp_st_birthtime = &file_stat_buf.st_birthtime; */

    printf(" File access time %s", ctime(&file_stat_buf.st_atime));

}

time_t * ats_earliest_timestamp_via_stat(FILE * stream_file_in)
{

    time_t tmp_st_atime;        /* time of last access */
    time_t tmp_st_mtime;        /* time of last modification */
    time_t tmp_st_ctime;        /* time of last status change */
    time_t tmp_st_birthtime;    /* time of creation */
    time_t * earliest_timestamp;
    struct tm      *tm;
    char            datestring[256];

    struct stat file_stat_buf;

    fstat(fileno(stream_file_in), &file_stat_buf);

    printf(" File access time %s", ctime(&file_stat_buf.st_atime));
    printf(" File modify time %s", ctime(&file_stat_buf.st_mtime));
    printf("File changed time %s", ctime(&file_stat_buf.st_ctime));
    /* printf("File birth time %s", ctime(&file_stat_buf.st_birthtime)); */

    tmp_st_atime = file_stat_buf.st_atime;
    tmp_st_mtime = file_stat_buf.st_mtime;
    tmp_st_ctime = file_stat_buf.st_ctime;
    /* tmp_st_birthtime = file_stat_buf.st_birthtime; */

    printf(" File access time %s", ctime(&tmp_st_atime));
    printf(" File modify time %s", ctime(&tmp_st_mtime));
    printf("File changed time %s", ctime(&tmp_st_ctime));
    /* printf("File birth time %s", ctime(&tmp_st_birthtime)); */

    if (&tmp_st_atime < &tmp_st_mtime)
    {
        earliest_timestamp = &file_stat_buf.st_atime;
    }
    else
    {
        earliest_timestamp = &file_stat_buf.st_mtime;
    }
    if (&file_stat_buf.st_ctime < earliest_timestamp)
    {
        earliest_timestamp = &file_stat_buf.st_ctime;
    }

    /* Get localized date string. */
    tm = localtime(&file_stat_buf.st_mtime);
    /* strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm); */

    return earliest_timestamp;
}
