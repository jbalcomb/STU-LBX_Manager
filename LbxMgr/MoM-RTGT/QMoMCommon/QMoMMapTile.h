// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2011-09-30
// ---------------------------------------------------------------------------

/// \file

#ifndef QMOMMAPTILE_H_
#define QMOMMAPTILE_H_

#include <QGraphicsItem>

#include "MoMLocation.h"
#include "MoMTemplate.h"

namespace MoM
{

class QMoMMapTile : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit QMoMMapTile(const MoM::MoMLocation& location);
    virtual ~QMoMMapTile();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    const MoM::MoMLocation& getLocation() const
    {
        return m_location;
    }
    void setLocation(MoM::MoMLocation& location)
    {
        m_location = location;
    }

    MoM::eTerrainBattle* getTerrainBattle() const
    {
        return m_terrainBattle;
    }
    void setTerrainBattle(MoM::eTerrainBattle* terrainBattle)
    {
        m_terrainBattle = terrainBattle;
    }

    MoM::eTerrainBonusDeposit* getTerrainBonus() const
    {
        return m_terrainBonus;
    }
    void setTerrainBonus(MoM::eTerrainBonusDeposit* bonusDeposit)
    {
        m_terrainBonus = bonusDeposit;
    }

    MoM::Terrain_Changes* getTerrainChange() const
    {
        return m_terrainChange;
    }
    void setTerrainChange(MoM::Terrain_Changes* terrainChange)
    {
        m_terrainChange = terrainChange;
    }

    uint8_t* getTerrainExplored() const
    {
        return m_terrainExplored;
    }
    void setTerrainExplored(uint8_t* terrainExplored)
    {
        m_terrainExplored = terrainExplored;
    }

    MoM::eTerrainType* getTerrainType() const
    {
        return m_terrainType;
    }
    void setTerrainType(MoM::eTerrainType* terrainType)
    {
        m_terrainType = terrainType;
    }

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *)
    {
//        qDebug() << "QMoMMapTile::hoverEnterEvent(event)";
    }

private:
    void timerEvent(QTimerEvent*);

    MoM::MoMLocation m_location;
    MoM::eTerrainBattle* m_terrainBattle;
    MoM::eTerrainBonusDeposit* m_terrainBonus;
    MoM::Terrain_Changes* m_terrainChange;
    uint8_t* m_terrainExplored;
    MoM::eTerrainType* m_terrainType;
    int m_frameNr;
    int m_idTimer;
};

}

#endif // QMOMMAPTILE_H
