// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef MOMFLI_H_
#define MOMFLI_H_

#include <vector>

#include "MoMCommon.h"

namespace MoM
{

/// \brief Defines a base class for reading and writing FLI files as written by MoM
class MoMFli
{
public:
    MoMFli();
    MoMFli(const MoMFli& rhs);
    MoMFli& operator=(const MoMFli& rhs);
    virtual ~MoMFli();

    virtual void close() throw();

    bool load(const std::string& filename);

    std::string getFilename() const
    {
        return m_filename;
    }

    int getHeight() const
    {
        return m_FLI_Header->height;
    }

    int getWidth() const
    {
        return m_FLI_Header->width;
    }

    uint8_t* getPalette() const
    {
        return m_paletteData;
    }

    uint8_t* getPixels() const
    {
        return m_pixelData;
    }

private:
    bool initPointers();

    std::string             m_filename;
    std::vector<uint8_t>    m_contents;

    FLIC_HEADER*            m_FLI_Header;
    FLIC_FRAME_TYPE*        m_frame;
    FLIC_COLOR_64*          m_palette_header;
    uint8_t*                m_paletteData;
    FLIC_FLI_COPY*          m_pixel_header;
    uint8_t*                m_pixelData;
};

}

#endif
