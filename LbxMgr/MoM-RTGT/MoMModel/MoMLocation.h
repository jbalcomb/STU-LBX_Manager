#ifndef MOMLOCATION_H
#define MOMLOCATION_H

#include "MoMTemplate.h"

namespace MoM
{

struct MoMLocation
{
    enum eMap
    {
        MAP_overland,
        MAP_battle,
        eMap_MAX
    };

    explicit MoMLocation(uint8_t xpos = 0, uint8_t ypos = 0, ePlane plane = PLANE_Arcanum, eMap aMap = MAP_overland) :
        m_XPos(xpos),
        m_YPos(ypos),
        m_Plane(plane),
        m_Map(aMap)
    {}
    template<class T>
    explicit MoMLocation(const T& t, eMap aMap = MAP_overland) :
        m_XPos(t.m_XPos),
        m_YPos(t.m_YPos),
        m_Plane(t.m_Plane),
        m_Map(aMap)
    {}

    bool operator==(const MoMLocation& rhs) const
    {
        return ((m_XPos == rhs.m_XPos) && (m_YPos == rhs.m_YPos) && (m_Plane == rhs.m_Plane) && (m_Map == rhs.m_Map));
    }
    bool operator!=(const MoMLocation& rhs) const
    {
        return !(*this == rhs);
    }

    uint8_t         m_XPos;
    uint8_t         m_YPos;
    ePlane          m_Plane;
    eMap            m_Map;
};

}

#endif // MOMLOCATION_H
