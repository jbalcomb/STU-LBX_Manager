// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef QMOMSHAREDPOINTERS_H
#define QMOMSHAREDPOINTERS_H

#include <QIcon>
#include <QSharedPointer>

#include <MoMGameBase.h>

namespace MoM {
	class MoMGameCustom;
	class MoMGameMemory;
    class MoMGameSave;
    class MoMLbxBase;
    class MoMUnit;
    class QMoMLazyIconBase;
}

class QMoMTreeItemModelBase;


/// \brief Shared pointer to a MoMGameBase
typedef QSharedPointer<MoM::MoMGameBase> QMoMGamePtr;

/// \brief Shared pointer to a MoMGameCustom
typedef QSharedPointer<MoM::MoMGameCustom> QMoMGameCustomPtr;

/// \brief Shared pointer to a MoMGameMemory
typedef QSharedPointer<MoM::MoMGameMemory> QMoMGameMemoryPtr;

/// \brief Shared pointer to a MoMGameSave
typedef QSharedPointer<MoM::MoMGameSave> QMoMGameSavePtr;

/// \brief Shared pointer to a QIcon
typedef QSharedPointer<QIcon> QMoMIconPtr;

/// \brief Shared pointer to a QImage
typedef QSharedPointer<QImage> QMoMImagePtr;

/// \brief Shared pointer to a QMoMLazyIcon
typedef QSharedPointer<MoM::QMoMLazyIconBase> QMoMLazyIconPtr;

/// \brief Shared pointer to a QTimer
typedef QSharedPointer<QTimer> QMoMTimerPtr;

/// \brief Shared pointer to a QMoMTreeItemBase
typedef QSharedPointer<QMoMTreeItemModelBase> QMoMTreeItemPtr;

/// \brief Shared pointer to a MoMUnit
typedef QSharedPointer<MoM::MoMUnit> QMoMUnitPtr;

typedef QSharedPointer<MoM::MoMLbxBase> QMoMLbxPtr;

#endif // QMOMSHAREDPOINTERS_H
