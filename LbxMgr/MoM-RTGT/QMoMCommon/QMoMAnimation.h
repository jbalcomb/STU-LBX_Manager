#ifndef QMOMANIMATION_H
#define QMOMANIMATION_H

#include <QVector>
#include <QRgb>

#include "QMoMSharedPointers.h"

/// \brief Container for a vector of shared pointers to QImage instances
class QMoMAnimation : public QVector<QMoMImagePtr>
{
public:
    explicit QMoMAnimation(int size = 0, const QMoMImagePtr& value = QMoMImagePtr());
    QMoMAnimation(const QMoMAnimation& other);
    QMoMAnimation& operator=(const QMoMAnimation& other);
    virtual ~QMoMAnimation();

    void crop();
    bool saveAsGif(const QString& gifFilename);
    void scale(double factor);
};

#endif // QMOMANIMATION_H
