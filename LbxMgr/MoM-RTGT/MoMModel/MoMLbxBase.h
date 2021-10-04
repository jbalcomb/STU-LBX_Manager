// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef MOMLBXBASE_H_
#define MOMLBXBASE_H_

#include <vector>

#include "MoMCommon.h"

namespace MoM
{

struct LBXRecordID
{
    explicit LBXRecordID(const char aLbxTitle[9], int aLbxIndex, int aLbxSubindex = 0) :
        lbxIndex(aLbxIndex), lbxSubindex(aLbxSubindex)
    {
        memset(lbxTitle, '\0', sizeof(lbxTitle));
        strncpy(lbxTitle, aLbxTitle, 8);
        char* extension = strchr(lbxTitle, '.');
        if (0 != extension)
        {
            *extension = '\0';
        }
    }
    char        lbxTitle[9];
    int         lbxIndex;
    int         lbxSubindex;
};

/// \brief Defines a base class for reading and writing LBX files
class MoMLbxBase
{
public:
    enum eRecordType
    {
        TYPE_empty,     // 0 bytes
        TYPE_array,     // <n> <recordsize> equals size
        TYPE_midi,      // DEAFh 0001h
        TYPE_wave,      // DEAFh 0002h
        TYPE_palette,   // FONTS.LBX 2+
        TYPE_images,    // <width> <height> = 3x3 (in MAIN.LBX) up to 320x200
        TYPE_font,      // FONTS.LBX 0
        TYPE_custom,    // Other (sound drivers, TERR*.LBX)
        eRecordType_MAX
    };

    struct Annotation
    {
        char subfile[9];
        char comment[23];
    };

    MoMLbxBase();
    MoMLbxBase(const MoMLbxBase& rhs);
    MoMLbxBase& operator=(const MoMLbxBase& rhs);
    virtual ~MoMLbxBase();

    virtual void close() throw();

    bool load(const std::string& filename);
    bool save(const std::string& filename);

    std::string getFilename() const
    {
        return m_filename;
    }

    size_t getNrRecords() const;

    eRecordType getRecordType(size_t recordNr) const;
    uint8_t* getRecord(size_t recordNr);
    const uint8_t* getRecord(size_t recordNr) const;
    size_t getRecordSize(size_t recordNr) const;
    bool getRecord(size_t recordNr, std::vector<uint8_t>& recordData) const;

    size_t getNrAnnotations() const;
    const Annotation* getAnnotation(size_t recordNr) const;

    size_t getNrSubRecords(size_t recordNr) const;
    const uint8_t* getSubRecord(size_t recordNr, size_t subRecordNr) const;
    uint8_t* getSubRecord(size_t recordNr, size_t subRecordNr);
    bool getSubRecord(size_t recordNr, size_t subRecordNr, std::vector<uint8_t>& subRecordData) const;
    size_t getSubRecordSize(size_t recordNr) const;

    bool replaceRecord(size_t recordNr, const std::vector<uint8_t>& dataBuffer);

private:
    bool initPointers();

    std::string             m_filename;
    std::vector<uint8_t>    m_contents;

    LBXHEADER*              m_LBX_Header;
    uint32_t*               m_DataOffsets;
    Annotation*             m_Annotations;
    uint8_t*                m_Data;
};

}

#endif
