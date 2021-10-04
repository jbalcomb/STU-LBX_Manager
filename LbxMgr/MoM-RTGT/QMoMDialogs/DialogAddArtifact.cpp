#include "DialogAddArtifact.h"
#include "ui_DialogAddArtifact.h"

#include <QGraphicsPixmapItem>
#include <QMessageBox>

#include "MoMController.h"
#include "MoMGenerated.h"
#include "QMoMResources.h"
#include "QMoMTableItem.h"
#include "QMoMUtility.h"

using namespace MoM;

DialogAddArtifact::DialogAddArtifact(QWidget *parent) :
    QMoMDialogBase(parent),
    m_scene(new QGraphicsScene),
    ui(new Ui::DialogAddArtifact)
{
    m_font.setPointSize(12);
    ui->setupUi(this);
    ui->graphicsView->setScene(m_scene);
    postInitialize();
}

DialogAddArtifact::~DialogAddArtifact()
{
    preFinalize();
    delete ui;
    delete m_scene;
}

void DialogAddArtifact::slot_gameChanged(const QMoMGamePtr &game)
{
    m_game = game;
    ui->comboBox_Artifacts->clear();
    if (0 == m_game)
        return;
    for (int artifactNr = 0; toUInt(artifactNr) < gMAX_ARTIFACTS_IN_GAME; ++artifactNr)
    {
        ui->comboBox_Artifacts->addItem("");
    }
    slot_gameUpdated();
}

void DialogAddArtifact::slot_gameUpdated()
{
    for (int artifactNr = 0; toUInt(artifactNr) < gMAX_ARTIFACTS_IN_GAME; ++artifactNr)
    {
        ItemDataLbx* artifact = m_game->getItemDataLbx(artifactNr);
        if (0 == artifact)
            break;
        const uint8_t* artifactsInGame = m_game->getArtifacts_in_game();
        bool inGame = false;
        if (0 != artifactsInGame)
        {
            inGame = artifactsInGame[artifactNr];
        }
        QString text = QString("%0 - %1 - %2").arg(artifactNr, 3).arg(inGame).arg(artifact->m_Item.m_Item_Name);
        ui->comboBox_Artifacts->setItemIcon(artifactNr, *QMoMResources::instance().getIcon(artifact->m_Item.m_Icon));
        ui->comboBox_Artifacts->setItemText(artifactNr, text);
    }
}

void DialogAddArtifact::on_pushButton_AddArtifact_clicked()
{
    MoMController momController(m_game.data());
    int artifactNr = ui->comboBox_Artifacts->currentIndex();
    bool ok = momController.addArtifact(PLAYER_YOU, artifactNr);
    if (!ok)
    {
        (void)QMessageBox::warning(this,
            tr("Conjure artifact"),
            tr("Failed to conjure artifact: %1").arg(momController.errorString().c_str()));
    }
    else
    {
        (void)QMessageBox::information(this,
            tr("Summon"),
            tr("Conjured artifact to your fortress"));
    }
}

