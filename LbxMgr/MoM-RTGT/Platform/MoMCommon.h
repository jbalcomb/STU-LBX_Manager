// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file
/// \brief  Common structures for mapping memory of (Borland) executable files
///         and of LBX files

#ifndef MOMCOMMON_H
#define MOMCOMMON_H

#include <cassert>
#include <iostream>
#include <sstream>
#include <cstring>

// Specify integer types with specific sizes
#if defined(_MSC_VER) || defined(SWIG)         // Compiler MS Visual Studio or Swig (no proper POSIX)
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
#else
#include <stdint.h>     // POSIX
#endif

// Specify to pack structures
#if defined(_MSC_VER)       // Compiler MS Visual Studio
  #pragma pack(push, 1)
  #define PACKED_STRUCT
#elif defined(__linux__)    // Compiler Linux g++
  #define PACKED_STRUCT __attribute__((packed))
#elif defined(__GNUC__)     // Compiler Linux g++ (use g++ -fshort-enums)
  #pragma pack(push, 1)
  #define PACKED_STRUCT __attribute__((packed))
#elif defined(SWIG)         // Swig wrapper generator
  #define PACKED_STRUCT
#endif

namespace MoM {

/// \brief The paragraph size used in the i386 architecture
///        It defines the unit of a segment address
const size_t gPARAGRAPH_SIZE = 16;

/// \brief The block size used in the i386 architecture
///        It is used in the EXE_Header to define where the Extra Data starts
const size_t gBLOCK_SIZE = 512;

/// \brief The layout of an EXE Header on the DOS platform
typedef struct PACKED_STRUCT // EXE_Header
{
  uint16_t signature;                       // 00 == 0x5a4D == "MZ"
  uint16_t bytes_in_last_block;             // 02 blocks are 512 bytes
  uint16_t blocks_in_file;                  // 04 blocks are 512 bytes
  uint16_t nr_relocation_items;             // 06 Number of items to relocate when loading offset is known
  uint16_t load_module_offset_paragraphs;   // 08 paragraph is 16 bytes, offset to Load Module
  uint16_t min_needed_paragraphs;           // 0A min # of paragraphs needed to run program
  uint16_t max_needed_paragraphs;           // 0C max # of paragraphs the program would like
  uint16_t ss_offset_in_load_module_paragraphs; // 0E offset in load module of stack segment (in paragraphs)
  uint16_t sp_initial;                      // 10
  uint16_t checksum;                        // 12
  uint16_t ip_program_entry_point;          // 14
  uint16_t cs_offset_in_load_module_paragraphs; // 16 offset in load module of the code segment (in paragraphs)
  uint16_t relocation_table_offset_bytes;   // 18 offset in .EXE file of first relocation item
  uint16_t overlay_number;                  // 1A overlay number (0 for root program)
                                            // 1C
} EXE_Header;

//
//The offset of the beginning of the EXE data is computed like this:
//
//exe_data_start = exe.header_paragraphs * 16L;
//
//The offset of the byte just after the EXE data (in DJGPP, the size of the stub and the start of the COFF image) is computed like this:
//
//extra_data_start = exe.blocks_in_file * 512L;
//if (exe.bytes_in_last_block)
//  extra_data_start -= (512 - exe.bytes_in_last_block);


/// \brief The layout of an DOS relocation unit (a far pointer)
typedef struct PACKED_STRUCT // EXE_Reloc
{
  uint16_t offset;
  uint16_t segment;
} EXE_Reloc;

/// \brief Typedef for a field representing an offset in the datasegment
typedef uint16_t DS_Offset;

typedef struct PACKED_STRUCT // BorlandSegmentEntry
{
    uint16_t    base;
    uint16_t    size;
    uint16_t    type;
    uint16_t    offset;
} BorlandSegmentEntry;

typedef struct PACKED_STRUCT // BorlandStubJmpEntry
{
    //          EA <offset> <base-addr> ; jmp <base-addr>:<offset>
    uint8_t     jmpfar_code;
    uint16_t    offset;
    uint16_t    base_addr;
} BorlandStubJmpEntry;

typedef struct PACKED_STRUCT // BorlandStub
{
    uint8_t     int_code[2];        // 00   CD 3F    ; int 3F
    uint16_t    memswap;            // 02
    uint32_t    fileoff;            // 04   Relative to first overlay
    uint16_t    codesize;           // 08
    uint16_t    relsize;            // 0A
    uint16_t    nentries;           // 0C   Number of jmp-entries
    uint16_t    prevstub;           // 0E
    uint8_t     workarea[0x10];     // 10
    BorlandStubJmpEntry entries[1]; // 20   Array of jmp-entries, each of size 5
                                    //      EA <offset> <base-addr> ; jmp <base-addr>:<offset>
} BorlandStub;

typedef struct PACKED_STRUCT // LBXHEADER
{
    uint16_t    n;                  // 00
    uint32_t    magic_number;       // 02: AD FE 00 00
    uint16_t    reserved;           // 06
                                    // SIZE 08
} LBXHEADER;

typedef struct PACKED_STRUCT // FLIC_HEADER
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
  uint16_t      aspect_dx;          // Width of square rectangle (FLC only)
  uint16_t      aspect_dy;          // Height of square rectangle (FLC only)
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

typedef struct PACKED_STRUCT // FLIC_FRAME_TYPE
{
  uint32_t      size;               // Size of the chunk, including subchunks
  uint16_t      type;               // Chunk type: 0xF1FA
  uint16_t      chunks;             // Number of subchunks
  uint16_t      delay;              // Delay in milliseconds
  int16_t       reserved;           // Always zero
  uint16_t      width;              // Frame width override (if non-zero)
  uint16_t      height;             // Frame height override (if non-zero)
} FLIC_FRAME_TYPE;

typedef struct PACKED_STRUCT // FLIC_COLOR_64
{
    uint32_t    size;               // Size of the chunk, including subchunks
    uint16_t    type;               // Chunk type: 11
    uint16_t    chunks;             // Number of subchunks
    uint16_t    skip;               // Number of entries to skip
} FLIC_COLOR_64;

typedef struct PACKED_STRUCT // FLIC_FLI_COPY
{
    uint32_t    size;               // Size of the chunk, including subchunks
    uint16_t    type;               // Chunk type: 16
} FLIC_FLI_COPY;

const uint16_t gFLIC_file_type_FLI = 0xAF11;        // For FLI files
const uint16_t gFLIC_chunk_type_FRAME = 0xF1FA;     // frame type
const uint16_t gFLIC_chunk_type_COLOR_64 = 11;      // 64-level colour palette
const uint16_t gFLIC_chunk_type_FLI_COPY = 16;      // uncompressed image

}

#if defined(_MSC_VER)       // Compiler MS Visual Studio
  #pragma pack(pop)
#elif defined(__MINGW32__)  // Compiler MinGW
  #pragma pack(pop)
#endif
#undef PACKED_STRUCT

#endif
