// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2011-09-30
// ---------------------------------------------------------------------------

/// \file

#ifndef QMOMUNITTILE_H_
#define QMOMUNITTILE_H_

#include <QGraphicsItem>

#include "MoMTemplate.h"
#include "MoMUnit.h"
#include "QMoMSharedPointers.h"

namespace MoM
{

class QMoMUnitTile: public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    QMoMUnitTile(bool isBattlefield = false);
    virtual ~QMoMUnitTile();

    QRectF boundingRect() const;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void setGame(const QMoMGamePtr& game)
    {
        m_game = game;
    }
    void setUnit(const QMoMUnitPtr& momUnit);
    void setFrameNr(int frameNr)
    {
        m_frameNr = frameNr;
    }

private slots:
    void slot_actionUnitView();

private:
    int calcHeading() const;
    void drawBattleUnit(QPainter* painter, const QMoMImagePtr& imageUnit);
    void drawOverlandUnit(QPainter* painter, const QMoMImagePtr& imageUnit);
    void timerEvent(QTimerEvent*);

    QMoMGamePtr m_game;
    QMoMUnitPtr m_momUnit;
    bool m_isBattlefield;
    int m_frameNr;
    int m_idTimer;
};

}

#endif // QMOMUNITTILE_H
