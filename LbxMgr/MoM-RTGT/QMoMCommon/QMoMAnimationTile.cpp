#include "QMoMAnimationTile.h"

#include <QPainter>

#include <stdlib.h>

QMoMAnimationTile::QMoMAnimationTile(const QMoMAnimation& animation) :
    QObject(),
    QGraphicsItem(),
    m_animation(animation),
    m_frameNr(0)
{
    if (!m_animation.empty())
    {
        m_frameNr = rand() % m_animation.count();
        startTimer(200);    // millisecond timer
    }
}

QRectF QMoMAnimationTile::boundingRect() const
{
    QRectF rect;
    if (!m_animation.empty())
    {
        rect = QRectF(m_animation.first()->rect());
    }
    return rect;
}

void QMoMAnimationTile::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if (m_frameNr >= m_animation.count())
        return;
    QMoMImagePtr image = m_animation[m_frameNr];
    painter->drawImage(0, 0, *image);
}

void QMoMAnimationTile::timerEvent(QTimerEvent*)
{
    assert(m_animation.count() > 0);
    m_frameNr = (m_frameNr + 1) % m_animation.count();
    update();
}
