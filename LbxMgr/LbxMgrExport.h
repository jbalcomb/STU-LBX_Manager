#ifndef LBXMGR_EXPORT_H
#define LBXMGR_EXPORT_H

#include "lib_lbx.h"

void lbxmgr_export_to_csv(LBX_DATA * lbx);

void lbxmgr_export_records_to_bin(LBX_DATA * lbx);
void lbxmgr_export_records_to_c(LBX_DATA * lbx);
void lbxmgr_export_records_to_hex(LBX_DATA * lbx);

#endif  // LBXMGR_EXPORT_H
