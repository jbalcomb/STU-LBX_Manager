// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include <assert.h>
#include <fstream>
#include <string.h>

#include "MoMFli.h"

namespace MoM
{

MoMFli::MoMFli() :
    m_filename(),
    m_contents(),
    m_FLI_Header(0),
    m_frame(0),
    m_palette_header(0),
    m_paletteData(0),
    m_pixel_header(0),
    m_pixelData(0)
{
}

MoMFli::MoMFli(const MoMFli& rhs) :
    m_filename(rhs.m_filename),
    m_contents(rhs.m_contents),
    m_FLI_Header(0),
    m_frame(0),
    m_palette_header(0),
    m_paletteData(0),
    m_pixel_header(0),
    m_pixelData(0)
{
    // Initialize
    initPointers();
}

MoMFli& MoMFli::operator=(const MoMFli& rhs)
{
    if (&rhs != this)
    {
        // Release resources
        close();

        // Copy resources
        m_filename = rhs.m_filename;
        m_contents = rhs.m_contents;

        // Initialize
        initPointers();
    }
    return *this;
}

MoMFli::~MoMFli()
{
}

void MoMFli::close() throw()
{
    m_filename.clear();
    m_contents.clear();

    m_FLI_Header = 0;
    m_frame = 0;
    m_palette_header = 0;
    m_paletteData = 0;
    m_pixel_header = 0;
    m_pixelData = 0;
}

bool MoMFli::initPointers()
{
    assert(!m_contents.empty());
    uint8_t* pContents = (uint8_t*)&m_contents[0];

    m_FLI_Header        = (FLIC_HEADER*)pContents;

    m_frame = (FLIC_FRAME_TYPE*)(m_FLI_Header + 1);

    // Check signature
    if (gFLIC_file_type_FLI != m_FLI_Header->type)
    {
        std::cout << "File is not a FLI file '" << m_filename << "'" << std::endl;
        return false;
    }

    m_palette_header = (FLIC_COLOR_64*)(m_frame + 1);
    m_paletteData = (uint8_t*)(m_palette_header + 1);

    if (m_paletteData > pContents + m_contents.size())
    {
        std::cout << "FLI file is corrupt with no palette in '" << m_filename << "'" << std::endl;
        return false;
    }

    // Check signature
    if (gFLIC_chunk_type_COLOR_64 != m_palette_header->type)
    {
        std::cout << "FLI file does not have a COLOR_64 type palette '" << m_filename << "'" << std::endl;
        return false;
    }

    m_pixel_header = (FLIC_FLI_COPY*)((uint8_t*)m_palette_header + m_palette_header->size);
    m_pixelData = (uint8_t*)(m_pixel_header + 1);

    if (m_pixelData > pContents + m_contents.size())
    {
        std::cout << "FLI file is corrupt with no pixel data in '" << m_filename << "'" << std::endl;
        return false;
    }

    // Check signature
    if (gFLIC_chunk_type_FLI_COPY != m_pixel_header->type)
    {
        std::cout << "FLI file does not have a FLI_COPY type (uncompressed) pixel data '" << m_filename << "'" << std::endl;
        return false;
    }

    // Check pixel data size
    if (m_pixelData + m_FLI_Header->width * m_FLI_Header->height > pContents + m_contents.size())
    {
        std::cout << "FLI file does not have enough pixel data for its dimensions '" << m_filename << "'" << std::endl;
        return false;
    }

    return true;
}

bool MoMFli::load(const std::string& filename)
{
    std::ifstream ifs(filename.c_str(), (std::ios_base::in | std::ios_base::binary));

    if (!ifs)
    {
        std::cout << "Could not open file '" << filename << "'" << std::endl;
        return false;
    }

    // Determine and check size
    ifs.seekg(0, std::ios_base::end);
    size_t size = ifs.tellg();
    ifs.seekg(0, std::ios_base::beg);
    if (size < sizeof(FLIC_HEADER))
    {
        std::cout << "File '"<< filename << "' is not large enough (size = " << size << ")" << std::endl;
        return false;
    }

    close();

    m_contents.resize(size);

    uint8_t* pContents = (uint8_t*)&m_contents[0];

    if (!ifs.read((char*)pContents, m_contents.size()))
    {
        std::cout << "Could not (fully) read file '"<< filename << "'" << std::endl;
        return false;
    }

    m_filename = filename;

    if (!initPointers())
    {
        close();
        return false;
    }

    return true;
}

}
