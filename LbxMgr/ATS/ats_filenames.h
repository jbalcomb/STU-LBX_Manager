#ifndef ATS_FILENAMES_H
#define ATS_FILENAMES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct s_FILE_NAMES FILE_NAMES;

/* Creation and Destruction. */
FILE_NAMES * create_filenames();
void destroy_filenames(FILE_NAMES * filenames);

/* Access Operations */
char ** get_filenames(FILE_NAMES* filenames);
char * get_filename(FILE_NAMES * filenames, size_t index);
char * get_file_path(FILE_NAMES * filenames, size_t index);
char * get_directory_path(FILE_NAMES * filenames);

/* Size Operations */
int get_filenames_element_count(FILE_NAMES * filenames);
int set_filenames_element_count(FILE_NAMES * filenames, size_t element_count);
int get_file_name_count(FILE_NAMES * filenames);

/* Populate Operations */
void populate_filenames_from_directory_path(FILE_NAMES * filenames, char * directory_path);
void populate_filenames_from_file_path(FILE_NAMES * filenames, char * file_path);

/* Output||Display||Print Operations */
void print_filenames(FILE_NAMES * filenames);

#ifdef __cplusplus
}
#endif

#endif  /* ATS_FILENAMES_H */
