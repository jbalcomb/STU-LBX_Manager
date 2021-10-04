#include <QDebug>
#include <QMouseEvent>

#include "QMapGraphicsView.h"

QMapGraphicsView::QMapGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
}

void QMapGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}
