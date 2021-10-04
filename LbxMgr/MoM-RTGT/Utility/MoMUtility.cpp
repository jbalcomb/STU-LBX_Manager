// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include <cctype>
#include <cmath>
#include <iomanip>

#include "MoMUtility.h"

namespace MoM
{

std::string dirFromFilepath(const std::string filepath)
{
    std::string dir;
    std::string::size_type pos = filepath.find_last_of("/\\");
    if (std::string::npos != pos)
    {
        dir = filepath.substr(0, pos);
    }
    return dir;
}

//! Knuth-Morris-Pratt algorithm to find a needle (m) in a haystack (n) of order O(m)+O(n)
//! \retval n if the needle was not found
//! \retval the index of the needle in the haystack otherwise
static size_t findKnuthMorrisPratt(const uint8_t* needle, size_t m, const uint8_t* haystack, size_t n)
{
    // Pre-compile the needle
    std::vector<int> border(m + 1);
    border[0] = -1;
    for (int i = 0; i < (int)m; ++i)
    {
        border[i+1] = border[i];
        while ((border[i+1] > -1) && (needle[border[i+1]] != needle[i]))
        {
            border[i+1] = border[border[i+1]];
        }
        border[i+1]++;
    }

    // Find the pre-compiled needle in the haystack
    int seen = 0;
    size_t foundIndex = n;
    for (int i = 0; i < (int)n; ++i)
    {
        while ((seen > -1) && (needle[seen] != haystack[i]))
        {
            seen = border[seen];
        }
        if (++seen == (int)m)
        {
            foundIndex = (size_t)(i - m + 1);

            // Break off the algorithm, since we have what we want;
            break;

            // There are no more characters in needle, so with the next input character let's try with the border of the whole needle.
            seen = border[m];
        }
    }
    return foundIndex;
}

size_t findStringInBuffer(const std::string& needle, const std::vector<uint8_t>& haystack)
{
    return findKnuthMorrisPratt((const uint8_t*)&needle[0], needle.size(), &haystack[0], haystack.size());
}

size_t findStringInBuffer(const std::string& needle, const uint8_t* haystack, size_t haystack_size)
{
    return findKnuthMorrisPratt((const uint8_t*)&needle[0], needle.size(), haystack, haystack_size);
}

std::string formatBufferAsHex(const std::vector<uint8_t>& buffer)
{
    std::ostringstream oss;

    // Set formatting to hex with leading zeroes for the rest of this function
    oss << std::hex << std::setfill('0');

    for (size_t lineIndex = 0; lineIndex < buffer.size(); lineIndex += 16)
    {
        if (lineIndex > 0)
        {
            oss << "\n";
        }

        // Output address within buffer
        oss << std::setw(4) << lineIndex << "|";

        // Output hex
        for (int col = 0; col < 16; ++col)
        {
            if (col == 8)
            {
                oss << "-";
            }
            else if (col > 0)
            {
                oss << " ";
            }

            if (lineIndex + col >= buffer.size())
            {
                oss << "  ";
            }
            else
            {
                oss << std::setw(2) << (unsigned)buffer[lineIndex + col];
            }
        }

        oss << "|";

        // Output ascii
        for (int col = 0; col < 16; ++col)
        {
            if (lineIndex + col >= buffer.size())
            {
                oss << " ";
            }
            else if (isprint((int)buffer[lineIndex + col]))
            {
                oss << buffer[lineIndex + col];
            }
            else
            {
                oss << ".";
            }
        }
    }

    return oss.str();
}

std::string lowercase(const std::string& str)
{
    std::string result = str;
    for (size_t i = 0; i < result.size(); ++i)
    {
        result[i] = (char)tolower(result[i]);
    }
    return result;
}

std::string replaceUnderscoresBySpaces(const std::string& str)
{
    std::string result = str;
    for (size_t i = 0; i < result.size(); ++i)
    {
        if (result[i] == '_')
        {
            result[i] = ' ';
        }
    }
    return result;
}

std::string replaceStrInStr(const std::string& str, const std::string& findStr, const std::string& replaceStr)
{
    std::string result;
    size_t pos = 0;
    size_t posFound;
    while ((posFound = str.find(findStr, pos)) != std::string::npos)
    {
        result += str.substr(pos, posFound - pos);
        result += replaceStr;
        pos = posFound + findStr.length();
    }
    result += str.substr(pos);
    return result;
}

int Round(double value)
{
    return (int)(std::floor(value + 0.5));
}

bool writeDataToStream(std::ostream& os, const unsigned char* pdata, size_t size)
{
    size &= 0xFFFFFFF0;     // Make size a multiple of 16

    os.fill('0');

    for (size_t paragraphIndex = 0; paragraphIndex < size; paragraphIndex += 16)
    {
        os << std::hex << std::setw(8) << paragraphIndex << ": ";

        for (size_t i = 0; i < 16; ++i)
        {
            os << std::hex << std::setw(2) << (unsigned)(unsigned char)pdata[paragraphIndex + i];
            if (7 == i)
            {
                os << '-';
            }
            else
            {
                os << ' ';
            }
        }

        for (size_t i = 0; i < 16; ++i)
        {
            if (std::isprint(pdata[paragraphIndex + i]))
            {
                os << pdata[paragraphIndex + i];
            }
            else
            {
                os << '.';
            }
        }

        os << "\n";
    }

    os << std::dec;

    return os.good();
}

}
