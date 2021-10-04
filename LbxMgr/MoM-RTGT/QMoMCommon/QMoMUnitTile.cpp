// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2011-09-30
// ---------------------------------------------------------------------------

#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

#include <math.h>

#include "DialogAddUnit.h"
#include "MainWindow.h"
#include "QMoMResources.h"
#include "QMoMUtility.h"

#include "QMoMUnitTile.h"

namespace MoM
{

const int gUnitFigureOffsets[gMAX_FIGURES_IN_UNIT][gMAX_FIGURES_IN_UNIT][2] =
{
    { {1, 8} },
    { {-7, 9}, {7, 9} },
    { {0, 4}, {-6, 10}, {7, 10} },
    { {1, 4}, {-7, 8}, {8, 8}, {1, 11} },
    { {1, 4}, {-7, 8}, {1, 8}, {8, 8}, {1, 11} },
    { {1, 4}, {4, 7}, {-8, 8}, {9, 8}, {-3, 9}, {1, 11} },
    { {1, 4}, {6, 6}, {-8, 8}, {1, 8}, {10, 8}, {-3, 11}, {1, 11} },
    { {1, 4}, {6, 6}, {-2, 7}, {-8, 8}, {10, 8}, {3, 9}, {-3, 11}, {1, 11} },
};

QMoMUnitTile::QMoMUnitTile(bool isBattlefield) :
	QObject(),
    QGraphicsItem(),
    m_momUnit(),
    m_isBattlefield(isBattlefield),
    m_frameNr(0),
    m_idTimer(-1)
{
}

QMoMUnitTile::~QMoMUnitTile()
{
}

QRectF QMoMUnitTile::boundingRect() const
{
    if (m_isBattlefield)
    {
        return QRectF(-28/2, -30, 28, 30);
    }
    else
    {
        return QRectF(0, 0, 20, 18);
    }
}

void QMoMUnitTile::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *unitViewAction = menu.addAction("View unit");
    QObject::connect(unitViewAction, SIGNAL(triggered()), this, SLOT(slot_actionUnitView()));
    (void)menu.exec(event->screenPos());
}

void QMoMUnitTile::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if (0 != m_momUnit)
    {
        // Calculate heading
        int heading = calcHeading();

        MoM::Wizard* wizard = 0;
        if (0 != m_game)
        {
            wizard = m_game->getWizard(m_momUnit->getOwner());
        }

        // Get unit image
        eBannerColor banner = BANNER_Green;
        if (0 != wizard)
        {
            banner = wizard->m_BannerColor;
        }
        const QMoMAnimation animationUnit = QMoMResources::instance().getAnimation(m_momUnit->getUnitTypeNr(), heading, banner);
        QMoMImagePtr imageUnit;
        if (!animationUnit.empty() && inRange(m_frameNr, 0, animationUnit.count() - 1))
        {
            imageUnit = animationUnit[m_frameNr % animationUnit.count()];
        }
        QMoMImagePtr imageBack;
        if (!m_isBattlefield && (0 != wizard))
        {
            imageBack = MoM::QMoMResources::instance().getImage(wizard->m_BannerColor);
        }

        // Paint unit image
        if (0 != imageBack)
		{
            painter->drawImage(boundingRect(), *imageBack);
		}
        if (0 != imageUnit)
        {
            if (m_isBattlefield)
            {
                drawBattleUnit(painter, imageUnit);
            }
            else
            {
                drawOverlandUnit(painter, imageUnit);
            }
         }

//        QPixmap pixmapUnit = MoM::QMoMResources::instance().getPixmap(unit->m_Unit_Type, 1);
//        if (pixmapUnit.height() > 2 * rectfTile.height())
//        {
//            double scale = 2 * rectfTile.height() / pixmapUnit.height();
//            pixmapUnit = MoM::QMoMResources::instance().getPixmap(unit->m_Unit_Type, scale);
//        }
    }
}

void QMoMUnitTile::setUnit(const QMoMUnitPtr& momUnit)
{
    m_momUnit = momUnit;

    if (m_idTimer != -1)
    {
        killTimer(m_idTimer);
        m_idTimer = -1;
    }
    m_frameNr = 0;

    if (this->m_isBattlefield)
    {
        m_frameNr = 1;
        if (momUnit->isFlying())
        {
            m_idTimer = startTimer(200);
        }
    }
}

void QMoMUnitTile::slot_actionUnitView()
{
    DialogAddUnit* dialog = new MoM::DialogAddUnit(MainWindow::getInstance());
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->slot_unitChanged(m_momUnit);
    dialog->show();
}

int QMoMUnitTile::calcHeading() const
{
    int heading = -1;
    if (m_isBattlefield)
    {
        int dx = m_momUnit->getBattleUnit().m_xPosHeaded - m_momUnit->getBattleUnit().m_xPos;
        int dy = m_momUnit->getBattleUnit().m_yPosHeaded - m_momUnit->getBattleUnit().m_yPos;
        if ((0 == dx) && (0 == dy))
        {
            heading = 0;
        }
        else
        {
            double angle = atan2((double)dy, (double)dx);
            heading = (int)(angle * 4 / 3.14159 + 3.5 + 10) - 10;
            if (heading < 0)
            {
                heading += 8;
            }
        }
    }
    return heading;
}

void QMoMUnitTile::drawBattleUnit(QPainter *painter, const QMoMImagePtr &imageUnit)
{
    assert(0 != imageUnit);
    assert(m_isBattlefield);

    int maxFigures = MoM::Range(0, m_momUnit->getMaxFigures(), (int)MoM::gMAX_FIGURES_IN_UNIT);
    for (int figureNr = 0; figureNr < maxFigures; ++figureNr)
    {
        QPointF offset(gUnitFigureOffsets[maxFigures - 1][figureNr][0], gUnitFigureOffsets[maxFigures - 1][figureNr][1] - 8);
        QRectF dst = boundingRect();
        dst.translate(offset);
        painter->drawImage(dst, *imageUnit);
    }
}

void QMoMUnitTile::drawOverlandUnit(QPainter *painter, const QMoMImagePtr &imageUnit)
{
    assert(0 != imageUnit);
    assert(!m_isBattlefield);

    // Maintain aspect ratio with the boundingRect()
    QRect src = imageUnit->rect();
    QRectF dst = boundingRect();
    qreal dstHeight = dst.width() * src.height() / src.width();
    if (dstHeight < dst.height())
    {
        dst = QRectF(dst.left(), dst.top() + (dst.height() - dstHeight) / 2, dst.width(), dstHeight);
    }
    else
    {
        qreal dstWidth = dst.height() * src.width() / src.height();
        dst = QRectF(dst.left() + (dst.width() - dstWidth) / 2, dst.top(), dstWidth, dst.height());
    }

    painter->drawImage(dst, *imageUnit);
}

void QMoMUnitTile::timerEvent(QTimerEvent *)
{
    m_frameNr = (m_frameNr + 1) % 3;
    update();
}

}
