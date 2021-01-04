#ifndef ATS_FILESYSTEM_H
#define ATS_FILESYSTEM_H

/* SEE: stdlib.h */
#define MAX_PATH 260
#define MAX_DRIVE 3
#define MAX_DIR 256
#define MAX_FNAME 256
#define MAX_EXT 256

/* SEE: stdlib.h */
#define ATS_EXIT_SUCCESS 0
#define ATS_EXIT_FAILURE 1

/* SEE: _mingw_stat64.h */
/*
struct ats_stat {
    _dev_t st_dev;
    _ino_t st_ino;
    unsigned short st_mode;
    short st_nlink;
    short st_uid;
    short st_gid;
    _dev_t st_rdev;
    _off_t st_size;
    time_t st_atime;
    time_t st_mtime;
    time_t st_ctime;
};
*/
/*
struct ats_stat_all {
    _dev_t st_dev;
    _ino_t st_ino;
    unsigned short st_mode;
    short st_nlink;
    short st_uid;
    short st_gid;
    _dev_t st_rdev;
    _off_t st_size;
    time_t st_atime;
    time_t st_mtime;
    time_t st_ctime;
    time_t st_birthtime; / * DNE * /
};
*/

struct _FILE_NAME {
    char * file_path_and_name;
    char windows_splitpath_path[MAX_PATH];
    char windows_splitpath_drive[MAX_DRIVE];
    char windows_splitpath_dir[MAX_DIR];
    char windows_splitpath_fname[MAX_FNAME];
    char windows_splitpath_ext[MAX_EXT];
    char * linux_basename;
    char * file_name;
    char * file_name_base;
};
typedef struct _FILE_NAME FILE_NAME;


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void ats_get_current_working_directory(char * current_working_directory_out);
int ats_create_directory(char * directory_path);

/* void ats_splitpath(char * file_path_and_name, char * file_path_out, char * file_name_out); */
void ats_directory_path_and_file_name(char * file_path_in, char * directory_path_out, char * file_name_out);
void ats_directory_path(char * file_path_in, char ** directory_path_out);
void ats_file_name(char * file_path_in, char ** file_name_out);
void ats_file_name_base_and_extension(char * file_name_in, char * file_name_base_out, char * file_name_extension_out);
void ats_file_name_base(char * file_name_in, char ** file_name_base_out);
char * ats_remove_extension_from_filename(char * file_name_out, char * file_name_in);
void ats_build_file_path_and_name(char ** export_path_and_file_name, char * directory_path, char * file_name_base, char * file_name_extension);

void ats_populate_file_name_structure(FILE_NAME * file_name_structure, char * file_path_and_name);

size_t ats_file_size_via_seek(FILE * stream_file_in);
size_t ats_file_size_via_stat(FILE * stream_file_in);
size_t ats_file_size(FILE * stream_file_in);

time_t * ats_earliest_timestamp_via_stat(FILE * stream_file_in);

#ifdef __cplusplus
}
#endif

#endif  /* ATS_FILESYSTEM_H */