void DialogAddArtifact::on_comboBox_Artifacts_currentIndexChanged(int index)
{
    m_scene->clear();

    {
        // Set background
        QPixmap pixmapItem = MoM::QMoMResources::instance().getPixmap(LBXRecordID("ITEMISC", 25), 2);
        QGraphicsPixmapItem* graphicsItem = m_scene->addPixmap(pixmapItem);
        graphicsItem->setPos(0, 0);
    }

    int artifactNr = index;
    ItemDataLbx* artifact = m_game->getItemDataLbx(artifactNr);
    Item* itemData = &artifact->m_Item;
    if (0 == artifact)
        return;

    int xLeft = 16;
    int y = 16;

    // Show name
    QPixmap pixmapItem = MoM::QMoMResources::instance().getPixmap(itemData->m_Icon, 2);
    QGraphicsPixmapItem* graphicsItem = m_scene->addPixmap(pixmapItem);
    graphicsItem->setPos(xLeft, y);

    int dxText = graphicsItem->boundingRect().width() * 5 / 4;
    int xText = xLeft + dxText;

    QGraphicsSimpleTextItem* textItem = addText(QPointF(), QString("%0").arg(itemData->m_Item_Name), "");
    y += (graphicsItem->boundingRect().height() - textItem->boundingRect().height()) / 2;
    textItem->setPos(xText, y);

    int dy = textItem->boundingRect().height() + 2;
    y += dy * 7 / 4 ;

    // Show bonuses
    if (0 != itemData->m_Bonuses.Attack)
    {
        addText(QPointF(xText, y), QString("+%0 Attack").arg(itemData->m_Bonuses.Attack), "");
        y += dy;
    }
    if (0 != itemData->m_Bonuses.To_Hit)
    {
        addText(QPointF(xText, y), QString("+%0 To Hit").arg(itemData->m_Bonuses.To_Hit), "");
        y += dy;
    }
    if (0 != itemData->m_Bonuses.Defense)
    {
        addText(QPointF(xText, y), QString("+%0 Defense").arg(itemData->m_Bonuses.Defense), "");
        y += dy;
    }
    if (0 != itemData->m_Bonuses.Movement_in_halves)
    {
//        QString text = QString("%0 Move").arg(QMoMTableItemBase::formatNumber(itemData->m_Bonuses.Movement_in_halves, SHOWNUMBER_halfMove));
        addText(QPointF(xText, y), QString("+%0 Movement").arg(itemData->m_Bonuses.Movement_in_halves), "");
        y += dy;
    }
    if (0 != itemData->m_Bonuses.Resistance)
    {
        addText(QPointF(xText, y), QString("+%0 Resistance").arg(itemData->m_Bonuses.Resistance), "");
        y += dy;
    }
    if (0 != itemData->m_Bonuses.Spell_Points)
    {
        addText(QPointF(xText, y), QString("+%0 Spell Skill").arg(itemData->m_Bonuses.Spell_Points), "");
        y += dy;
    }
    if (0 != itemData->m_Bonuses.Spell_Save)
    {
        addText(QPointF(xText, y), QString("-%0 Spell Save").arg(itemData->m_Bonuses.Spell_Save), "");
        y += dy;
    }

    // Show spells
    if (0 != itemData->m_Spell_Number_Charged)
    {
        QPixmap pixmapItem = MoM::QMoMResources::instance().getPixmap(itemData->m_Spell_Number_Charged, 1.5);
        QGraphicsPixmapItem* graphicsItem = m_scene->addPixmap(pixmapItem);
        graphicsItem->setToolTip(QMoMResources::instance().getHelpText(itemData->m_Spell_Number_Charged).c_str());
        graphicsItem->setPos(xLeft + (dxText - graphicsItem->boundingRect().width()) / 2, y + (dy - graphicsItem->boundingRect().height()) / 2);

        addText(QPointF(xText, y), QString("%0 x %1").arg(prettyQStr(itemData->m_Spell_Number_Charged)).arg(itemData->m_Number_Of_Charges), "");

        y += dy;
    }

    // Show enchantments
    MOM_FOREACH(eItemPower, itemPower, eItemPower_MAX)
    {
        uint32_t bitmask = (1 << (unsigned)itemPower);
        if (0 != (itemData->m_Bitmask_Powers.bits & bitmask))
        {
            QPixmap pixmapItem = MoM::QMoMResources::instance().getPixmap(itemPower, 1.5);
            QGraphicsPixmapItem* graphicsItem = m_scene->addPixmap(pixmapItem);
            graphicsItem->setToolTip(QMoMResources::instance().getHelpText(itemPower).c_str());
            graphicsItem->setPos(xLeft + (dxText - graphicsItem->boundingRect().width()) / 2, y + (dy - graphicsItem->boundingRect().height()) / 2);

            addText(QPointF(xText, y), QString("%0").arg(prettyQStr(itemPower)), QMoMResources::instance().getHelpText(itemPower).c_str());
            y += dy;
        }
    }
}

QGraphicsSimpleTextItem* DialogAddArtifact::addText(const QPointF& pos, const QString& text, const QString& helpText)
{
    QGraphicsSimpleTextItem* textItem = m_scene->addSimpleText(text, m_font);
    textItem->setToolTip(helpText);
    textItem->setBrush(QBrush(Qt::yellow));
    textItem->setPos(pos);
    return textItem;
}

