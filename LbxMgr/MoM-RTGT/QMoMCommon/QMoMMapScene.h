#ifndef QMOMMAPSCENE_H
#define QMOMMAPSCENE_H

#include <QGraphicsScene>
#include <QList>
class QGraphicsItem;

#include <MoMLocation.h>
#include <MoMTemplate.h>

namespace MoM
{

class QMoMMapScene : public QGraphicsScene
{
    Q_OBJECT
public:
    QMoMMapScene(MoM::ePlane plane, bool isBattlefield, QObject *parent = 0);

    void addItemAtLocation(QGraphicsItem* item, const MoM::MoMLocation& location);
    QGraphicsPixmapItem* addPixmapAtLocation(const QPixmap& pixmap, const MoM::MoMLocation& location);
    void convertLocationToScenePos(const MoM::MoMLocation& location, QPointF& scenePos) const;
    void convertScenePosToLocation(const QPointF& scenePos, MoM::MoMLocation& location) const;
    int convertScenePosToZValue(const QPointF& scenePos) const;
    void setPlane(MoM::ePlane plane)
    {
        m_plane = plane;
    }

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

public slots:

signals:
    void signal_tileChanged(const MoM::MoMLocation& loc);
    void signal_tileDragged(const MoM::MoMLocation& locFrom, const MoM::MoMLocation& locTo);
    void signal_tileSelected(const MoM::MoMLocation& loc);

private:
    MoM::ePlane m_plane;
    bool m_isBattlefield;
    MoMLocation m_dragFrom;
};

}

#endif // QMOMMAPSCENE_H
