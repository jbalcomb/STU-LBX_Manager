
#include <cstdint>
#include <iostream>             // std:: cout; FILE
#include <iomanip>              // std:: setfill, setw;

#include "MoMRTGT__convertLbxToImages.h"

#include <QImage>


/* F:\Development_OPC\momrtgt-code\QMoMCommon\QMoMLbx.cpp */

static bool gVerbose = true;


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
                    ptr++;
                    continue;
                }
                else if ((0x00 == *ptr) || (0x80 == *ptr))
                {
                    // Sequence(s)
                    uint8_t rowCode = *ptr++;
                    uint8_t nrowbytes = *ptr++;
                    const uint8_t* endrow_ptr = ptr + nrowbytes;
                    int y = 0;
                    while (ok && (ptr < endrow_ptr))
                    {
                        uint8_t nseqbytes = *ptr++;
                        uint8_t delta = *ptr++;
                        y += delta;
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

    return ok;
}
