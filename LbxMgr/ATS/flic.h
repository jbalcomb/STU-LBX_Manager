#ifndef FLIC_H
#define FLIC_H

/*
 * J:\STU-EduMat\_FileFormats\FLIC\flic.asc
if (flic->head.type == FLI_TYPE)
{
    / * Do some conversion work here. * /
    flic->head.oframe1 = sizeof(flic->head);
    flic->head.speed = flic->head.speed * 1000L / 70L;          NOTE(JWB): pretty sure the is the jitters/ticks conversion
    return Success;
}
 */

/* J:\STU\DBWD\developc\AnimatorPro\src\QUICKFLI\fli.h */
/*
/ * size of a buffer gauranteed big enough to fit in one frame * /
#define CBUF_SIZE (64000L+3*COLORS+256+16+sizeof(struct video_form) )
*/



typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

typedef struct // FLIC_HEADER
{
    uint32_t      size;               // Size of FLIC including this header
    uint16_t      type;               // File type 0xAF11, 0xAF12, 0xAF30, 0xAF44, ...
    uint16_t      frames;             // Number of frames in first segment
    uint16_t      width;              // FLIC width in pixels
    uint16_t      height;             // FLIC height in pixels
    uint16_t      depth;              // Bits per pixel (usually 8)
    uint16_t      flags;              // Set to zero or to three
    uint32_t      speed;              // Delay between frames
    uint16_t      reserved1;          // Set to zero
    uint32_t      created;            // Date of FLIC creation (FLC only)
    uint32_t      creator;            // Serial number or compiler id (FLC only)
    uint32_t      updated;            // Date of FLIC update (FLC only)
    uint32_t      updater;            // Serial number (FLC only), see creator
    uint16_t      aspect_dx;          // Width of square rectangle (FLC only)       NOTE: 320x200 = 6:5 .:. aspect_dx = 5
    uint16_t      aspect_dy;          // Height of square rectangle (FLC only)      NOTE: 320x200 = 6:5 .:. aspect_dy = 5
    uint16_t      ext_flags;          // EGI: flags for specific EGI extensions
    uint16_t      keyframes;          // EGI: key-image frequency
    uint16_t      totalframes;        // EGI: total number of frames (segments)
    uint32_t      req_memory;         // EGI: maximum chunk size (uncompressed)
    uint16_t      max_regions;        // EGI: max. number of regions in a CHK_REGION chunk
    uint16_t      transp_num;         // EGI: number of transparent levels
    uint8_t       reserved2[24];      // Set to zero
    uint32_t      oframe1;            // Offset to frame 1 (FLC only)
    uint32_t      oframe2;            // Offset to frame 2 (FLC only)
    uint8_t       reserved3[40];      // Set to zero
} FLIC_HEADER;

typedef struct // FLIC_FRAME_TYPE
{
    uint32_t      size;               // Size of the chunk, including subchunks
    uint16_t      type;               // Chunk type: 0xF1FA
    uint16_t      chunks;             // Number of subchunks
    uint16_t      delay;              // Delay in milliseconds
    int16_t       reserved;           // Always zero
    uint16_t      width;              // Frame width override (if non-zero)
    uint16_t      height;             // Frame height override (if non-zero)
} FLIC_FRAME_TYPE;

typedef struct // FLIC_COLOR_64
{
    uint32_t    size;               // Size of the chunk, including subchunks
    uint16_t    type;               // Chunk type: 11
    uint16_t    chunks;             // Number of subchunks
    uint16_t    skip;               // Number of entries to skip
} FLIC_COLOR_64;

typedef struct // FLIC_FLI_COPY
{
    uint32_t    size;               // Size of the chunk, including subchunks
    uint16_t    type;               // Chunk type: 16
} FLIC_FLI_COPY;

const uint16_t gFLIC_file_type_FLI = 0xAF11;        // For FLI files
const uint16_t gFLIC_chunk_type_FRAME = 0xF1FA;     // frame type
const uint16_t gFLIC_chunk_type_COLOR_64 = 11;      // 64-level colour palette
const uint16_t gFLIC_chunk_type_FLI_COPY = 16;      // uncompressed image

}

#endif  /* FLIC_H */
