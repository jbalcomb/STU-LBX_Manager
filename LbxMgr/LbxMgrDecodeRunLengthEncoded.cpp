#include "LbxMgrDecodeRunLengthEncoded.h"

/*
 * Decode Run Length Encoded (RLE)
 *
 * Size of Destination?
 * ...if (image) then (Width * Height * Bytes Per Pixel)
 *
 * Type of RLE?
 * ...
 * Adaptive Run Length Encoding (ARLE)
 *
 *
 * Characteristics
 * Set vs. Copy
 * Set: copy 1 byte n times
 * Copy: copy n bytes 1 time
 * For a 'Set' operation,
 * the 'packet' is 2 bytes, where byte 1 is the count, and byte 2 is the data
 * For a 'Copy' operation,
 * the 'packet' is 1 + n bytes, where byte 1 is the length of the sequence, and bytes n are the sequence
 *
 * How do you know if it is a 'Set' or a 'Copy' 'packet'?
 * There must be an 'indicator'.
 * The simplest approach is an interpretation of byte 1 in the packet, based on where it is non-negative or negative.
 * If it is non-negative, then the 'Operation' is a 'Set' operation.
 * If it is negative, then the 'Operation' is a 'Copy' operation.
 *
 */

void lbx_decode_run_length_encoded(char * dst, char * src)
{

}
