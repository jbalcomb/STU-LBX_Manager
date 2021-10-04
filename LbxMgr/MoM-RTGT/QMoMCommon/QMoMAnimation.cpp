#include "QMoMAnimation.h"

#include <QDebug>
#include <QFile>

#include "QMoMGifHandler.h"

QMoMAnimation::QMoMAnimation(int size, const QMoMImagePtr& value)
    : QVector<QMoMImagePtr>(size, value)
{
}

QMoMAnimation::QMoMAnimation(const QMoMAnimation& other)
    : QVector<QMoMImagePtr>(other)
{
}

QMoMAnimation& QMoMAnimation::operator=(const QMoMAnimation& other)
{
    if (&other != this)
    {
        QVector<QMoMImagePtr>::operator=(other);
    }
    return *this;
}

QMoMAnimation::~QMoMAnimation()
{
}

void QMoMAnimation::crop()
{
    // TODO: Handle cases that picture is not 8-bit

    QRect rect(1000, 1000, -1000, -1000);
    for (int frameNr = 0; frameNr < this->count(); ++frameNr)
    {
        const QMoMImagePtr image = (*this)[frameNr];
        for (int y = 0; y < image->height(); ++y)
        {
            const uchar* line = image->scanLine(y);
            for (int x = 0; x < image->width(); ++x)
            {
                if (line[x] == 0)
                    continue;
                rect.setLeft(MoM::Min(rect.left(), x));
                rect.setTop(MoM::Min(rect.top(), y));
                rect.setRight(MoM::Max(rect.right(), x));
                rect.setBottom(MoM::Max(rect.bottom(), y));
            }
        }
    }

    for (int frameNr = 0; frameNr < this->count(); ++frameNr)
    {
        const QMoMImagePtr src = (*this)[frameNr];
        QMoMImagePtr dst(new QImage(rect.width(), rect.height(), QImage::Format_Indexed8));
        dst->setColorTable(src->colorTable());
        for (int y = 0; y < dst->height(); ++y)
        {
            const uchar* srcLine = src->scanLine(rect.top() + y);
            uchar* dstLine = dst->scanLine(y);
            memcpy(dstLine, srcLine + rect.left(), rect.width());
        }

        (*this)[frameNr] = dst;
    }
}

bool QMoMAnimation::saveAsGif(const QString& gifFilename)
{
    QFile fileGif(gifFilename);
    //qDebug() << "Writing to " << fileGif.fileName();
    if (!fileGif.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    QMoMGifHandler gifHandler;
    gifHandler.setDevice(&fileGif);
    // TODO: Make options accessible
    gifHandler.setAnimationOption(QMoMGifHandler::Loop, 0);
    gifHandler.setAnimationOption(QMoMGifHandler::TransparentColor, 0);
    gifHandler.setAnimationOption(QMoMGifHandler::Disposal, 2);
    gifHandler.setAnimationOption(QMoMGifHandler::Delay, 20);
    if (!gifHandler.writeAnimation(*this))
        return false;

    return true;
}

void QMoMAnimation::scale(double factor)
{
    for (int frameNr = 0; frameNr < this->count(); ++frameNr)
    {
        const QMoMImagePtr src = (*this)[frameNr];
        (*this)[frameNr] = QMoMImagePtr(new QImage(src->scaled(src->width() * factor + 0.5, src->height() * factor + 0.5, Qt::IgnoreAspectRatio, Qt::FastTransformation)));
    }
}

