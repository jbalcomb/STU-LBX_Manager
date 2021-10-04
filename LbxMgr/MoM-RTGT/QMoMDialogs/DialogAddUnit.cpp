// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2012-03-12
// ---------------------------------------------------------------------------

#include "DialogAddUnit.h"
#include "ui_DialogAddUnit.h"

#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <QMessageBox>

#include "MoMController.h"
#include "MoMGenerated.h"
#include "MoMUnit.h"
#include "MoMUtility.h"
#include "QMoMAnimationTile.h"
#include "QMoMResources.h"
#include "QMoMSettings.h"
#include "QMoMSharedPointers.h"

namespace MoM
{

DialogAddUnit::DialogAddUnit(QWidget *parent) :
    QMoMDialogBase(parent),
    m_unit(new MoM::MoMUnit),
    m_updating(false),
    m_labelWidth(),
    m_lineHeight(),
    m_pictureHeight(),
    m_sceneUnit(new QGraphicsScene),
    ui(new Ui::DialogAddUnit)
{
    m_font.setPointSize(14);
    ui->setupUi(this);

    postInitialize();

    // Initalize graphics view AFTER restoring the size of the graphicsView in postInitialize()
    QRectF rectf = ui->graphicsView_Unit->rect();
    m_sceneUnit->setSceneRect(0, 0, rectf.width()-8, rectf.height());
    ui->graphicsView_Unit->setSceneRect(-8, 0, rectf.width(), rectf.height());

    // Use a pixmap as reference for coordinate positions
    QPixmap pixmapSword(":/images/sword_normal.gif");
    m_lineHeight = pixmapSword.height() * 4 / 3;
    QPixmap pixmapPicture(":/units/Healer.gif");
    m_labelWidth = pixmapPicture.width() * 4 / 3;
    m_pictureHeight = pixmapPicture.height() * 4 / 3;

    ui->graphicsView_Unit->setScene(m_sceneUnit);
}

DialogAddUnit::~DialogAddUnit()
{
    preFinalize();
    delete ui;
    delete m_sceneUnit;
}

QGraphicsSimpleTextItem* DialogAddUnit::addText(const QPointF& pos, const QString& text, const QString& helpText)
{
    QGraphicsSimpleTextItem* textItem = m_sceneUnit->addSimpleText(text, m_font);
    textItem->setToolTip(helpText);
    textItem->setBrush(QBrush(Qt::white));
    textItem->setPos(pos);
    return textItem;
}

void DialogAddUnit::displayItem(QPointF &pos, MoM::eSlot_Type16 slotType, const MoM::Item* momItem)
{
    if ((MoM::eSlot_Type16)0 == slotType)
        return;

    QPointF tmpPos = pos;

    QPixmap pixmapSlot = MoM::QMoMResources::instance().getPixmap(slotType, 2.0);
    if (!pixmapSlot.isNull())
    {
        QGraphicsItem* item = m_sceneUnit->addPixmap(pixmapSlot);
        item->setPos(tmpPos);
        tmpPos.rx() += pixmapSlot.width() * 4 / 3;
    }

	QPixmap pixmapItem;
	if (0 != momItem)
	{
		pixmapItem = MoM::QMoMResources::instance().getPixmap(momItem->m_Icon, 2);
		if (!pixmapItem.isNull())
		{
			QGraphicsItem* item = m_sceneUnit->addPixmap(pixmapItem);
			item->setPos(tmpPos);
			tmpPos.rx() += pixmapItem.width() * 4 / 3;
		}

		QString text = QString("%0").arg(momItem->m_Item_Name);
        (void)addText(tmpPos, text);
	}

    pos.ry() += MoM::Max(pixmapItem.height() + 2, 34);
}

void DialogAddUnit::displayLevel(QPointF &pos, const std::string& levelName, int level, int experience)
{
    QGraphicsItem* item = 0;

    QPixmap pixmap(":/images/Level " + QString("%0").arg(level) + ".gif");
    item = m_sceneUnit->addPixmap(pixmap);
    item->setPos(pos);

    QString text = QString("Level %0 (%1 ep)").arg(level).arg(experience);
    if (!levelName.empty())
    {
        text = QString("%0 (level %1 / %2 ep)").arg(levelName.c_str()).arg(level).arg(experience);
    }
    item = addText(pos + QPointF(pixmap.width() * 4 / 3, 0), text);

    pos.ry() += MoM::Max(pixmap.height() + 2, 34);
}

void DialogAddUnit::displaySectionBasicAttributes(QPointF &pos)
{
    MoM::MoMUnit::BaseAttributes actualAttr = m_unit->getActualAttributes();
    MoM::MoMUnit::BaseAttributes bonusAttr = m_unit->getBonusAttributes();

    QPointF posLabel = pos;
    posLabel.rx() = 0;

    (void)addText(posLabel, "Melee", MoM::QMoMResources::instance().getHelpText(MoM::HELP_MELEE).c_str());
    posLabel.ry() += m_lineHeight;
    (void)addText(posLabel, "Range", MoM::QMoMResources::instance().getHelpText(MoM::HELP_RANGE_1).c_str() + QString("\nRANGED TYPE: ") + prettyQStr(m_unit->getRangedType()));
    posLabel.ry() += m_lineHeight;
    (void)addText(posLabel, "Armor", MoM::QMoMResources::instance().getHelpText(MoM::HELP_UNITVIEW_ARMOR).c_str());
    posLabel.ry() += m_lineHeight;
    (void)addText(posLabel, "Resist", MoM::QMoMResources::instance().getHelpText(MoM::HELP_UNITVIEW_RESISTANCE).c_str());
    posLabel.ry() += m_lineHeight;
    (void)addText(posLabel, "Hits", MoM::QMoMResources::instance().getHelpText(MoM::HELP_UNITVIEW_HITS).c_str());
    posLabel.ry() += m_lineHeight;

    QString imageBaseName;
    switch (m_unit->getWeaponType())
    {
    default:
    case MoM::WEAPON_normal:        imageBaseName = "sword"; break;
    case MoM::WEAPON_magic:         imageBaseName = "sword_magic"; break;
    case MoM::WEAPON_mithril:       imageBaseName = "sword_mithril"; break;
    case MoM::WEAPON_adamantium:    imageBaseName = "sword_adamantium"; break;
    }

    pos.rx() = m_labelWidth;

    displayStrength(pos, actualAttr.melee, bonusAttr.melee, imageBaseName, prettyQStr(m_unit->getWeaponType()));

    switch (m_unit->getRangedType())
    {
    case MoM::RANGED_Rock:           imageBaseName = "rock"; break;
    case MoM::RANGED_Arrow:          imageBaseName = "bow"; break;
    case MoM::RANGED_Bullet:         imageBaseName = "rock"; break;
    case MoM::RANGED_Thrown_Weapons: imageBaseName = "thrown"; break;
    default:                         imageBaseName = "fireball"; break;
    }

    if (m_unit->hasMagicalBreathAttack() || m_unit->hasMagicalGazeAttack())
    {
        displayStrength(pos, 0, 0, imageBaseName, prettyQStr(m_unit->getRangedType()));
    }
    else
    {
        displayStrength(pos, actualAttr.ranged, bonusAttr.ranged, imageBaseName, prettyQStr(m_unit->getRangedType()));
    }

    displayStrength(pos, actualAttr.defense, bonusAttr.defense, "shield", MoM::QMoMResources::instance().getHelpText(MoM::HELP_UNITVIEW_ARMOR).c_str());
    displayStrength(pos, actualAttr.resistance, bonusAttr.resistance, "resistance",MoM::QMoMResources::instance().getHelpText(MoM::HELP_UNITVIEW_RESISTANCE).c_str());
    displayStrength(pos,actualAttr.hitpoints, bonusAttr.hitpoints, "heart", MoM::QMoMResources::instance().getHelpText(MoM::HELP_UNITVIEW_HITS).c_str());

    pos.rx() = 0;
}

void DialogAddUnit::displaySectionSpecials(QPointF &pos)
{
    if (m_unit->getCost() > 0)
    {
        QString text = QString("Cost %0").arg(m_unit->getCost());
        if (m_unit->isHero() && (m_unit->getUnitTypeNr() != UNITTYPE_Chosen))
        {
//            ePlayer playerNr = PLAYER_YOU;   // TODO: Other players

            int reqFame = (m_unit->getCost() - 100) / 10;
//            int curFame = m_game->getWizard(playerNr)->m_Fame;
//            // TODO: Check effect of Famous+10 and JustCause+10 in the game
//            // TODO
//            int nrHeroes = 0; //m_game->getNrHiredHeroes(playerNr);
//            int nrPossibleHeroes = 10; //countPossibleHeroes(curFame, books);
//            double chance = (curFame / 25 + 3) / ((nrHeroes + 1) / 2 + 1);
//            if (m_game->getWizard(playerNr)->m_Skills.s.Famous)
//            {
//                chance *= 2;
//            }
//            if (chance > 10)
//            {
//                chance = 10;
//            }
//            if (playerNr != PLAYER_YOU)
//            {
//                chance += 10;
//            }

//            chance = chance * nrPossibleHeroes / (gMAX_HERO_TYPES - 1);

//             When a hero decides to consider you, he looks whether his (cost hero - 100) / 10 is less than or equal to your fame.
//            And also if you have Life books or Death books (if the hero is the Priestess, the Paladin, the Black Knight, or the Necromancer).
//            If you don't meet his requirements he passes you over.

            text = QString("Cost %0 gold (fame %1)").arg(m_unit->getCost()).arg(reqFame);
        }
        else if (m_unit->isNormal())
        {
            text = QString("Cost %0 production").arg(m_unit->getCost());
        }
        else if (m_unit->isSummoned() || (m_unit->getUnitTypeNr() != UNITTYPE_Chosen))
        {
            // TODO: Unit cost is NOT the casting cost.
            //       We should look up the casting cost and mention that.
            //       For now: just leave it out
//            text = QString("Cost %0 (casting %1)").arg(m_unit->getCost());
        }
        else
        {
            // Nothing left
        }
        displaySpecial(pos, text, 0, QPixmap(), "");
    }

    MoM::MoMUnit::ListBuildings listBuildings = m_unit->getRequiredBuildings();
    for (size_t i = 0; i < listBuildings.size(); ++i)
    {
        displaySpecial(pos, listBuildings[i], 0);
    }

    MoM::MoMUnit::ListSpells listSpells = m_unit->getHeroSpells();
    for (size_t i = 0; i < listSpells.size(); ++i)
    {
        displaySpecial(pos, listSpells[i], 0);
    }

    MOM_FOREACH(eItemPower, itemPower, eItemPower_MAX)
    {
        if (m_unit->hasItemPower(itemPower))
        {
            QString specialName = prettyQStr(itemPower);
            displaySpecial(pos, specialName, 0, ":/abilities/", MoM::QMoMResources::instance().getHelpText(itemPower).c_str());  // TODO: Lookup images for item effects
        }
    }

    if (m_unit->hasMissileRangedAttack())
    {
        displaySpecial(pos, "Arrows", m_unit->getMaxRangedShots(), ":/abilities/", MoM::QMoMResources::instance().getHelpText(m_unit->getRangedType()).c_str());  // TODO: Lookup images for item effects
    }
    else if (m_unit->hasMagicalRangedAttack())
    {
//        displaySpecial(pos, m_unit->getRangedType(), 0);
        displaySpecial(pos, prettyQStr(m_unit->getRangedType()), 0,
                       QMoMResources::instance().getAnimation(m_unit->getRangedType()),
                       QMoMResources::instance().getHelpText(m_unit->getRangedType()).c_str());
        if (m_unit->getMaxRangedShots() > 0)
        {
            displaySpecial(pos, "Spells", m_unit->getMaxRangedShots(), ":/abilities/", MoM::QMoMResources::instance().getHelpText(m_unit->getRangedType()).c_str());  // TODO: Lookup images for item effects
        }
    }
    else if (m_unit->hasMagicalBreathAttack())
    {
        displaySpecial(pos, prettyQStr(m_unit->getRangedType()), m_unit->getActualAttributes().ranged, ":/abilities/", MoM::QMoMResources::instance().getHelpText(m_unit->getRangedType()).c_str());    // TODO: Lookup images
    }
    else if (m_unit->hasMagicalGazeAttack())
    {
        int specialValue = m_unit->getGazeModifier();
        if (m_unit->getRangedType() == RANGED_Multiple_Gaze)
        {
            specialValue = m_unit->getActualAttributes().ranged;
        }
        displaySpecial(pos, prettyQStr(m_unit->getRangedType()), specialValue, ":/abilities/", MoM::QMoMResources::instance().getHelpText(m_unit->getRangedType()).c_str());  // TODO: Lookup images for item effects
    }
    else if (m_unit->getMaxRangedShots() > 0)
    {
        displaySpecial(pos, prettyQStr(m_unit->getRangedType()), m_unit->getMaxRangedShots(), ":/abilities/", MoM::QMoMResources::instance().getHelpText(m_unit->getRangedType()).c_str());  // TODO: Lookup images for item effects
    }
    else
    {
        // No ranged attack information
    }

    if (m_unit->getCastingSkillBase() > 0)
    {
        displaySpecial(pos, "Caster", m_unit->getCastingSkillTotal(), ":/abilities/", MoM::QMoMResources::instance().getHelpText(MoM::HELP_SPECIAL_CASTER).c_str());    // TODO: Lookup images
    }
    if (m_unit->getScouting() > 1)
    {
        displaySpecial(pos, "Scouting", m_unit->getScouting(), ":/abilities/", MoM::QMoMResources::instance().getHelpText(MoM::HELP_SPECIAL_SCOUTING).c_str());    // TODO: Lookup images
    }
    if (m_unit->getConstructionSkill() > 0)
    {
        displaySpecial(pos, "Construction", m_unit->getConstructionSkill(), ":/abilities/", MoM::QMoMResources::instance().getHelpText(MoM::HELP_SPECIAL_CONSTRUCTION).c_str());    // TODO: Lookup images
    }

    MoM::Hero_Stats_Initializer stats = m_unit->getHeroStatsInitializer();
    if (stats.m_Nr_Random_picks > 0)
    {
        QString specialName = prettyQStr(stats.m_Random_pick_type);
        displaySpecial(pos, specialName, stats.m_Nr_Random_picks, ":/abilities/");
    }
    MOM_FOREACH(eHeroAbility, heroAbility, eHeroAbility_MAX)
    {
        if (m_unit->hasHeroAbility(heroAbility))
        {
            QString specialName = prettyQStr(heroAbility);
            displaySpecial(pos, specialName, m_unit->getHeroAbility(heroAbility), ":/abilities/", MoM::QMoMResources::instance().getHelpText(heroAbility).c_str());    // TODO: Lookup images
        }
    }
    MOM_FOREACH(eUnitAbility, unitAbility, eUnitAbility_MAX)
    {
        if (m_unit->hasUnitAbility(unitAbility))
        {
            QString specialName = prettyQStr(unitAbility);
            displaySpecial(pos, specialName, m_unit->getUnitAbility(unitAbility), ":/abilities/", MoM::QMoMResources::instance().getHelpText(unitAbility).c_str());    // TODO: Lookup images
        }
    }
    MOM_FOREACH(eUnitMutation, unitMutation, eUnitMutation_MAX)
    {
        if (m_unit->hasMutation(unitMutation))
        {
            QString specialName = prettyQStr(unitMutation);
            displaySpecial(pos, specialName, 0, ":/abilities/", MoM::QMoMResources::instance().getHelpText(unitMutation).c_str());    // TODO: Lookup images
        }
    }
    MOM_FOREACH(eUnitEnchantment, unitEnchantment, eUnitEnchantment_MAX)
    {
        if (m_unit->hasUnitEnchantment(unitEnchantment))
        {
            QString specialName = prettyQStr(unitEnchantment);
            displaySpecial(pos, specialName, 0, ":/spells/", MoM::QMoMResources::instance().getHelpText(unitEnchantment).c_str());    // TODO: Lookup images
        }
    }
}

void DialogAddUnit::displaySectionTop()
{
    QGraphicsSimpleTextItem* textItem = 0;
    QPointF pos(0, m_pictureHeight);

    pos = QPoint(m_labelWidth, m_pictureHeight - 5 * MoM::QMoMResources::g_FontSmall.pointSize());
    textItem = addText(pos, "Figures");
    textItem->setFont(MoM::QMoMResources::g_FontSmall);
    pos.ry() += MoM::QMoMResources::g_FontSmall.pointSize();
    textItem = addText(pos, "Moves", MoM::QMoMResources::instance().getHelpText(MoM::HELP_MOVES).c_str());
    textItem->setFont(MoM::QMoMResources::g_FontSmall);
    pos.ry() += MoM::QMoMResources::g_FontSmall.pointSize();
    textItem = addText(pos, "Upkeep", MoM::QMoMResources::instance().getHelpText(MoM::HELP_1_UPKEEP).c_str());
    textItem->setFont(MoM::QMoMResources::g_FontSmall);
    pos.ry() += MoM::QMoMResources::g_FontSmall.pointSize();

    const QMoMImagePtr pImage = MoM::QMoMResources::instance().getImage(m_unit->getUnitTypeNr());
    if (0 != pImage)
    {
        QImage image(*pImage);
        if (image.width() < 32)
        {
            image = image.scaled(image.size() * 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
        QPixmap pixmap;
        pixmap.convertFromImage(image);
        QGraphicsItem* item = m_sceneUnit->addPixmap(pixmap);
        QPointF pos((m_labelWidth - pixmap.width()) / 2, (m_pictureHeight - pixmap.height()) / 2);
        item->setPos(pos);
    }

    // Name
    pos = QPointF(m_labelWidth, 4);
    QFont fontName(MoM::QMoMResources::g_Font);
    fontName.setPointSize(20);
    if (m_unit->getHeroName().empty())
    {
        textItem = addText(pos, QString(m_unit->getDisplayName().c_str()));
        textItem->setFont(fontName);
    }
    else
    {
        textItem = addText(pos, QString(m_unit->getHeroName().c_str()));
        textItem->setFont(fontName);
        pos.ry() += fontName.pointSize();
        textItem = addText(pos, QString(m_unit->getDisplayName().c_str()));
        textItem->setFont(fontName);
    }

    // Figures, moves, upkeep
    pos = QPoint(m_labelWidth + m_labelWidth / 2, m_pictureHeight - 5 * MoM::QMoMResources::g_FontSmall.pointSize());
    textItem = addText(pos, QString("%0").arg(m_unit->getMaxFigures()));
    textItem->setFont(MoM::QMoMResources::g_FontSmall);
    pos.ry() += MoM::QMoMResources::g_FontSmall.pointSize();
    textItem = addText(pos, QString("%0").arg(m_unit->getMoves(), 0, 'f', 1));
    textItem->setFont(MoM::QMoMResources::g_FontSmall);
    pos.ry() += MoM::QMoMResources::g_FontSmall.pointSize();
    textItem = addText(pos, QString("%0").arg(m_unit->getUpkeep()));
    textItem->setFont(MoM::QMoMResources::g_FontSmall);

    // To Hit
    MoM::MoMUnit::BaseAttributes actualAttr = m_unit->getActualAttributes();

    pos = QPointF(ui->graphicsView_Unit->width() * 2 / 3, m_pictureHeight - 4 * MoM::QMoMResources::g_FontSmall.pointSize());
    QPixmap pixmap(":images/tohit.gif");
    QGraphicsItem* item = m_sceneUnit->addPixmap(pixmap);
    item->setPos(pos);
    pos.rx() += pixmap.width() + 2;

    if (actualAttr.melee && actualAttr.toHitMelee)
    {
       displayToHit(pos, actualAttr.toHitMelee, "To Hit Melee");
    }
    else
    {
        pos.ry() += 10;
    }
    if (actualAttr.ranged && actualAttr.toHitRanged)
    {
        displayToHit(pos, actualAttr.toHitRanged, "To Hit Ranged");
    }
    else
    {
        pos.ry() += 10;
    }
    if (actualAttr.defense && actualAttr.toDefend)
    {
        displayToHit(pos, actualAttr.toDefend, "To Defend");
    }
}

void DialogAddUnit::displaySpecial(QPointF& pos, const QString& specialName, int specialValue, const QPixmap& pixmap, const QString& helpText)
{
    QGraphicsItem* item = 0;

    item = m_sceneUnit->addPixmap(pixmap);
    item->setToolTip(helpText);
    item->setPos(pos);

    QString text = specialName;
    if (specialValue != 0)
    {
        text += " " + QString("%0").arg(specialValue);
    }
    item = addText(pos + QPointF(pixmap.width() * 4 / 3, 0), text);
    item->setToolTip(helpText);

    pos.rx() += ui->graphicsView_Unit->width() / 2;
    if (pos.rx() > ui->graphicsView_Unit->width() * 3 / 4)
    {
        pos.setX(0);
        pos.ry() += MoM::Max(pixmap.height() + 2, 34);
    }
}

void DialogAddUnit::displaySpecial(QPointF& pos, const QString& specialName, int specialValue, const QMoMAnimation& animation, const QString& helpText)
{
    QMoMAnimation scaledAnimation = animation;
   // scaledAnimation.scale(2.0);
    QMoMAnimationTile* animationItem = new QMoMAnimationTile(scaledAnimation);

    m_sceneUnit->addItem(animationItem);
    animationItem->setToolTip(helpText);
    animationItem->setPos(pos);

    QString text = specialName;
    if (specialValue != 0)
    {
        text += " " + QString("%0").arg(specialValue);
    }
    QGraphicsSimpleTextItem* itemText = addText(pos + QPointF(animationItem->boundingRect().width() * 4 / 3, 0), text);
    itemText->setToolTip(helpText);

    pos.rx() += ui->graphicsView_Unit->width() / 2;
    if (pos.rx() > ui->graphicsView_Unit->width() * 3 / 4)
    {
        pos.setX(0);
        pos.ry() += MoM::Max(itemText->boundingRect().height() + 2.0, 34.0);
    }
}

void DialogAddUnit::displayStrength(QPointF& pos, int strength, int bonusStrength, const QString& imageBaseName, const QString& helpText)
{
  // calculate number of normal, lost_normal, gold, and lost_gold icons
  int normal = strength - bonusStrength;
  int gold = bonusStrength;
//      int lost_normal = 0, lost_gold = 0;
//      if (m_unit.bonuses[strength] >= m_unit.penalties[strength])
//      {
//         // Bonus at least as large as the penalty - only lost gold
//         normal = unit.fixedunit[strength];
//         lost_normal = 0;
//         gold = unit.bonuses[strength] - unit.penalties[strength];
//         lost_gold = unit.penalties[strength];
//      }
//      else if (unit.fixedunit[strength] + unit.bonuses[strength] >= unit.penalties[strength])
//      {
//         // More penalty than bonus, but the total is not negative - lost gold and lost normal
//         normal = unit[strength];
//         lost_normal = unit.penalties[strength] - unit.bonuses[strength];
//         gold = 0;
//         lost_gold = unit.bonuses[strength];
//      }
//      else
//      {
//         // More penalty than strength - truncate to zero with lost normal and lost gold
//         normal = 0;
//         lost_normal = unit.fixedunit[strength];
//         gold = 0;
//         lost_gold = unit.bonuses[strength];
//      }

  int left = pos.x();
  int x = left;
  int y = pos.y();
  int i, col = 0;
  QPixmap pixmap(":/images/" + imageBaseName + "_normal.gif");
  for (i = 0; i < normal; ++i, ++col)
  {
     if (col > 0 && col % 15 == 0)
     {
         left += pixmap.width() / 3;
         x = left;
         y += pixmap.width() / 3;
     }
     if (col > 0 && col % 5 == 0)
     {
         x += pixmap.width() / 5;
     }
     QGraphicsItem* item = m_sceneUnit->addPixmap(pixmap);
     item->setToolTip(helpText);
     item->setPos(x, y);
     x += pixmap.width() + 1;
  }
//      for (i = 0; i < lost_normal; ++i, ++col)
//      {
//         if (col > 0 && col % 15 == 0) doc.writeln("<br />");
//         doc.write("<img ");
//         if (col > 0 && col % 5 == 0) doc.write(style_spacer);
//         doc.writeln("src=\"images/" + image + "_normal_lost.gif\" alt=\"n\">");
//      }
    pixmap = QPixmap(":/images/" + imageBaseName + "_gold.gif");
      for (i = 0; i < gold; ++i, ++col)
      {
         if (col > 0 && col % 15 == 0)
         {
             left += pixmap.width() / 3;
             x = left;
             y += pixmap.width() / 3;
         }
         if (col > 0 && col % 5 == 0)
         {
             x += pixmap.width() / 5;
         }
         QGraphicsItem* item = m_sceneUnit->addPixmap(pixmap);
         item->setToolTip(helpText);
         item->setPos(x, y);
         x += pixmap.width() + 1;
      }
//      for (i = 0; i < lost_gold; ++i, ++col)
//      {
//         if (col > 0 && col % 15 == 0) doc.writeln("<br />");
//         doc.write("<img ");
//         if (col > 0 && col % 5 == 0) doc.write(style_spacer);
//         doc.writeln("src=\"images/" + image + "_gold_lost.gif\" alt=\"g\">");
//      }

  pos.ry() += m_lineHeight;
}

void DialogAddUnit::displayToHit(QPointF& pos, int toHit, const QString& labelText)
{
    QString textModifier = QString("%0").arg(toHit);
    if (toHit >= 0)
    {
        textModifier = QString("+%0").arg(toHit);
    }

  if (!labelText.isEmpty())
  {
      QGraphicsSimpleTextItem* textItem = addText(pos, textModifier + " " + labelText);
      textItem->setFont(MoM::QMoMResources::g_FontSmall);
  }

  pos.ry() += 10;
}

void DialogAddUnit::on_comboBox_Unit_currentIndexChanged(int index)
{
    // Do nothing if we are busy with an external update
    if (m_updating)
        return;

    m_unit->changeUnit(static_cast<MoM::eUnit_Type>(index - 1));
	update();
}

void DialogAddUnit::on_pushButton_Summon_clicked()
{
    MoM::eUnit_Type unitType = static_cast<MoM::eUnit_Type>(ui->comboBox_Unit->currentIndex() - 1);

    MoMController momController(m_game.data());
    bool ok = momController.addUnit(MoM::PLAYER_YOU, unitType);
    if (!ok)
    {
        (void)QMessageBox::warning(this,
            tr("Summon"),
            tr("Failed to summon %0: %1").arg(prettyQStr(unitType)).arg(momController.errorString().c_str()));
    }
    else
    {
        (void)QMessageBox::information(this,
            tr("Summon"),
            tr("Summoned %0 to your summoning circle").arg(prettyQStr(unitType)));
    }
}

void DialogAddUnit::slot_gameChanged(const QMoMGamePtr& game)
{
    MoM::UpdateLock lock(m_updating);

    m_game = game;
	m_unit->setGame(m_game.data());

    // Reinitialize combo box with units

    // Save current unitType index
	int index = ui->comboBox_Unit->currentIndex();
    ui->comboBox_Unit->clear();

    ui->comboBox_Unit->addItem("");
    MOM_FOREACH(eUnit_Type, unitTypeNr, eUnit_Type_MAX)
    {
        QString title = prettyQStr(unitTypeNr);

        MoM::Unit_Type_Data* data = 0;
        if ((0 != game) && (0 != (data = game->getUnitTypeData(unitTypeNr))))
        {
            title = QString("%0").arg((int)unitTypeNr, 3) + "   " + QString(game->getRaceName(data->m_Race_Code).c_str()) + "   " + QString(game->getNameByOffset(data->m_PtrName));
        }
        QMoMIconPtr icon = MoM::QMoMResources::instance().getIcon(unitTypeNr);

        ui->comboBox_Unit->addItem(*icon, title);
	}

    // Restore current unitType index
    m_unit->changeUnit(static_cast<MoM::eUnit_Type>(index - 1));
    ui->comboBox_Unit->setCurrentIndex(index);

    update();
}

void DialogAddUnit::slot_gameUpdated()
{
    MoM::UpdateLock lock(m_updating);

    update();
}

void DialogAddUnit::slot_unitChanged(const QMoMUnitPtr& unit)
{
    MoM::UpdateLock lock(m_updating);

    m_unit = unit;
    MoM::eUnit_Type unitTypeNr = m_unit->getUnitTypeNr();
    ui->comboBox_Unit->setCurrentIndex(1 + toInt(unitTypeNr));

    update();
}

void DialogAddUnit::update()
{
    // Remove old QGraphicsItems
    m_sceneUnit->clear();

    if (0 == m_game)
        return;

    // Add new QGraphicsItems

    // Display top section
    displaySectionTop();

    QPointF pos = QPointF(0, m_pictureHeight);

    // Basic attributes
    displaySectionBasicAttributes(pos);
    pos.ry() += m_lineHeight;

    // Level/XP
    if (0 != m_unit->getLevel())
    {
        displayLevel(pos, m_unit->getLevelName(), m_unit->getLevel(), m_unit->getXP());
    }

    // Items
    for (int slotNr = 0; MoM::toUInt(slotNr) < MoM::gMAX_ITEMSLOTS; ++slotNr)
    {
        displayItem(pos, m_unit->getSlotType(slotNr), m_unit->getSlotItem(slotNr));
    }

    // Abilities, item effects, and spell effects
    displaySectionSpecials(pos);
}

}
