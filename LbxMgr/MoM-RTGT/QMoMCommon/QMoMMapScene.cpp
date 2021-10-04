#include <QDebug>
#include <QGraphicsSceneMouseEvent>

#include <QMoMMapTile.h>

#include "QMoMMapScene.h"

namespace MoM
{

const int gBATTLE_SQUARE_WIDTH = 32;
const int gBATTLE_SQUARE_HEIGHT = 8;

QMoMMapScene::QMoMMapScene(MoM::ePlane plane, bool isBattlefield, QObject *parent) :
    QGraphicsScene(parent),
    m_plane(plane),
    m_isBattlefield(isBattlefield),
    m_dragFrom()
{
    QRectF rectfTile = MoM::QMoMMapTile(MoMLocation()).boundingRect();
    if (m_isBattlefield)
    {
//        setSceneRect(0, 0, (11 - 1) * gBATTLE_SQUARE_WIDTH, (22 - 1) * gBATTLE_SQUARE_HEIGHT);
        setSceneRect(0, 0, 320, 200);
    }
    else
    {
        setSceneRect(0, 0, MoM::gMAX_MAP_COLS * rectfTile.width(), MoM::gMAX_MAP_ROWS * rectfTile.height());
    }
}

void QMoMMapScene::addItemAtLocation(QGraphicsItem* item, const MoMLocation& location)
{
    QPointF scenePos;
    convertLocationToScenePos(location, scenePos);
    item->setPos(scenePos);
    addItem(item);
}

QGraphicsPixmapItem* QMoMMapScene::addPixmapAtLocation(const QPixmap& pixmap, const MoM::MoMLocation& location)
{
    QGraphicsPixmapItem* item = addPixmap(pixmap);
    QPointF scenePos;
    convertLocationToScenePos(location, scenePos);
    if (m_isBattlefield)
    {
        item->setOffset(- pixmap.width() / 2, -pixmap.height());
    }
    item->setPos(scenePos);
    return item;
}

// (row,col) in (0..21,0..10)
// xpos = row / 2 + col
// ypos = (row + 1) / 2 + 9 - col
// xpel = 32 * col - 16 * (row % 2)
// ypel = 8 * row - 8
// index = ypos * 21 + xpos

// row = xpos + ypos - 9
// col = (xpos - ypos + 9) / 2
// xpel = (xpos - ypos + 9) * 16
// ypel = (xpos + ypos - 9) * 8
// xpos = xpel / 32 + ypel / 16
// ypos = 9 - xpel / 32 + ypel / 16

void QMoMMapScene::convertLocationToScenePos(const MoMLocation& location, QPointF& scenePos) const
{
    int x = location.m_XPos;
    int y = location.m_YPos;

    if (m_isBattlefield)
    {
        double xpel = (x - y + 11) / 2.0 * gBATTLE_SQUARE_WIDTH;
        double ypel = (x + y - 8) * gBATTLE_SQUARE_HEIGHT;

        scenePos = QPointF(xpel, ypel);
    }
    else
    {
        QRectF rectfTile = MoM::QMoMMapTile(MoMLocation()).boundingRect();
        scenePos = QPointF(x * rectfTile.width(), y * rectfTile.height());
    }
}

void QMoMMapScene::convertScenePosToLocation(const QPointF& scenePos, MoMLocation& location) const
{
    if (m_isBattlefield)
    {
        // xpos = xpel / 32 + ypel / 16
        // ypos = 9 - xpel / 32 + ypel / 16
        double xpel = scenePos.x();
        double ypel = scenePos.y();
        int8_t xpos = (int8_t)(xpel / gBATTLE_SQUARE_WIDTH + ypel / (gBATTLE_SQUARE_HEIGHT * 2) - 0.5);
        int8_t ypos = (int8_t)(10.5 - xpel / gBATTLE_SQUARE_WIDTH + ypel / (gBATTLE_SQUARE_HEIGHT * 2));
        location = MoM::MoMLocation(xpos, ypos, m_plane, MoMLocation::MAP_battle);
    }
    else
    {
        QRectF rectfTile = MoM::QMoMMapTile(MoMLocation()).boundingRect();
        location = MoM::MoMLocation(scenePos.x() / rectfTile.width(), scenePos.y() / rectfTile.height(), m_plane, MoMLocation::MAP_overland);
    }
}

int QMoMMapScene::convertScenePosToZValue(const QPointF &scenePos) const
{
    int zvalue = 0;
    if (m_isBattlefield)
    {
        zvalue = 8320 * scenePos.y() + 17 * scenePos.x();
    }
    return zvalue;
}

void QMoMMapScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    MoM::MoMLocation loc;
    convertScenePosToLocation(event->scenePos(), loc);

    emit signal_tileChanged(loc);
}

void QMoMMapScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    convertScenePosToLocation(event->scenePos(), m_dragFrom);
}

void QMoMMapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    MoM::MoMLocation loc;
    convertScenePosToLocation(event->scenePos(), loc);

    if (loc == m_dragFrom)
    {
        emit signal_tileSelected(loc);
    }
    else
    {
        //qDebug() << "Drag from " << m_dragFrom.m_XPos << m_dragFrom.m_YPos << " to " << loc.m_XPos << loc.m_YPos;
        emit signal_tileDragged(m_dragFrom, loc);
    }
}

}
