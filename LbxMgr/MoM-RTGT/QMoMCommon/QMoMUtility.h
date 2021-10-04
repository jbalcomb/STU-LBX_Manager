// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef QMOMUTILITY_H_
#define QMOMUTILITY_H_

#include <QDateTime>
#include <QDebug>

#include <iomanip>
#include <sstream>

#include "MoMCommon.h"

inline QString getDateTimeStr()
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    return QString("%0 %1.%2")
            .arg(date.toString(Qt::ISODate))
            .arg(time.toString(Qt::ISODate))
            .arg(time.msec(), 3, 10, QChar('0'));
}

inline std::ostream& operator<<(std::ostream& os, const MoM::EXE_Reloc& rhs)
{
    os << std::hex << rhs.segment << ':' << rhs.offset << std::dec;
    return os;
}

template< typename T >
inline QString toQStr(const T& t)
{
    std::ostringstream oss;
    oss << std::boolalpha;
    oss << t;
    return oss.str().c_str();
}

template<>
inline QString toQStr<uint8_t>(const uint8_t& t)
{
    std::ostringstream oss;
    oss << (unsigned)t;
    if (t >= 16)
    {
        oss << " 0x" << std::hex << std::setfill('0') << std::setw(2) << (unsigned)t;
    }
    return oss.str().c_str();
}

template<>
inline QString toQStr<int8_t>(const int8_t& t)
{
    std::ostringstream oss;
    oss << (int)t;
    if (t < -1 || t >= 16)
    {
        oss << " 0x" << std::hex << std::setfill('0') << std::setw(2) << (unsigned)(uint8_t)t;
    }
    return oss.str().c_str();
}

template<>
inline QString toQStr<uint16_t>(const uint16_t& t)
{
    std::ostringstream oss;
    oss << (unsigned)t;
    if (t >= 16)
    {
        oss << " 0x" << std::hex << std::setfill('0') << std::setw(4) << t;
    }
    return oss.str().c_str();
}

template<>
inline QString toQStr<int16_t>(const int16_t& t)
{
    std::ostringstream oss;
    oss << (int)t;
    if (t < -1 || t >= 16)
    {
        oss << " 0x" << std::hex << std::setfill('0') << std::setw(4) << (uint16_t)t;
    }
    return oss.str().c_str();
}

template<>
inline QString toQStr<uint32_t>(const uint32_t& t)
{
    std::ostringstream oss;
    oss << (unsigned)t;
    if (t >= 16)
    {
        oss << " 0x" << std::hex << std::setfill('0') << std::setw(8) << (unsigned)t;
    }
    return oss.str().c_str();
}

template<>
inline QString toQStr<int32_t>(const int32_t& t)
{
    std::ostringstream oss;
    oss << (int)t;
    if (t < -1 || t >= 16)
    {
        oss << " 0x" << std::hex << std::setfill('0') << std::setw(8) << (uint32_t)t;
    }
    return oss.str().c_str();
}

template< typename T >
inline QString prettyQStr(const T& t)
{
    QString tmp(toQStr(t));

    // Strip number information
    tmp.replace(QRegExp(" \\(\\d+\\)"), "");
    // Strip m_ prefix
    tmp.replace(QRegExp("^m_"), "");
    // Strip CAPITAL_ prefix
    tmp.replace(QRegExp("^[A-Z][A-Z0-9]+_"), "");
    // Replace underscores by spaces
    tmp.replace('_', ' ');

    return tmp;
}

template< class X, class Y >
bool equalLocation(const X& x, const Y& y)
{
    return ((x.m_Plane == y.m_Plane) && (x.m_XPos == y.m_XPos) && (x.m_YPos == y.m_YPos));
}

#endif
