// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef QMOMLBX_H_
#define QMOMLBX_H_

#include <QImage>
#include <QVector>

#include "MoMCommon.h"
#include "QMoMAnimation.h"
#include "QMoMSharedPointers.h"

namespace MoM
{

typedef QVector<QRgb> QMoMPalette;

const uint8_t gTRANSPARENT_COLOR = 0;
const uint8_t gSHADOW_COLOR = 232;
const uint8_t gFIRST_BANNER_COLOR = 215; // Either 215 or 216. Apparently only 3 colors are actually used (216-218).
const uint8_t gCOUNT_BANNER_COLOR = 4;

bool convertImagesToLbx(const QMoMAnimation& images, std::vector<uint8_t>& dataBuffer, const std::string& context);

inline bool convertImageToLbx(const QMoMImagePtr& image, std::vector<uint8_t>& dataBuffer, const std::string& context)
{
    return convertImagesToLbx(QMoMAnimation(1, image), dataBuffer, context);
}

bool convertLbxToImages(const uint8_t* data, size_t size, const QMoMPalette& defaultColorTable, QMoMAnimation& images, const std::string& context);

inline QMoMImagePtr convertLbxToImage(const uint8_t* data, size_t size, const QMoMPalette& defaultColorTable, const std::string& context)
{
    QMoMAnimation images;
    (void)convertLbxToImages(data, size, defaultColorTable, images, context);
    if (images.empty())
    {
        return QMoMImagePtr();
    }
    else
    {
        return images[0];
    }
}

void convertLbxToPalette(const uint8_t* dataPalette, QMoMPalette& colorTable);

}

#endif
