// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#pragma once

#include <QDialog>
#include <QList>
#include <QMap>

#include "MoMLocation.h"
#include "MoMTemplate.h"
#include "QMoMDialogBase.h"

class QGraphicsItem;
class QGraphicsScene;
class QTreeWidget;
class QTreeWidgetItem;

namespace Ui {
    class DialogBuildTree;
}

namespace MoM
{
class QMoMBuildingScene;

class DialogBuildingTree : public QMoMDialogBase
{
    Q_OBJECT

public:
    explicit DialogBuildingTree(QWidget *parent = 0);
    ~DialogBuildingTree();

private:
    std::unique_ptr<QMoMBuildingScene> m_scene;
    QMoMTimerPtr m_timer;

    std::unique_ptr<Ui::DialogBuildTree> ui;

private slots:
    void on_comboBox_Plane_currentIndexChanged(int index);
    void on_treeWidget_Tile_customContextMenuRequested(const QPoint &pos);
    void on_verticalSlider_Zoom_valueChanged(int value);
    void slot_addressCalculator();
    void slot_gameChanged(const QMoMGamePtr& game);
    void slot_gameUpdated();
    void slot_itemAction();
    void slot_tileChanged(const MoM::MoMLocation& loc);
    void slot_tileDragged(const MoM::MoMLocation& locFrom, const MoM::MoMLocation& locTo);
    void slot_tileSelected(const MoM::MoMLocation& loc);
    void slot_timerAnimations();
};

}
