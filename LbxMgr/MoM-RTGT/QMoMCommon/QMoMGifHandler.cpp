#include "QMoMGifHandler.h"
#include <QDebug>
#include <QVariant>
#include <gif_lib.h>
#include <string.h>		// memset
#include "QMoMAnimation.h"

//extern int _GifError;

static const int InterlacedOffset[] = { 0, 4, 2, 1 };	/* The way Interlaced image should */
static const int InterlacedJumps[] = { 8, 8, 4, 2 };	/* be read - offsets and jumps... */

int doOutput(GifFileType* gif, const GifByteType * data, int i)
{
	QIODevice* out = (QIODevice*)gif->UserData;
//	qDebug("given %d bytes to write; device is writeable? %d", i, out->isWritable());
	return out->write((const char*)data, i);
}

int doInput(GifFileType* gif, GifByteType* data, int i)
{
	QIODevice* in = (QIODevice*)gif->UserData;
	return in->read((char*)data, i);
}

QMoMGifHandler::QMoMGifHandler() :
    QImageIOHandler(),
    m_disposalMode(DISPOSE_PREVIOUS),
    m_animationOptions(AnimationOptionMax)
{
    m_animationOptions[Loop] = 0;
    m_animationOptions[Disposal] = DISPOSE_BACKGROUND;
    m_animationOptions[Delay] = 0;
    m_animationOptions[TransparentColor] = 0;
}

bool QMoMGifHandler::canRead () const
{
	if (canRead(device())) {
		setFormat("gif");
		return true;
	}
	return false;
}

bool QMoMGifHandler::read ( QImage * image )
{
    QMoMAnimation animation;
    bool result = readAnimation(animation);
    if (!animation.empty())
    {
        *image = *animation.first();
    }
    return result;
}

