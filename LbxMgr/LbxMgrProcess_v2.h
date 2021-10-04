#ifndef LBXMGR_PROCESS_H
#define LBXMGR_PROCESS_H

#include "LbxMgr.h"  // LBXMGR_DATA

void lbxmgr_process_directory_mode(LBXMGR_DATA * lbxmgr_data);
void lbxmgr_process_file_mode(LBXMGR_DATA * lbxmgr_data);
void lbxmgr_process_entry_mode(LBXMGR_DATA * lbxmgr_data);

void lbxmgr_process_lbx_files(LBXMGR_DATA * lbxmgr_data);

#endif  // LBXMGR_PROCESS_H
