#include <stdlib.h>         /* itoa(), splitpath(); EXIT_FAILURE, EXIT_SUCCESS; */
#include <string.h>         /* memcpy(), strcat(), strcpy(), strncpy(), strlen(); */

#include "LbxMgrTest.h"
#include "LbxMgr.h"                 // LBXMGR_DATA; LBXMGR_DEBUG_MODE;
#include "LbxMgrLoadLbxData.h"

#include "ATS/ats_filesystem.h"     // ats_create_directory();
#include "LbxMgrPathsNames.h"
#include "LbxMgrExportCsv.h"
#include "LbxMgrExport.h"

#include <lib_lbx_process_lbx.h>
#include <ATS/ats_matrix_transform.h>
#include <ATS/ats_array_test.h>


/*
char TEST_DIRECTORY_PATH[] = "F:\\devel_data\\STU\\MoM131_LBX\\_test";
char TEST_FILE_PATH[] = "F:\\devel_data\\STU\\MoM131_LBX\\MAIN.LBX";
int TEST_ENTRY_INDEX = 0;
*/

/*!
 * lbxmgr_test - call a specific function for testing purposes
 * @param lbxmgr_data The LBXMGR structure containing the run-time data for the current program instance
 */
void lbxmgr_test(LBXMGR_DATA * lbxmgr_data)
{
    if (LBXMGR_DEBUG_MODE) printf("DEBUG: BEGIN: lbxmgr_test()\n");

    test_print_arrays_orders();

    /*
     * Test Matrix Rearrangement
     */
    unsigned int row_count;
    unsigned int column_count;
    unsigned int element_count;
    unsigned char * column_major;
    unsigned char * row_major;
    unsigned char * flipped_x;

    row_count = 8;
    column_count = 5;
    element_count = column_count * row_count;

    column_major = (unsigned char *)malloc(element_count);
    memcpy(column_major, test_array_pixels_onscreen, sizeof(test_array_pixels_onscreen));
    print_array(column_major, element_count);
    print_matrix_with_box(column_major, row_count, column_count);

    /*
     * pixels as On-Screen
     * pixels as FLIC array
     * pixels as BMP array
     * ...
     * transpose
     * reflect / mirror / flip
     * rotate
     * ...
     * array as matrix
     *
     */
    row_major = (unsigned char *)malloc(element_count);
    ats_transform_column_major_to_row_major(row_major, column_major, row_count, column_count);
    print_array(row_major, element_count);
    print_matrix_with_box(row_major, row_count, column_count);


//    flipped_x = (unsigned char *)malloc(row_count * column_count);
//    flip_x_axis(flipped_x, column_major, row_count, column_count);
//    print_array(column_major, element_count);
//    print_array(flipped_x, element_count);


    if (LBXMGR_DEBUG_MODE) printf("DEBUG: END: lbxmgr_test()\n");
    exit(EXIT_SUCCESS);
}
