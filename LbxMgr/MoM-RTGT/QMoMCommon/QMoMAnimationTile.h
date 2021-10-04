#ifndef QMOMANIMATIONTILE_H
#define QMOMANIMATIONTILE_H

#include <QGraphicsItem>

#include <QMoMAnimation.h>

class QMoMAnimationTile : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit QMoMAnimationTile(const QMoMAnimation& animation);
    
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
    
public slots:

private:
    void timerEvent(QTimerEvent *event);

private:
    QMoMAnimation m_animation;
    int m_frameNr;
};

#endif // QMOMANIMATIONTILE_H
