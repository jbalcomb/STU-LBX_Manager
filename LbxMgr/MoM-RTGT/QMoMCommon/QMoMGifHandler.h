#ifndef QGIFLIBHANDLER_H
#define QGIFLIBHANDLER_H

#include <QImageIOHandler>
#include <QImage>
#include <QVariant>

#include "QMoMSharedPointers.h"

class QMoMAnimation;

class QMoMGifHandler : public QImageIOHandler
{
public:
    enum AnimationOption {
        Disposal,           ///< See DisposalOption
        Loop,               ///< LoopCount. Value of 0 is infinite.
        Delay,              ///< Units of 0.01 second
        TransparentColor,   ///< Color index. No transparent color is -1.
        AnimationOptionMax
    };

    enum DisposalOption
    {
        DisposalUnspecified,
        DisposalDoNot,
        DisposalBackground,
        DisposalPrevious
    };

#define NO_TRANSPARENT_COLOR	-1


    QMoMGifHandler();
    bool canRead() const;
    bool read(QImage* image);
    bool readAnimation(QMoMAnimation& animation);
    bool write(const QImage& image);
    bool writeAnimation(const QMoMAnimation& animation);

    static bool canRead(QIODevice* device);

    bool supportsOption(ImageOption option) const;
    void setOption(ImageOption option, const QVariant& value);
    QVariant option(ImageOption option) const;
    void setDisposalMode(int value) { m_disposalMode = value; }
    void setAnimationOption(AnimationOption option, const QVariant& value)
    {
        m_animationOptions[option] = value;
    }
    QVariant animationOption(AnimationOption option) const
    {
        return m_animationOptions[option];
    }

private:
    struct ColorMapObject* createColorMap(const QMoMImagePtr& image);

//	QVariant m_description;
	QString m_description;
    int m_disposalMode;
    QVector<QVariant> m_animationOptions;
};

#endif // QGIFLIBHANDLER_H
