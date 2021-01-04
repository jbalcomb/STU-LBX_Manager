#ifndef LBXMGR_PROCESS_H
#define LBXMGR_PROCESS_H

#include "ATS/ats_filenames.h"

void lbxmgr_process_directory_mode(FILE_NAMES * filenames);
void lbxmgr_process_file_mode(FILE_NAMES * filenames);
void lbxmgr_process_entry_mode(FILE_NAMES * filenames);

void lbxmgr_process_lbx(FILE_NAMES * filenames);

#endif  // LBXMGR_PROCESS_H
