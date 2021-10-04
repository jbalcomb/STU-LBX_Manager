// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef DIALOGTOOLS_H
#define DIALOGTOOLS_H

#include <QStatusBar>

#include "QMoMDialogBase.h"

namespace Ui {
class DialogTools;
}

class DialogTools : public QMoMDialogBase
{
    Q_OBJECT
    
public:
    explicit DialogTools(QWidget *parent = 0);
    ~DialogTools();
   
public slots:
    void slot_gameChanged(const QMoMGamePtr& game);
    void slot_gameUpdated();

private slots:
    void on_pushButton_ApplyBuildQueues_clicked();
    void on_pushButton_CatnipMod_clicked();
    void on_pushButton_LucernMod_clicked();
    void on_pushButton_RepopLairs_clicked();
    void on_pushButton_SelectInitialSpells_clicked();

    void on_pushButton_ExternalAI_clicked();

    void on_pushButton_ConjureArtifact_clicked();

    void on_pushButton_SelectRaces_clicked();

    void on_pushButton_BuildingTree_clicked();

private:
	QStatusBar* statusBar();

    Ui::DialogTools *ui;
};

#endif // DIALOGTOOLS_H
