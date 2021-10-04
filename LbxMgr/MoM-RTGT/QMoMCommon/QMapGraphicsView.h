#ifndef QMAPGRAPHICSVIEW_H
#define QMAPGRAPHICSVIEW_H

#include <QGraphicsView>

class QMapGraphicsView : public QGraphicsView
{
public:
    explicit QMapGraphicsView(QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent* event);

signals:
    
public slots:
    
};

#endif // QMAPGRAPHICSVIEW_H
