// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef MOMUTILITY_H
#define MOMUTILITY_H

#include <sstream>
#include <vector>

#include "MoMCommon.h"

namespace MoM {

#ifdef ARRAYSIZE
#undef ARRAYSIZE
#endif
#define ARRAYSIZE(a) (sizeof(a) / sizeof(*(a)))

#define MOM_FOREACH(type, var, max) \
    for (MoM::type var = (MoM::type)0; var < MoM::max; MoM::inc(var))

template< typename T >
inline T Abs(const T& x)
{
    return (x < 0 ? -x : x);
}

std::string dirFromFilepath(const std::string filepath);

// Knuth-Morris-Pratt algorithm to find a needle (m) in a haystack (n) of order O(m)+O(n)
// \retval haystack.size() if the needle was not found
// \retval the index of the needle in the haystack otherwise
size_t findStringInBuffer(const std::string& needle, const std::vector<uint8_t>& haystack);
size_t findStringInBuffer(const std::string& needle, const uint8_t* haystack, size_t haystack_size);

std::string formatBufferAsHex(const std::vector<uint8_t>& buffer);

template< typename E >
inline E inc(E& e)
{
    unsigned u = (unsigned)e;
    u++;
    e = (E)u;
    return e;
}

template< typename E, typename Max >
inline bool inRange(const E& e, const Max& m)
{
    return (static_cast<unsigned>(e) < static_cast<unsigned>(m));
}

template< typename T >
inline bool inRange(const T& t, const T& lo, const T& hi)
{
    return ((t >= lo) && (t <= hi));
}

std::string lowercase(const std::string& str);

template< typename T >
inline T Max(const T& a, const T& b)
{
    return (a < b ? b : a);
}

template< typename T >
inline T Min(const T& a, const T& b)
{
    return (a < b ? a : b);
}

template< typename T >
inline T Range(const T& value, const T& minimum, const T& maximum)
{
    if (value < minimum)
        return minimum;
    if (value > maximum)
        return maximum;
    return value;
}

int Round(double value);

std::string replaceUnderscoresBySpaces(const std::string& str);

std::string replaceStrInStr(const std::string& str, const std::string& findStr, const std::string& replaceStr);

template< typename T >
inline T Sqr(const T& x)
{
    return x * x;
}

template< typename E >
inline E Succ(const E& e)
{
    unsigned u = (unsigned)e;
    u++;
    return (E)u;
}

template< typename T >
inline int toInt(const T& t)
{
    return static_cast<int>(t);
}

template< typename T >
std::string toStr(const T& t)
{
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

template< typename T >
inline unsigned toUInt(const T& t)
{
    return static_cast<unsigned>(t);
}

/// \brief Class to set a flag to true while it's in scope.
///        Other methods can check the flag to see if someone else is updating.
///        Typically this is to prevent methods from making changes when
///        unintended Qt events occur.
class UpdateLock
{
public:
    /// \brief Constructor that set the parameter to true.
    /// \param updating Flag that is set to true
    UpdateLock(bool& updating) : m_updating(updating)
    {
        m_updating = true;
    }
    /// \brief Destructor that sets the flag to false again.
    ~UpdateLock()
    {
        m_updating = false;
    }
private:
    bool& m_updating;
};

bool writeDataToStream(std::ostream& os, const unsigned char* pdata, size_t size);

}

#endif