bool QMoMGifHandler::readAnimation(QMoMAnimation& animation)
{
	// The contents of this function are based on gif2rgb.c, from the giflib source.
//    qDebug("QGIFLibHandler::read into image with size %d x %d",
//           image->size().width(), image->size().height());
    int error = 0;
    GifFileType* gifFile = DGifOpen(device(), doInput, &error);
    if (!gifFile) {
        qDebug("DGifOpen() failed with error %d", error);
        return false;
    }
    qDebug("dimensions %d x %d", gifFile->SWidth, gifFile->SHeight);

    animation.clear();

	GifRecordType recordType;
	ColorMapObject* ColorMap;

	int	i, row, imageNum = 0, topRow, leftCol, width, height;
	int transColor = -1;
    int ccount;
    QMoMImagePtr image(new QImage);
	do
	{
		DGifGetRecordType(gifFile, &recordType);
		switch (recordType)
		{
		case IMAGE_DESC_RECORD_TYPE:
			if (DGifGetImageDesc(gifFile) == GIF_ERROR)
			{
                qWarning("QGIFLibHandler::read: error %d", gifFile->Error);
				return false;
			}
			topRow = gifFile->Image.Top; /* Image Position relative to Screen. */
			leftCol = gifFile->Image.Left;
			width = gifFile->Image.Width;
			height = gifFile->Image.Height;
			qDebug("Image %d at (%d, %d) [%dx%d]",
				++imageNum, leftCol, topRow, width, height);
			if (gifFile->Image.Left + width > gifFile->SWidth ||
				   gifFile->Image.Top + height > gifFile->SHeight)
			{
				qCritical("Image %d is not confined to screen dimension, aborted.", imageNum);
				return false;
			}

            image = QMoMImagePtr(new QImage(gifFile->SWidth, gifFile->SHeight, QImage::Format_Indexed8));

			// Pre-fill with background color
			qDebug("background color is at index %d", gifFile->SBackGroundColor);
			image->fill(gifFile->SBackGroundColor);

            // Now read the image datas
			if (gifFile->Image.Interlace)
			{
				/* Need to perform 4 passes on the images: */
				for (i = 0; i < 4; i++)
					for (row = topRow + InterlacedOffset[i]; row < topRow + height;
								 row += InterlacedJumps[i])
					{
						if (DGifGetLine(gifFile, image->scanLine(row), width) == GIF_ERROR)
						{
                            qWarning("QGIFLibHandler::read: error %d", gifFile->Error);
							return false;
						}
//						else
//							qDebug("got row %d: %d %d %d %d %d %d %d %d ...", row,
//								image->scanLine(row)[0], image->scanLine(row)[1], image->scanLine(row)[2], image->scanLine(row)[3],
//								image->scanLine(row)[4], image->scanLine(row)[5], image->scanLine(row)[6], image->scanLine(row)[7]);
					}
			}
			else
			{
				for (row = 0; row < height; row++)
				{
					if (DGifGetLine(gifFile, image->scanLine(row), width) == GIF_ERROR)
					{
                        qWarning("QGIFLibHandler::read: error %d", gifFile->Error);
						return false;
					}
//					else
//						qDebug("got row %d: %d %d %d %d %d %d %d %d ...", row,
//							image->scanLine(row)[0], image->scanLine(row)[1], image->scanLine(row)[2], image->scanLine(row)[3],
//							image->scanLine(row)[4], image->scanLine(row)[5], image->scanLine(row)[6], image->scanLine(row)[7]);
				}
			}

            //	BackGround = gifFile->SBackGroundColor;
            ColorMap = (gifFile->Image.ColorMap
                ? gifFile->Image.ColorMap
                : gifFile->SColorMap);
            if (!ColorMap)
            {
                qWarning("QGIFLibHandler::read: Image does not have a colormap");
                return false;
            }
            ccount = ColorMap->ColorCount;
            image->setColorCount(ccount);
            for (i = 0; i < ccount; ++i)
            {
                GifColorType gifColor = ColorMap->Colors[i];
                QRgb color = gifColor.Blue | (gifColor.Green << 8) | (gifColor.Red << 16);
                // If this is not the transparent color,
                // set the alpha to opaque.
                if (i != transColor)
                    color |= 0xff << 24;
        //		qDebug("color %d: 0x%X", i, color);
                image->setColor(i, color);
            }

            animation.append(image);

			break;
		case EXTENSION_RECORD_TYPE:
			{
				int extCode;
				GifByteType* extData;
				/* Skip any extension blocks in file: */
				if (DGifGetExtension(gifFile, &extCode, &extData) == GIF_ERROR)
				{
                    qWarning("QGIFLibHandler::read: error %d", gifFile->Error);
					return false;
				}
				while (extData != NULL)
				{
					int len = extData[0];
					switch (extCode)
					{
					case GRAPHICS_EXT_FUNC_CODE:	// Graphics control extension
						qDebug("graphics control: %x %x %x %x %x",
							   extData[0], extData[1], extData[2], extData[3], extData[4]);
						// Should be block size, packed fields, delay time,
						// transparent color, block terminator
						// see doc/gif89.txt in libgif source package
						// If the trans bit is set in packed fields,
						// then set the trans color to the one given
						if (extData[1] & 0x01)
						{
							transColor = extData[3];
							qDebug("transparent color is at index %d", transColor);
							/// @todo is it correct to override default fill color?
//							image->fill(transColor);
						}
						break;
					case COMMENT_EXT_FUNC_CODE:
						{
							QByteArray comment((char*)(extData + 1), len);
//							qDebug("comment of len %d: \"%s\"", len, comment.constData());
							image->setText("Description", comment);
						}
						break;
					case PLAINTEXT_EXT_FUNC_CODE:
						break;
					}
					if (DGifGetExtensionNext(gifFile, &extData) == GIF_ERROR)
					{
                        qWarning("QGIFLibHandler::read: error %d", gifFile->Error);
						return false;
					}
				}
			}
			break;
		case TERMINATE_RECORD_TYPE:
			break;
		default:
			break;
		}
	}
	while (recordType != TERMINATE_RECORD_TYPE);

    return true;
}

bool QMoMGifHandler::canRead(QIODevice *device)
{
	if (!device) {
		qWarning("QGIFLibHandler::canRead() called with no device");
		return false;
	}

	char head[6];
	if (device->peek(head, sizeof(head)) == sizeof(head))
		return qstrncmp(head, "GIF87a", 6) == 0
			|| qstrncmp(head, "GIF89a", 6) == 0;
	return false;
}

