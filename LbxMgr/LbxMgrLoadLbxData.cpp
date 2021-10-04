#include "LbxMgrLoadLbxData.h"
#include "LbxMgr.h"  // LBXMGR_DATA; LBXMGR_DEBUG_MODE;
#include "lib_lbx_process_lbx.h"



void lbxmgr_prepare_lbx_data_array(LBXMGR_DATA * lbxmgr_data)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_prepare_lbx_data_array()\n");

    int file_name_count;
    int itr_file_name_count;
    char * tmp_file_path;

    file_name_count = get_file_name_count(lbxmgr_data->filenames);

    if (LBXMGR_DEBUG_VERBOSE_MODE) printf("DEBUG: file_name_count: %d\n", file_name_count);

    for (itr_file_name_count = 0; itr_file_name_count < file_name_count; itr_file_name_count++)
    {

        tmp_file_path = get_file_path(lbxmgr_data->filenames, itr_file_name_count);

        if (LBXMGR_DEBUG_VERBOSE_MODE) printf("DEBUG: tmp_file_path: %s\n", tmp_file_path);


        /* LBX_DATA * lbx_data; lbx_data = create_lbx_data(char * tmp_file_path); */
        lbxmgr_data->lbx_data_array[itr_file_name_count] = create_lbx_data(tmp_file_path);

        liblbx_process_lbx_file(lbxmgr_data->lbx_data_array[itr_file_name_count]);

    }

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_prepare_lbx_data_array()\n");
}

void lbxmgr_unload_lbx_data_array(LBXMGR_DATA * lbxmgr_data)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_unload_lbx_data_array()\n");

    int file_name_count;
    int itr_filenames;

    file_name_count = get_file_name_count(lbxmgr_data->filenames);

    for (itr_filenames = 0; itr_filenames < file_name_count; itr_filenames++)
    {

        destroy_lbx_data(lbxmgr_data->lbx_data_array[itr_filenames]);

    }

    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_unload_lbx_data_array()\n");
}
