// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2011-09-30
// ---------------------------------------------------------------------------

#include <QPainter>

#include <stdlib.h>

#include "MoMGenerated.h"
#include "QMoMResources.h"
#include "QMoMUtility.h"

#include "QMoMMapTile.h"

namespace MoM
{

const int gRoadDirectionOffset[9] = { 0, -60, -59, +1, +61, +60, +59, -1, -61 };

QMoMMapTile::QMoMMapTile(const MoM::MoMLocation& location) :
    QObject(),
    QGraphicsItem(),
    m_location(location),
    m_terrainBattle(0),
    m_terrainBonus(0),
    m_terrainChange(0),
    m_terrainExplored(0),
    m_terrainType(0),
    m_frameNr(0),
    m_idTimer(-1)
{
    setAcceptHoverEvents(true);
    m_frameNr = rand();
}

QMoMMapTile::~QMoMMapTile()
{
}

QRectF QMoMMapTile::boundingRect() const
{
    if (MoMLocation::MAP_battle == m_location.m_Map)
    {
        return QRectF(-30/2, -16, 30, 16);
    }
    else
    {
        return QRectF(0, 0, 20, 18);
    }
}

void QMoMMapTile::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    bool animated = false;

    if (0 != m_terrainType)
    {
        int iTerrainType = (int) *m_terrainType;
        if (MoM::PLANE_Myrror == m_location.m_Plane)
        {
            iTerrainType += MoM::eTerrainType_MAX;
        }
		MoM::eTerrainType terrainType = static_cast<MoM::eTerrainType>(iTerrainType);
        const QMoMAnimation animation = QMoMResources::instance().getAnimation(terrainType);
        if (animation.size() > 0)
		{
            m_frameNr = m_frameNr % animation.size();
            if (0 != animation[m_frameNr])
            {
                painter->drawImage(boundingRect(), *animation[m_frameNr]);
            }
		}
        animated = (animation.size() >= 2);
    }

    if (!animated && (m_idTimer != -1))
    {
        killTimer(m_idTimer);
        m_idTimer = -1;
    }
    else if (animated && (m_idTimer == -1))
    {
        m_idTimer = startTimer(200);
    }

    if (0 != m_terrainBattle)
    {
        const QMoMImagePtr image = QMoMResources::instance().getImage(*m_terrainBattle);
        if (0 != image)
        {
            painter->drawImage(boundingRect(), *image);
        }
    }

    if (0 != m_terrainBonus)
    {
        QString toolTip;
        QVector<MoM::eTerrainBonusDeposit> vecDeposits;
        vecDeposits         << DEPOSIT_Iron_Ore
                            << DEPOSIT_Coal
                            << DEPOSIT_Silver_Ore
                            << DEPOSIT_Gold_Ore
                            << DEPOSIT_Gems
                            << DEPOSIT_Mithril_Ore
                            << DEPOSIT_Adamantium_Ore
                            << DEPOSIT_Quork
                            << DEPOSIT_Crysx
                            << DEPOSIT_Wild_Game
                            << DEPOSIT_Nightshade;
        for (int index = vecDeposits.size(); index-- > 0;)
        {
            MoM::eTerrainBonusDeposit deposit = vecDeposits[index];
            if ((*m_terrainBonus & deposit) != deposit)
                continue;
            const QMoMImagePtr image = QMoMResources::instance().getImage(deposit);
            if (0 != image)
            {
                painter->drawImage(boundingRect(), *image);
            }
            toolTip += prettyQStr(deposit);
            break;
        }
        setToolTip(toolTip);
    }

    if (0 != m_terrainChange)
    {
        // TODO
        //if (m_terrainChange->Volcano_producing_for_Owner)
        //{
        //    const QMoMImagePtr image = QMoMResources::instance().getImage(TERRAINCHANGE_Volcano_owner);
        //    if (0 != image)
        //    {
        //        painter->drawImage(boundingRect(), *image);
        //    }
        //}
        if (m_terrainChange->road)
        {
            for (int roadDirection = 0; roadDirection <= 8; ++roadDirection)
            {
                if (m_terrainChange[ gRoadDirectionOffset[roadDirection] ].road || m_terrainChange[ gRoadDirectionOffset[roadDirection] ].enchanted_road)
                {
                    const QMoMImagePtr image = QMoMResources::instance().getImage(TERRAINCHANGE_Road, roadDirection);
                    if (0 != image)
                    {
                        painter->drawImage(boundingRect(), *image);
                    }
                }
            }
        }
        if (m_terrainChange->enchanted_road)
        {
            for (int roadDirection = 0; roadDirection <= 8; ++roadDirection)
            {
                if (m_terrainChange[ gRoadDirectionOffset[roadDirection] ].road || m_terrainChange[ gRoadDirectionOffset[roadDirection] ].enchanted_road)
                {
                    const QMoMImagePtr image = QMoMResources::instance().getImage(TERRAINCHANGE_Enchanted_Road, roadDirection);
                    if (0 != image)
                    {
                        painter->drawImage(boundingRect(), *image);
                    }
                }
            }
        }
        if (m_terrainChange->corruption)
        {
            const QMoMImagePtr image = QMoMResources::instance().getImage(TERRAINCHANGE_Corruption);
            if (0 != image)
            {
                painter->drawImage(boundingRect(), *image);
            }
        }
    }

    if (0 != m_terrainExplored)
    {
        if (*m_terrainExplored == 0)
        {
            // Not explored
            painter->setPen(QPen());
            painter->setBrush(QBrush(Qt::black));
            painter->drawRect(boundingRect());
        }
        else
        {
            // (Partially) explored
            // Nothing to do
        }
    }
}

void QMoMMapTile::timerEvent(QTimerEvent *)
{
    m_frameNr = (m_frameNr + 1) % 4;
    update();
}

}