bool QMoMGifHandler::write(const QImage & image)
{
    QMoMAnimation animation;
    animation.append(QMoMImagePtr(new QImage(image)));
    return writeAnimation(animation);
}

bool QMoMGifHandler::writeAnimation(const QMoMAnimation& animation)
{
    if (animation.empty())
        return false;

    QMoMImagePtr toWrite = animation.front();
	/// @todo how to specify dithering method
    qDebug("toWrite %dx%d numColors %d", toWrite->width(), toWrite->height(), toWrite->colorCount());
    if (toWrite->colorCount() == 0 || toWrite->colorCount() > 256)
    {
        qDebug() << "Converting to QImage::Format_Indexed8";
        toWrite = QMoMImagePtr(new QImage(toWrite->convertToFormat(QImage::Format_Indexed8)));
    }

	/// @todo write to m_device
    int gifError = 0;
    GifFileType * gif = EGifOpen(device(), doOutput, &gifError);
//	GifFileType* gif = EGifOpenFileName("/tmp/out.gif", 0);
    if (!gif) {
        qDebug("DGifOpen() failed with error %d", gifError);
        return false;
    }
    qDebug() << "EGifSetGifVersion";
    EGifSetGifVersion(gif, true); // 89a
    /// @todo how to specify background

    ColorMapObject* colorMap = createColorMap(toWrite);
    if (EGifPutScreenDesc(gif, toWrite->width(), toWrite->height(), 256, 0, colorMap) == GIF_ERROR)
        qCritical("EGifPutScreenDesc returned error %d", gif->Error);
    GifFreeMapObject(colorMap);
    colorMap = NULL;

	QVariant descText = option(QImageIOHandler::Description);
	if (descText.type() == QVariant::String)
	{
		QString comment = descText.toString();
		// Will be something like "Description: actual text" or just
		// ": actual text", so remove everything leading up to and
		// including the first colon and the space following it.
		int idx = comment.indexOf(": ");
		if (idx >= 0)
			comment.remove(0, idx + 2);
//		qDebug() << "comment:" << comment;
		if (!comment.isEmpty())
			EGifPutComment(gif, comment.toUtf8().constData());
	}
//	else
//		qDebug("description is of qvariant type %d", descText.type());


    // Loop image
    int loop_count = m_animationOptions[QMoMGifHandler::Loop].toInt();
    char nsle[12] = "NETSCAPE2.0";
    char subblock[3];
    subblock[0] = 1;
    subblock[2] = loop_count % 256;
    subblock[1] = loop_count / 256;
    if (EGifPutExtensionLeader(gif, APPLICATION_EXT_FUNC_CODE) == GIF_ERROR) {
        qCritical("EGifPutExtensionLeader returned error %d", gif->Error);
    }
    if (EGifPutExtensionBlock(gif, 11, nsle) == GIF_ERROR) {
        qCritical("EGifPutExtensionBlock returned error %d", gif->Error);
    }
    if (EGifPutExtensionBlock(gif, 3, subblock) == GIF_ERROR) {
        qCritical("EGifPutExtensionBlock returned error %d", gif->Error);
    }
    if (EGifPutExtensionTrailer(gif) == GIF_ERROR) {
        qCritical("EGifPutExtensionTrailer returned error %d", gif->Error);
    }

	/// @todo foreach of multiple images in an animation...
    for (int imageNr = 0; imageNr < animation.count(); ++imageNr)
    {
        if (imageNr > 0)
        {
            toWrite = animation[imageNr];
            /// @todo how to specify dithering method
            if (toWrite->colorCount() == 0 || toWrite->colorCount() > 256)
            {
                toWrite = QMoMImagePtr(new QImage(toWrite->convertToFormat(QImage::Format_Indexed8)));
            }
        }

        GraphicsControlBlock GCB = { 0, 0, 0, 0 };
        GCB.DisposalMode        = m_animationOptions[QMoMGifHandler::Disposal].toInt(); // what to do with the background of the image
        GCB.UserInputFlag       = 0;    // whether user input is expected
        GCB.DelayTime           = m_animationOptions[QMoMGifHandler::Delay].toInt();    // 0.01 sec increments
        GCB.TransparentColor    = m_animationOptions[QMoMGifHandler::TransparentColor].toInt();    // palette index (default is not transparent)
        GifByteType GifExtension[4] = { 0 };
        size_t sizeExtension = EGifGCBToExtension(&GCB, GifExtension);
        if (sizeExtension != sizeof(GifExtension)) {
            qCritical("EGifGCBToExtension returnd wrong size %u instead of %u",
                      static_cast<unsigned>(sizeExtension), static_cast<unsigned>(sizeof(GifExtension)));
        }
        EGifPutExtension(gif, GRAPHICS_EXT_FUNC_CODE, sizeExtension, GifExtension);

        ColorMapObject* colorMap = createColorMap(toWrite);
        if (EGifPutImageDesc(gif, 0, 0, toWrite->width(), toWrite->height(), 0, colorMap) == GIF_ERROR)
            qCritical("EGifPutImageDesc returned error %d", gif->Error);
        GifFreeMapObject(colorMap);
        colorMap = NULL;

        int lc = toWrite->height();
        int llen = toWrite->width();
    //	qDebug("will write %d lines, %d bytes each", lc, llen);
        for (int l = 0; l < lc; ++l)
        {
            uchar* line = toWrite->scanLine(l);
            if (EGifPutLine(gif, (GifPixelType*)line, llen) == GIF_ERROR)
            {
                int i = gif->Error;
                qCritical("EGifPutLine returned error %d", i);
            }
        }
    }

	EGifCloseFile(gif);
    return true;
}

