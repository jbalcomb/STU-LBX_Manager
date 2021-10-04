// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include <assert.h>
#include <fstream>
#include <string.h>

#include "MoMUtility.h"

#include "MoMLbxBase.h"

namespace MoM
{

MoMLbxBase::MoMLbxBase() :
    m_filename(),
    m_contents(),
    m_LBX_Header(0),
    m_DataOffsets(0),
    m_Annotations(0),
    m_Data(0)
{
}

MoMLbxBase::MoMLbxBase(const MoMLbxBase& rhs) :
    m_filename(rhs.m_filename),
    m_contents(rhs.m_contents),
    m_LBX_Header(0),
    m_DataOffsets(0),
    m_Annotations(0),
    m_Data(0)
{
    // Initialize
    initPointers();
}

MoMLbxBase& MoMLbxBase::operator=(const MoMLbxBase& rhs)
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

MoMLbxBase::~MoMLbxBase()
{
}

void MoMLbxBase::close() throw()
{
    m_filename.clear();
    m_contents.clear();

    m_LBX_Header = 0;
    m_DataOffsets = 0;
    m_Annotations = 0;
    m_Data = 0;
}

size_t MoMLbxBase::getNrRecords() const
{
    if (m_contents.empty())
        return 0;
    return m_LBX_Header->n;
}

MoMLbxBase::eRecordType MoMLbxBase::getRecordType(size_t recordNr) const
{
    size_t size = getRecordSize(recordNr);

    if (size == 0)
    {
        return TYPE_empty;
    }
    if (size < 4)
    {
        return TYPE_custom;
    }
    if (getNrSubRecords(recordNr) > 0)
    {
        return TYPE_array;
    }

    const uint8_t* data = getRecord(recordNr);
    assert(0 != data);
    uint16_t signature1 = *(uint16_t*)&data[0];
    uint16_t signature2 = *(uint16_t*)&data[2];
    if ((0xDEAF == signature1) && (1 == signature2))
    {
        return TYPE_midi;
    }
    if ((0xDEAF == signature1) && (2 == signature2))
    {
        return TYPE_wave;
    }

    std::string lowercase_filename = lowercase(m_filename);
    if (std::string::npos != lowercase_filename.find("fonts.lbx"))
    {
        if (recordNr == 0)
        {
            return TYPE_font;
        }
        if (recordNr >= 2)
        {
            return TYPE_palette;
        }
    }

    if (inRange((int)signature1, 3, 320) && inRange((int)signature2, 3, 200))
    {
        return TYPE_images;
    }

    return TYPE_custom;
}

uint8_t* MoMLbxBase::getRecord(size_t recordNr)
{
    uint8_t* ptr = 0;

    size_t size = getRecordSize(recordNr);  // Includes range checks
    if (size > 0)
    {
        ptr = (uint8_t*)(&m_contents[0] + m_DataOffsets[recordNr]);
    }

    return ptr;
}

const uint8_t* MoMLbxBase::getRecord(size_t recordNr) const
{
    const uint8_t* ptr = 0;

    size_t size = getRecordSize(recordNr);  // Includes range checks
    if (size > 0)
    {
        ptr = (const uint8_t*)(&m_contents[0] + m_DataOffsets[recordNr]);
    }

    return ptr;
}

size_t MoMLbxBase::getRecordSize(size_t recordNr) const
{
    if (m_contents.empty())
        return 0;
    if (recordNr >= m_LBX_Header->n)
        return 0;
    if ((m_DataOffsets[recordNr + 1] <= m_DataOffsets[recordNr]) || (m_DataOffsets[recordNr + 1] > m_contents.size()))
        return 0;

    size_t size = m_DataOffsets[recordNr + 1] - m_DataOffsets[recordNr];

    return size;
}

bool MoMLbxBase::getRecord(size_t recordNr, std::vector<uint8_t> &recordData) const
{
    recordData.clear();
    const uint8_t* data = getRecord(recordNr);
    if (0 != data)
    {
        size_t size = getRecordSize(recordNr);
        recordData.resize(size);
        memcpy(&recordData[0], data, size);
    }
    return (0 != data);
}

size_t MoMLbxBase::getNrAnnotations() const
{
    if (m_contents.empty())
        return 0;
    return ((Annotation*)m_Data - m_Annotations);
}

const MoMLbxBase::Annotation *MoMLbxBase::getAnnotation(size_t recordNr) const
{
    if (recordNr >= getNrAnnotations())
        return 0;
    return &m_Annotations[recordNr];
}

size_t MoMLbxBase::getNrSubRecords(size_t recordNr) const
{
    size_t nrSubRecords = 0;
    const uint8_t* dataChunk = getRecord(recordNr);
    size_t   sizeChunk = getRecordSize(recordNr);

    if ((0 != dataChunk) && (sizeChunk >= 4))
    {
        size_t nrRecords = *(uint16_t*)&dataChunk[0];
        size_t recordSize = *(uint16_t*)&dataChunk[2];
        if (4 + nrRecords * recordSize == sizeChunk)
        {
            nrSubRecords = nrRecords;
        }
    }

    return nrSubRecords;
}

const uint8_t *MoMLbxBase::getSubRecord(size_t recordNr, size_t subRecordNr) const
{
    const uint8_t* data = 0;

    const uint8_t* dataChunk = getRecord(recordNr);
    size_t   sizeChunk = getRecordSize(recordNr);

    if ((0 != dataChunk) && (sizeChunk >= 4))
    {
        size_t nrRecords = *(const uint16_t*)&dataChunk[0];
        size_t recordSize = *(const uint16_t*)&dataChunk[2];
        if ((4 + nrRecords * recordSize == sizeChunk) && (subRecordNr < nrRecords))
        {
            data = &dataChunk[4] + subRecordNr * recordSize;
        }
    }

    return data;
}

uint8_t *MoMLbxBase::getSubRecord(size_t recordNr, size_t subRecordNr)
{
    uint8_t* data = 0;

    uint8_t* dataChunk = getRecord(recordNr);
    size_t   sizeChunk = getRecordSize(recordNr);

    if ((0 != dataChunk) && (sizeChunk >= 4))
    {
        size_t nrRecords = *(const uint16_t*)&dataChunk[0];
        size_t recordSize = *(const uint16_t*)&dataChunk[2];
        if ((4 + nrRecords * recordSize == sizeChunk) && (subRecordNr < nrRecords))
        {
            data = &dataChunk[4] + subRecordNr * recordSize;
        }
    }

    return data;
}

bool MoMLbxBase::getSubRecord(size_t recordNr, size_t subRecordNr, std::vector<uint8_t>& subRecordData) const
{
    subRecordData.clear();
    const uint8_t* data = getSubRecord(recordNr, subRecordNr);
    if (0 != data)
    {
        size_t recordSize = getSubRecordSize(recordNr);
        subRecordData.resize(recordSize);
        memcpy(&subRecordData[0], data, recordSize);
    }
    return (0 != data);
}

size_t MoMLbxBase::getSubRecordSize(size_t recordNr) const
{
    size_t value = 0;
    const uint8_t* dataChunk = getRecord(recordNr);
    size_t   sizeChunk = getRecordSize(recordNr);

    if ((0 != dataChunk) && (sizeChunk >= 4))
    {
        size_t nrRecords = *(uint16_t*)&dataChunk[0];
        size_t recordSize = *(uint16_t*)&dataChunk[2];
        if (4 + nrRecords * recordSize == sizeChunk)
        {
            value = recordSize;
        }
    }

    return value;
}

bool MoMLbxBase::initPointers()
{
    assert(!m_contents.empty());
    uint8_t* pContents = (uint8_t*)&m_contents[0];
    bool ok = true;

    m_LBX_Header        = (LBXHEADER*)pContents;
    m_DataOffsets       = (uint32_t*)(pContents + sizeof(LBXHEADER));
    m_Annotations       = (Annotation*)(pContents + 0x200);
    m_Data              = (uint8_t*)(pContents + m_DataOffsets[0]);

    // Check signature "AD FE 00 00"
    if (0xFEAD != m_LBX_Header->magic_number)
    {
        std::cout << "File does not have the LBX signature" << std::endl;
        ok = false;
    }

    if ((uint8_t*)&m_DataOffsets[m_LBX_Header->n + 2] > pContents + m_contents.size())
    {
        // Not enough space for dataoffset array
        ok = false;
    }
    if ((uint8_t*)&m_Annotations[m_LBX_Header->n + 1] > pContents + m_contents.size())
    {
        // Not enough space for annotations array
        ok = false;
    }
    if (m_DataOffsets[m_LBX_Header->n + 1] > m_contents.size())
    {
        // Not enough space for last record
        ok = false;
    }

    return ok;
}

bool MoMLbxBase::load(const std::string& filename)
{
    std::ifstream ifs(filename.c_str(), (std::ios_base::in | std::ios_base::binary));

    if (!ifs)
    {
        std::cout << "Could not open file '"<< filename << "'" << std::endl;
        return false;
    }

    // Determine and check size
    ifs.seekg(0, std::ios_base::end);
    size_t size = ifs.tellg();
    ifs.seekg(0, std::ios_base::beg);
    if (size < sizeof(LBXHEADER) + sizeof(uint32_t))
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

    if (!initPointers())
    {
        std::cout << "File is not a correct LBX file '"<< filename << "'" << std::endl;
        return false;
    }

    m_filename = filename;

    return true;
}

bool MoMLbxBase::replaceRecord(size_t recordNr, const std::vector<uint8_t>& dataBuffer)
{
    if (m_contents.empty())
        return false;
    if (recordNr >= this->getNrRecords())
        return false;

//    const uint32_t oldRecordSize = m_DataOffsets[recordNr + 1] - m_DataOffsets[recordNr];
    const uint32_t oldOffsetNext = m_DataOffsets[recordNr + 1];
    const uint32_t newOffsetNext = m_DataOffsets[recordNr] + dataBuffer.size();
    const uint32_t sizeToMove = m_DataOffsets[ this->getNrRecords() ] - m_DataOffsets[recordNr + 1];

    // Reallocate contents buffer if bigger size needed
    if (newOffsetNext > oldOffsetNext)
    {
        m_contents.resize(newOffsetNext + sizeToMove);
        initPointers();
    }
    uint8_t* pContents = (uint8_t*)&m_contents[0];

    // Shift all higher records (old and new destinations overlap!)
    memmove(pContents + newOffsetNext, pContents + oldOffsetNext, sizeToMove);

    // Update index table
    for (size_t i = recordNr + 1; i <= this->getNrRecords(); ++i)
    {
        m_DataOffsets[i] = m_DataOffsets[i] + newOffsetNext - oldOffsetNext;
    }

    // Replace record
    memcpy(pContents + m_DataOffsets[recordNr], &dataBuffer[0], dataBuffer.size());

    // Reallocate contents buffer if smaller size needed
    if (newOffsetNext < oldOffsetNext)
    {
        m_contents.resize(newOffsetNext + sizeToMove);
        initPointers();
    }

    return true;
}

bool MoMLbxBase::save(const std::string& filename)
{
    if (m_contents.empty())
        return false;

    std::ofstream ofs(filename.c_str(), std::ios_base::out | std::ios_base::binary);
    if (!ofs)
    {
        std::cout << "Could not open file '"<< filename << "' for writing" << std::endl;
        return false;
    }

    if (!ofs.write((const char*)&m_contents[0], m_contents.size()))
    {
        std::cout << "Could not write .EXE data (fully) to file '"<< filename << "'" << std::endl;
        return false;
    }

    m_filename = filename;

    return true;
}

}
