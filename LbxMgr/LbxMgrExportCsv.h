#ifndef LBXMGR_EXPORT_CSV_H
#define LBXMGR_EXPORT_CSV_H

#include "LbxMgr.h"  // LBXMGR_DATA; LBXMGR_DEBUG_MODE;
#include "lib_lbx.h"  // LBX_DATA

void lbxmgr_output_to_csv(LBX_DATA * lbx);
void lbxmgr_output_csv_lbx_files(LBXMGR_DATA * lbxmgr_data);
void lbxmgr_output_csv_lbx_records(LBXMGR_DATA * lbxmgr_data);

#endif  // LBXMGR_EXPORT_CSV_H