bool QMoMGifHandler::supportsOption ( ImageOption option ) const
{
//	qDebug("supportsOption %d", option);
	switch (option)
	{
		// These are relevant only for reading
		case QImageIOHandler::ImageFormat:
		case QImageIOHandler::Size:
		// This is relevant for both reading and writing
		case QImageIOHandler::Description:
			return true;
			break;
		default:
			return false;
	}
}

void QMoMGifHandler::setOption ( ImageOption option, const QVariant & value )
{
//	qDebug("setOption given option %d, variant of type %d", option, value.type());
	if (option == QImageIOHandler::Description)
		m_description = value.toString();
}

QVariant QMoMGifHandler::option( ImageOption option ) const
{
	switch (option)
	{
		case QImageIOHandler::ImageFormat:
			return QVariant();	/// @todo
			break;
		case QImageIOHandler::Size:
			return QVariant();	/// @todo
			break;
		case QImageIOHandler::Description:
			return QVariant(m_description);
			break;
		default:
			return QVariant();
    }
}

struct ColorMapObject* QMoMGifHandler::createColorMap(const QMoMImagePtr& image)
{
    QVector<QRgb> colorTable = image->colorTable();

    // numColors must be a power of 2
    int numColors = 1 << GifBitSize(image->colorCount());
    GifColorType* colorValues = (GifColorType*)malloc(numColors * sizeof(GifColorType));
    int c = 0;
    for(; c < image->colorCount(); ++c)
    {
//qDebug("color %d has %02X%02X%02X", c, qRed(colorTable[c]), qGreen(colorTable[c]), qBlue(colorTable[c]));
        colorValues[c].Red = qRed(colorTable[c]);
        colorValues[c].Green = qGreen(colorTable[c]);
        colorValues[c].Blue = qBlue(colorTable[c]);
    }
    // In case we had an actual number of colors that's not a power of 2,
    // fill the rest with something (black perhaps).
    for (; c < numColors; ++c)
    {
        colorValues[c].Red = 0;
        colorValues[c].Green = 0;
        colorValues[c].Blue = 0;
    }

    ColorMapObject* colorMap = GifMakeMapObject(numColors, colorValues);

    colorMap->BitsPerPixel = 8;	/// @todo based on numColors (or not? we did ask for Format_Indexed8, so the data is always 8-bit, right?)

    return colorMap;
}
