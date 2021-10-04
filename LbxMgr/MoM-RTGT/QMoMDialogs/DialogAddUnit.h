// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef DIALOGADDUNIT_H
#define DIALOGADDUNIT_H

#include "MoMTemplate.h"
#include "QMoMResources.h"
#include "QMoMDialogBase.h"
#include "QMoMUtility.h"

class QAbstractButton;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSimpleTextItem;
class UnitModel;

namespace Ui {
    class DialogAddUnit;
}

namespace MoM {
    class MoMGameBase;
    class MoMUnit;
}

namespace MoM {

class DialogAddUnit : public QMoMDialogBase
{
    Q_OBJECT
    
public:
    explicit DialogAddUnit(QWidget *parent);
    ~DialogAddUnit();
    
public slots:
	void slot_gameChanged(const QMoMGamePtr& game);
	void slot_gameUpdated();
    void slot_unitChanged(const QMoMUnitPtr& unit);

private slots:
    void on_comboBox_Unit_currentIndexChanged(int index);
    void on_pushButton_Summon_clicked();

private:
    QGraphicsSimpleTextItem* addText(const QPointF& pos, const QString& text, const QString& helpText = "");

	void displayItem(QPointF &pos, MoM::eSlot_Type16 slotType, const MoM::Item* momItem);
	void displayLevel(QPointF& pos, const std::string& levelName, int level, int experience);

    void displaySectionBasicAttributes(QPointF& pos);
    void displaySectionSpecials(QPointF& pos);
    void displaySectionTop();

    template<typename Enum>
    void displaySpecial(QPointF& pos, const Enum special, int specialValue)
    {
        QString specialName = prettyQStr(special);
        std::string helpText = MoM::QMoMResources::instance().getHelpText(special);
        QPixmap pixmap = QMoMResources::instance().getPixmap(special);
        displaySpecial(pos, specialName, specialValue, pixmap, helpText.c_str());
    }
    void displaySpecial(QPointF& pos, const QString& specialName, int specialValue, const QString& pixmapDir, const QString& helpText = "")
    {
        QPixmap pixmap(pixmapDir + specialName + ".gif");
        displaySpecial(pos, specialName, specialValue, pixmap, helpText);
    }
    void displaySpecial(QPointF& pos, const QString& specialName, int specialValue, const QPixmap& pixmap, const QString& helpText);
    void displaySpecial(QPointF& pos, const QString& specialName, int specialValue, const QMoMAnimation& animation, const QString& helpText);

    void displayStrength(QPointF& pos, int strength, int bonusStrength, const QString& imageBaseName, const QString& helpText);
    void displayToHit(QPointF& pos, int toHit, const QString& labelText);
	void update();

	// CONFIG
    QSharedPointer<MoM::MoMUnit> m_unit;

	// STATUS
    bool m_updating;
    int m_labelWidth;
    int m_lineHeight;
    int m_pictureHeight;
    QGraphicsScene* m_sceneUnit;
    Ui::DialogAddUnit *ui;
};

}

#endif // DIALOGADDUNIT_H
