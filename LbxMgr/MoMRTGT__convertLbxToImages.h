#ifndef LBX_MGR_MOMRTGT_CONVERTLBXTOIMAGES_H
#define LBX_MGR_MOMRTGT_CONVERTLBXTOIMAGES_H

#include <QImage>
#include <QVector>



/* F:\Development_OPC\momrtgt-code\QMoMCommon\QMoMSharedPointers.h */

#include <QIcon>
#include <QSharedPointer>

// #include <MoMGameBase.h>

/// \brief Shared pointer to a QImage
typedef QSharedPointer<QImage> QMoMImagePtr;



/* F:\Development_OPC\momrtgt-code\QMoMCommon\QMoMAnimation.h */

#include <QVector>
#include <QRgb>

// #include "QMoMSharedPointers.h"

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


/* F:\Development_OPC\momrtgt-code\QMoMCommon\QMoMLbx.h */

typedef QVector<QRgb> QMoMPalette;

const uint8_t gTRANSPARENT_COLOR = 0;
const uint8_t gSHADOW_COLOR = 232;
const uint8_t gFIRST_BANNER_COLOR = 215; // Either 215 or 216. Apparently only 3 colors are actually used (216-218).
const uint8_t gCOUNT_BANNER_COLOR = 4;



/* F:\Development_OPC\momrtgt-code\QMoMCommon\QMoMLbx.cpp */
static void dump(const uint8_t* ptr, unsigned n);
static void dumpnl(const uint8_t* ptr, unsigned n);
bool convertLbxToImages(const uint8_t* data, size_t size, const QMoMPalette& defaultColorTable, QMoMAnimation& images, const std::string& context);


#endif  // LBX_MGR_MOMRTGT_CONVERTLBXTOIMAGES_H
