// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include <cassert>
#include <iomanip>
#include <iostream>

#include "QMoMLbx.h"

namespace MoM
{

//
// IMAGE AND ANIMATION DATA:
//
// From: http://masteroforion2.blogspot.nl/
//
// The flags is a two byte value also stored as little endian. Contrary to previous multibyte values this doesn't represent decimal value, but a binary value. Each flag is represented as one bit within the value. Take a look:
// 0000 0000 0000 0000
// ..JI FB.N .... ....
// This is the binary representation of the two byte flags. The first line shows bits and the line below corresponding flags. The dot in the second line means that setting this bit either doesn't do anything (is not used) or the behaviour of the flag is unknown. I have given those flags unique letters and they are as follows:
// J - Junction - I don't know really what it's for. Grig de Griz has pointed out this flag in his format description so I'm including it here. (TAlchemist pointed out that: "Junction flag means that you add each frame to the frame before it making a composted image. and start from the begining on the first frame. That is how only the first frame of the big animations is large and all other frames just include the pixels that changed.")
// I - Internal palette - Tells if the image has internal palette
// F - Functional color - Tells if the image has functional color. This color is used for effects like transparency or shading.
// B - Fill background - Tells the game if it should clear the area on which the image is going to be drawn.
// N - No compression - Tells if the image uses compression. For me it's behaviour is unknown. It's probably a left-over from MOO1 graphic formats which are almost identical to MOO2.

//struct LBX_IMAGE_HEADER
//{
//    uint16_t width;                 // 00
//    uint16_t height;                // 02
//    uint16_t compression;           // 04     0=RLE, 0xDE0A=uncompressed
//    uint16_t nframes;               // 06
//    uint16_t frameDelay;            // 08
//    uint16_t flags;                 // 0A
//    uint16_t reserved_0C;           // 0C
//    uint16_t paletteInfoOffset;     // 0E
//    uint16_t reserved_10;           // 10
//                                    // SIZE 12
//    uint32_t frameOffsets[1];       // 12 [nframes + 1]
//};

//struct LBX_PALETTE_INFO
//{
//    uint16_t paletteOffset;         // 00
//    uint16_t firstPaletteColorIndex;// 02
//    uint16_t paletteColorCount;     // 04
//                                    // SIZE 6
//};

// RLE row codes
// FFh   empty row
// 00h   set of raw RLE sequences
// 80h   set of encoded RLE sequences
//
// chunk            :   image_header
//                      frameoffsets[nframes+1]
//                      palette?
//                      frames
//                  |   (image_header uncompressed_frame)+
//
// palette          :   palette_info
//                      palette_entries
//
// palette_entries  :   (red green blue)+
//
// red, green, blue :   0-63
//
// frames           :   full_frame delta_frame*
//
// full_frame       :   01h frame
//
// delta_frame      :   00h frame
//
// frame            :   rle_row+
//
// rle_row          :   FFh
//                  |   00h <nrowbytes> raw_sequence+
//                  |   80h <nrowbytes> encoded_sequence+
//
// raw_sequence     :   <nseqbytes> <delta> <color>+
//
// encoded_sequence :   <nseqbytes> <delta> (DFh + <count>) <color>
//                  |   <nseqbytes> <delta> <color 0-DFh>+
//


//
// FONT DATA
//
// Inspired from: http://www.spheriumnorth.com/orion-forum/nfphpbb/viewtopic.php?t=91
// But taken from MoM code.
//
// Scratch area that the font data is copied into
// 0000
// 0010 font height
// 0012
// 0014
// 0024
// 0034
// 0048 horizontal spacing
// 004A widths of ascii characters 20h - 7Dh with a spare for 7E and 7F
// 00AA word for the glyph offset of each ascii character starting from 20h - 7Dh with a spare for 7E and 7F
//
// 016A font heights (word) for 8 fonts
// 017A horizontal spacing (word) for 8 fonts
// 018A something heights (word) for 8 fonts (x-height? C-height? left leader?)
// 019A character widths (byte) for 8 fonts (8 x 60h)
// 049A glyph data offsets (word) for 4 fonts (4 x 120h)
// 0A9A glyph data (3ADAh)
// 4574 (SIZE FontsStyleData)
//

static bool gVerbose = false;

static void dump(const uint8_t* ptr, unsigned n)
{
    if (gVerbose)
    {
        std::cout << ":";
        for (unsigned i = 0; i < n; ++i)
        {
            if (i > 0)
            {
                std::cout << " ";
            }
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (unsigned)*ptr++;
        }
        std::cout << std::dec;
    }
}

static void dumpnl(const uint8_t* ptr, unsigned n)
{
    if (gVerbose)
    {
        dump(ptr, n);
        std::cout << std::endl;
    }
}

bool convertImagesToLbx(const QMoMAnimation& images, std::vector<uint8_t>& dataBuffer, const std::string&)
{
    if (images.empty())
        return false;

    assert(0 != images[0]);
    const QImage& image0 = *images[0];

    size_t upper_bound = 200 * 320 * 200;
    if (images.size() * image0.width() * image0.height() > (int)upper_bound / 2)
    {
        std::cout << "convertImagesToLbx failed: buffer too big size=" << images.size()
                  << " width=" << image0.width() << " height=" << image0.height()
                  << " > " << (int)upper_bound / 2 << std::endl;
        return false;
    }

    dataBuffer.resize(upper_bound); // Upper bound
    uint8_t* data = &dataBuffer[0];
    size_t dataOffset = 0;

    // Encode header
    *(uint16_t*)(data + 0) = image0.width();
    *(uint16_t*)(data + 2) = image0.height();
    *(uint16_t*)(data + 6) = images.size();
    *(uint16_t*)(data + 14) = 0;        // No internal palette
    *(uint16_t*)(data + 16) = 1;        // ??? Copied from original MoM image
    uint32_t* frameOffsets = (uint32_t*)(data + 18);
    uint8_t* ptr = (uint8_t*)(frameOffsets + (images.size() + 1));

    frameOffsets[0] = dataOffset + (ptr - data);

    for (int imageNr = 0; imageNr < images.size(); ++imageNr)
    {
        assert(0 != images[imageNr]);
        const QImage& image = *images[imageNr];

        // TODO: check width, height
        // TODO: check encoding is 8-bit indexed (assumed to be corresponding to the default palette)

        *ptr++ = '\x01';       // Identify full frame (00=incremental, 01=full)
        for (int x = 0; x < image.width(); ++x)
        {
            int y = 0;
            int prev_y = y;

            // Find first non-transparent
            for (; y < image.height(); ++y)
            {
                uint8_t value = image.pixelIndex(x, y);
                if ((gTRANSPARENT_COLOR != value) && (255 != value))
                {
                    break;
                }
            }
            if (y >= image.height())
            {
                *ptr++ = '\xFF';    // Empty line
                continue;
            }

            uint8_t* row_ptr = ptr;
            *ptr++ = '\x00';                            // Code for Absolute values (i.e. NON-RLE)
            *ptr++ = image.height() + 2;                // Upper estimate for Bytes in row

            while (y < image.height())
            {
                uint8_t* seq_ptr = ptr;
                *ptr++ = image.height();                    // Upper estimate for Bytes in sequence
                *ptr++ = y - prev_y;                        // Delta y

                // Copy pixels until transparent pixel found
                for (; y < image.height(); ++y)
                {
                    uint8_t value = image.pixelIndex(x, y);
                    if ((gTRANSPARENT_COLOR == value) || (255 == value))
                    {
                        break;
                    }
                    *ptr++ = value;
               }

                seq_ptr[0] = (uint8_t)(ptr - seq_ptr - 2);  // Update Bytes in sequence
                prev_y = y;

                // Find next non-transparent
                for (; y < image.height(); ++y)
                {
                    uint8_t value = image.pixelIndex(x, y);
                    if ((gTRANSPARENT_COLOR != value) && (255 != value))
                    {
                        break;
                    }
                }
            }

            row_ptr[1] = (uint8_t)(ptr - row_ptr - 2);  // Update Bytes in row
        }

        frameOffsets[imageNr + 1] = dataOffset + (ptr - data);
    }

    dataBuffer.resize(ptr - data);

    return true;
}

bool convertLbxToImages(const uint8_t* data, size_t size, const QMoMPalette& defaultColorTable, QMoMAnimation& images, const std::string& context)
{
    images.clear();

    if (0 == data)
        return false;

    // Extract header
    uint16_t width = *(uint16_t*)(data + 0);
    uint16_t height = *(uint16_t*)(data + 2);
    uint16_t nframes = *(uint16_t*)(data + 6);
    uint16_t paletteInfoOffset = *(uint16_t*)(data + 14);
    uint32_t* frameOffsets = (uint32_t*)(data + 18);
    if (paletteInfoOffset >= size)
    {
        std::cout << "Lbx bitmap '" << context << "' is corrupt"  << std::endl;
        return false;
    }

    uint16_t firstPaletteColorIndex = 0;
    uint16_t paletteColorCount = 255;

    QMoMPalette colorTable(defaultColorTable);

    if (gVerbose)
    {
        std::cout << "Images: context=" << context << "\n"
                  << " size=" << (unsigned)size
                  << " width=" << (unsigned)width
                  << " height=" << (unsigned)height
                  << " nframes=" << (unsigned)nframes
                  << " paletteInfoOffset=" << (unsigned)paletteInfoOffset
                  << std::endl;
dumpnl(data, 32);
    }

    // TODO: Range checks
    int nimages = nframes;
    if (nframes < 1)
    {
        nimages = 1;        // ??? nframes == 0 is code for uncompressed ???
    }

    bool ok = true;

    // Decode palette if present
    if (paletteInfoOffset > 0)
    {
        uint16_t paletteOffset = *(uint16_t*)(data + paletteInfoOffset);
        firstPaletteColorIndex = *(uint16_t*)(data + paletteInfoOffset + 2);
        paletteColorCount = *(uint16_t*)(data + paletteInfoOffset + 4);

        if (gVerbose)
        {
            std::cout << "Palette: " << "\n"
                      << " paletteOffset=" << (unsigned)paletteOffset
                      << " firstPaletteColorIndex=" << (unsigned)firstPaletteColorIndex
                      << " paletteColorCount=" << (unsigned)paletteColorCount
                      << std::endl;
        }
        if (firstPaletteColorIndex + paletteColorCount > colorTable.size())
        {
            colorTable.resize(firstPaletteColorIndex + paletteColorCount);
        }
        uint8_t* palette = (uint8_t*)(data + paletteOffset);
        for (size_t i = 0; i < paletteColorCount; ++i)
        {
dumpnl(palette + 3 * i, 3);
            colorTable[firstPaletteColorIndex + i] = qRgb(4 * palette[3*i], 4 * palette[3*i+1], 4 * palette[3*i+2]);
        }
    }

    images.resize(nimages);

    // Create images
    for (int frameNr = 0; frameNr < nimages; ++frameNr)
    {
        // Create empty image
        images[frameNr] = QMoMImagePtr(new QImage(width, height, QImage::Format_Indexed8));
        QImage& image = *images[frameNr];

        // Set color table
        image.setColorTable(colorTable);

        // Fill with transparent color
        images[frameNr]->fill(gTRANSPARENT_COLOR);

        const uint8_t* ptr = data;
        if (nframes > 0)
        {
            if (gVerbose)
            {
                std::cout << "frameOffset[" << frameNr << "]=" << frameOffsets[frameNr] << std::endl;
            }
            ptr += frameOffsets[frameNr];
            if ((frameOffsets[frameNr] >= size) || (ptr >= data + size))
            {
                std::cout << "Lbx bitmap '" << context << "' is corrupt"  << std::endl;
                ok = false;
                break;
            }
dumpnl(ptr, 1);

            uint8_t frameCode = *ptr++;     // Should be 01 for first frame,
                                            // and either 00 (incremental), or 01 (complete) for the following frames
            if ((0 == frameCode) && (frameNr > 0))
            {
                // Copy previous image
                image = QImage(*images[frameNr - 1]);
            }
            else if (1 != frameCode)
            {
                std::cout << "Lbx bitmap '" << context << "' has unexpected frameCode " << (unsigned)frameCode << std::endl;
                ok = false;
                break;
            }

            for (int row = 0; ok && (row < width); ++row)
            {
                int x = row;
                if (0xFF == *ptr)
                {
                    // Empty line
dumpnl(ptr, 1);
                    ptr++;
                    continue;
                }
                else if ((0x00 == *ptr) || (0x80 == *ptr))
                {
                    // Sequence(s)
dump(ptr, 2);
                    uint8_t rowCode = *ptr++;
                    uint8_t nrowbytes = *ptr++;
                    const uint8_t* endrow_ptr = ptr + nrowbytes;
                    int y = 0;
                    while (ok && (ptr < endrow_ptr))
                    {
dump(ptr, 2);
                        uint8_t nseqbytes = *ptr++;
                        uint8_t delta = *ptr++;
                        y += delta;
dump(ptr, nseqbytes);
                        const uint8_t* endseq_ptr = ptr + nseqbytes;
                        while (ok && (ptr < endseq_ptr))
                        {
                            uint8_t value = *ptr++;
                            if ((0x80 == rowCode) && (value >= 0xE0))
                            {
                                value -= 0xDF;
                                while (value-- > 0)
                                {
                                    if (0 == *ptr)
                                    {
                                        std::cout << "Lbx bitmap '" << context << "' has (repeated) color 0 at x = " << x << " y = " << y << " offset(d) = " << (unsigned)(ptr - data) << std::endl;
                                        ok = false;
                                        break;
                                    }
                                    if (y >= height)
                                    {
                                        std::cout << "Lbx bitmap '" << context << "' has (repeated) coordinate out-of-range at x = " << x << " y = " << y << " offset(d) = " << (unsigned)(ptr - data) << std::endl;
                                        ok = false;
                                        break;
                                    }
                                    if (*ptr >= colorTable.size())
                                    {
                                        std::cout << "Lbx bitmap '" << context << "' has (repeated) value " << (unsigned)*ptr << " out-of-range at x = " << x << " y = " << y << " offset(d) = " << (unsigned)(ptr - data) << std::endl;
                                        ok = false;
                                        break;
                                    }
                                    image.setPixel(x, y, *ptr);
                                    y++;
                                }
                                ptr++;
                            }
                            else
                            {
                                if (0 == value)
                                {
                                    //std::cout << "Lbx bitmap '" << context << "' has color 0 at x = " << x << " y = " << y << " offset(d) = " << (unsigned)(ptr - data) << std::endl;
                                }
                                if (y >= height)
                                {
                                    std::cout << "Lbx bitmap '" << context << "' has coordinate out-of-range at x = " << x << " y = " << y << " offset(d) = " << (unsigned)(ptr - data) << std::endl;
                                    ok = false;
                                    break;
                                }
                                if (value >= colorTable.size())
                                {
                                    std::cout << "Lbx bitmap '" << context << "' has value " << (unsigned)value << " out-of-range at x = " << x << " y = " << y << " offset(d) = " << (unsigned)(ptr - data) << std::endl;
                                    ok = false;
                                    break;
                                }
                                image.setPixel(x, y, value);
                                y++;
                            }
                        }
                    }
dumpnl(ptr, 0);
                }
                else
                {
dumpnl(ptr, 1);
                    std::cout << "Lbx bitmap '" << context << "' has unknown directive " << (unsigned)*ptr << " at x = " << x << " offset(d) = " << (unsigned)(ptr - data) << std::endl;
                    ok = false;
                    break;
                }
            }
//if ((frameNr + 1 < nframes) && (ptr < data + frameOffsets[frameNr+1]))
//{
//std::cout << "Remaining bytes: " << std::endl;
//dumpnl(ptr, (unsigned)(data + frameOffsets[frameNr+1] - ptr));
//}
        }
        else
        {
            // ??? Uncompressed ???
            if (gVerbose)
            {
                std::cout << "Uncompressed" << std::endl;
            }
dumpnl(ptr, 16);
            ptr += 16;

            if (ptr + width * height > data + size)
            {
                std::cout << "Lbx bitmap '" << context << "' has corrupt uncompressed data" << std::endl;
                ok = false;
            }

            for (int row = 0; ok && (row < width); ++row)
            {
                int x = row;
dumpnl(ptr, height);
                for (int y = 0; y < height; ++y)
                {
                    image.setPixel(x, y, *ptr);
                    ptr++;
                }
            }
        }
dumpnl(ptr, 0);
    }
dumpnl(0, 0);

    return ok;
}

void convertLbxToPalette(const uint8_t* dataPalette, QMoMPalette& colorTable)
{
    for (int i = 0; i < 256; ++i)
    {
        colorTable[i] = qRgb(4 * dataPalette[3*i], 4 * dataPalette[3*i+1], 4 * dataPalette[3*i+2]);
    }
    // Set transparent color
    colorTable[gTRANSPARENT_COLOR] = qRgba(0, 0, 0, 0);
}

}
