// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2012-03-12
// ---------------------------------------------------------------------------

#include "DialogTools.h"
#include "ui_DialogTools.h"

#include "MainWindow.h"
#include "MoMCatnip.h"
#include "MoMController.h"
#include "MoMGameCustom.h"
#include "MoMGenerated.h"
#include "MoMLbxBase.h"
#include "MoMGameMemory.h"
#include "MoMProcess.h"
#include "MoMGameSave.h"
#include "MoMUtility.h"
#include "QMoMAnimation.h"
#include "QMoMLbx.h"
#include "QMoMResources.h"

#include "DialogAddArtifact.h"
#include "DialogBuildingTree.h"
#include "DialogExternalAI.h"
#include "DialogManageCities.h"
#include "DialogSelectInitialSpells.h"
#include "DialogSelectRaces.h"

DialogTools::DialogTools(QWidget *parent) :
    QMoMDialogBase(parent),
    ui(new Ui::DialogTools)
{
    ui->setupUi(this);
    postInitialize();
}

DialogTools::~DialogTools()
{
    preFinalize();
    delete ui;
}

void DialogTools::slot_gameChanged(const QMoMGamePtr &game)
{
    m_game = game;
}

void DialogTools::slot_gameUpdated()
{
}

void DialogTools::on_pushButton_ApplyBuildQueues_clicked()
{
//    DialogBuildingQueues* dialog = new DialogBuildingQueues(MainWindow::getInstance());
//    dialog->show();

    DialogManageCities* window = new DialogManageCities(MainWindow::getInstance());
    window->show();
}

void DialogTools::on_pushButton_CatnipMod_clicked()
{
    if (m_game.isNull())
    {
        (void)QMessageBox::warning(this,
            tr("Catnip mod"),
            tr("There is no game to operate on"));
        return;
    }

//    MOM_FOREACH(eUnit_Type, unitTypeNr, eUnit_Type_MAX)
//    {
//        int unitNr = 999;
//        MoM::Unit* unit = game->getUnit(unitNr);
//        if (0 == unit)
//            return;
//        memset(unit, '\0', sizeof(MoM::Unit));
//        unit->m_Unit_Type = unitTypeNr;
//        unit->m_Hero_Slot_Number = -1;

//        MoM::Unit_Type_Data* unitType = game->getUnitTypeData(unit->m_Unit_Type);
//        if (0 == unitType)
//            return;

//        MoM::Battle_Unit battleUnit = { 0 };
//        memcpy(&battleUnit.m_Melee, &unitType->m_Melee, sizeof(MoM::Unit_Type_Data) - offsetof(MoM::Unit_Type_Data, m_Melee));
//        battleUnit.m_unitNr = unitNr;
//        battleUnit.m_xPos = 0;
//        battleUnit.m_yPos = 0;

//        QMoMUnitPtr momUnit = QMoMUnitPtr(new MoM::MoMUnit(game.data()));
//        qDebug() << "momUnit empty" << momUnit->getDisplayName().c_str();
//        momUnit->changeUnit(&battleUnit);
//        qDebug() << "momUnit" << momUnit->getDisplayName().c_str();

//        MoM::QMoMUnitTile unitTile(true);
//        unitTile.setGame(game);
//        unitTile.setUnit(momUnit);
//        QRectF rect = unitTile.boundingRect();
//        qDebug() << "unitTile rect" << rect;
//        const int arrFrameNr[10] = {1,0,1,2,1,0,1,2,3,1};
//        QMoMAnimation animation;
//        for (int heading = 2; heading < 10; ++heading)
//        for (int frame = 0; frame < 10; ++frame)
//        {
//            unitTile.setFrameNr(arrFrameNr[frame]);
//            int step = MoM::Min(frame, 8);

//            double angle = (heading - 3) * 3.14159 / 4;
//            int dx = MoM::Round(cos(angle));
//            int dy = MoM::Round(sin(angle));
//            battleUnit.m_xPosHeaded = dx;
//            battleUnit.m_yPosHeaded = dy;
//            qDebug("heading %d %d,%d", heading, battleUnit.m_xPosHeaded, battleUnit.m_yPosHeaded);

//            QMoMImagePtr image(new QImage(rect.width() * 4, rect.height() * 4, QImage::Format_RGB32));
//            qDebug() << "numColors" << image->numColors();

//            MoM::QMoMPalette colorTable = MoM::QMoMResources::instance().getColorTable();
//            colorTable.resize(244);
//            colorTable[0] = qRgb(255, 0, 255);                  // Treat MAGENTA RGB(255, 0, 255) as TRANSPARENT (0)!
//            colorTable[MoM::gSHADOW_COLOR] = qRgb(0, 255, 0);   // Treat GREEN RGB(0, 255, 0) as SHADOW (232)

//            QRgb rgbTransparent = colorTable[0];
//            qDebug() << "rgbTransparent" << rgbTransparent;
//            image->fill(rgbTransparent);
//            QPainter painter(image.data());
//            qDebug() << "translate";
//            painter.translate(rect.width() * 2, rect.height() * 3);
//            qDebug() << "paint terrain";
//            const QMoMImagePtr imageTerrain = MoM::QMoMResources::instance().getImage(MoM::TERRAINBATTLE_firstbasic);
//            if (0 != imageTerrain)
//            {
//                QRectF rectTerrain(-30/2, -16, 30, 16);
//                rectTerrain.translate(-(dx - dy) * (step) * 16 / 8, -(dx + dy) * (step) * 8 / 8 );
//                painter.drawImage(rectTerrain, *imageTerrain);
//                if (step != 0)
//                {
//                    rectTerrain.translate((dx - dy) * 16, (dx + dy) * 8 );
//                    painter.drawImage(rectTerrain, *imageTerrain);
//                }
//            }


//            qDebug() << "paint unit";
//            unitTile.paint(&painter, NULL, NULL);
//            painter.end();

//            image = QMoMImagePtr(new QImage(image->convertToFormat(QImage::Format_Indexed8, colorTable, Qt::AutoColor)));
//            image->setColorTable(MoM::QMoMResources::instance().getColorTable());
//            animation.append(image);

//            qDebug() << "setPixmap";
//            ui->label->setPixmap(QPixmap::fromImage(*image));
//        }

//        QString title = "Tactical_" + QString(momUnit->getDisplayName().c_str()).replace(QRegExp("[^A-Za-z0-9]"), "");
//        QString testFilenameWrite = "C:\\GAMES\\Klaas_Master_of_Magic\\LBX\\gif\\_crop\\Units\\" + title + ".gif";
//        qDebug() << "Opening file " << testFilenameWrite << " to write to";
//        QFile testFileWrite(testFilenameWrite);
//        qDebug() << "exists() -> " << testFileWrite.exists();
//        bool result = testFileWrite.open(QFile::WriteOnly | QFile::Truncate);
//        qDebug() << "open(WriteOnly | Truncate) -> " << result;

//        QMoMGifHandler gifHandlerWrite;
//        gifHandlerWrite.setDevice(&testFileWrite);
//        if (!animation.empty())
//        {
//            animation.crop();
//            animation.scale(2.0);
//            gifHandlerWrite.setAnimationOption(QMoMGifHandler::Disposal, QMoMGifHandler::DisposalBackground);
//            gifHandlerWrite.setAnimationOption(QMoMGifHandler::Delay, 20);
//            result = gifHandlerWrite.writeAnimation(animation);
//        }
//        qDebug() << "gifHandler.writeAnimation(animation) -> " << result;
//    }

//    return;


    MoM::MoMCatnip catnip;

    bool ok = catnip.apply(m_game.data());
    if (!ok)
    {
        statusBar()->showMessage(tr("Failed to apply Catnip mod"));
        (void)QMessageBox::warning(this,
            tr("Catnip mod"),
            catnip.errorString().c_str());
    }
    else
    {
        statusBar()->showMessage(tr("Catnip mod applied"));
        (void)QMessageBox::warning(this,
            tr("Catnip mod"),
            tr( "Catnip mod applied\n"
                "\n"
                "1. All units have an additional half move\n"
                "2. The heroes have alternative slots (and a couple of changes)\n"
                "3. Magicians, priests, and shamen have been renamed and have different abilities\n"
            ));
    }
}

void DialogTools::on_pushButton_RepopLairs_clicked()
{
    if (m_game.isNull())
    {
        (void)QMessageBox::warning(this, 
            tr("Repop Lairs"),
            tr("There is no game to operate on"));
        return;
    }

    bool ok = m_game->readData();

    if (ok)
    {
        MoM::MoMController momController(m_game.data());
        ok = momController.repopLairs(ui->checkBox_RepopMaxOut->isChecked());
        if (!ok)
        {
            (void)QMessageBox::warning(this, 
                tr("Repop Lairs"),
				tr("Failed to repop the lairs: %0").arg(momController.errorString().c_str()));
        }
    }

    if (!ok)
    {
        statusBar()->showMessage(tr("Failed to repop Lairs"));
    }
    else
    {
        statusBar()->showMessage(tr("Lairs have been repopulated"));
    }

    update();
}

void DialogTools::on_pushButton_LucernMod_clicked()
{
    if (m_game.isNull())
    {
        (void)QMessageBox::warning(this,
            tr("Lucern mod"),
            tr("There is no game to operate on"));
        return;
    }

    // Load FONTS.LBX (TODO: needs centralization)
    MoM::QMoMPalette colorTable(256);
    std::string fontsLbxFile = m_game->getGameDirectory() + "/" + "FONTS.LBX";
    MoM::MoMLbxBase fontsLbx;
    bool ok = fontsLbx.load(fontsLbxFile);
    if (ok)
    {
        const uint8_t* dataPalette = fontsLbx.getRecord(2);
        MoM::convertLbxToPalette(dataPalette, colorTable);
    }

    // Adjust color table for proper color matching
    colorTable.resize(244);
    colorTable[0] = qRgb(255, 0, 255);  // Treat MAGENTA RGB(255, 0, 255) as TRANSPARENT!
    colorTable[232] = qRgb(0, 255, 0);    // Treat GREEN RGB(0, 255, 0) as SHADOW (232 or 239??)

    // Load Centaur Scouts
    QMoMImagePtr image(new QImage);
    if (ok)
    {
        ok = image->load("UNITS2_000_000.png");
    }

    if (ok)
    {
        QImage saveImage = image->convertToFormat(QImage::Format_Indexed8, colorTable, Qt::AutoColor);

        // TODO: Remove verification
        saveImage.save("UNIT_Centaur_Scouts.png");
        std::cout << "pixel(1,5) = " << (int)saveImage.pixelIndex(1,5) << " pixel(1,6) = " << (int)saveImage.pixelIndex(1,6) << std::endl;
//        ui->label_Image->setPixmap(QPixmap::fromImage(saveImage));
    }

    // Load UNITS2.LBX
    std::string units2LbxFile = m_game->getGameDirectory() + "/" + "UNITS2.LBX";
    MoM::MoMLbxBase units2Lbx;
    if (ok)
    {
        ok = units2Lbx.load(units2LbxFile);
    }

    // Load FIGURES9.LBX
    std::string figuresLbxFile = m_game->getGameDirectory() + "/" + "FIGURES9.LBX";
    MoM::MoMLbxBase figuresLbx;
    if (ok)
    {
        ok = figuresLbx.load(figuresLbxFile);
    }

    // Replace UNITS2/0 by Lucern/FIGURES4_077_001
    const QMoMAnimation images(1, image);
    std::vector<uint8_t> dataBuffer;
    if (ok)
    {
        ok = MoM::convertImagesToLbx(images, dataBuffer, "LucernMod");
    }

    if (ok)
    {
        ok = units2Lbx.replaceRecord(0, dataBuffer);
    }

    // Replace FIGURES9/0-8 by Lucern/FIGURES4_072-079 (4 images each)
    for (int direction = 0; direction < 8; ++direction)
    {
        QMoMAnimation images(4);
        for (int movement = 0; movement < 4; ++movement)
        {
            QImage image;
            QString filename = QString("Lucern/Centaur Scouts/FIGURES4_%0_%1.png").arg(72 + direction, 3, 10, QChar('0')).arg(movement, 3, 10, QChar('0'));
            if (ok)
            {
                ok = image.load(filename);
            }
            if (ok)
            {
                images[movement] = QMoMImagePtr(new QImage(image.convertToFormat(QImage::Format_Indexed8, colorTable, Qt::AutoColor)));
            }
        }
        std::vector<uint8_t> dataBuffer;
        if (ok)
        {
            ok = MoM::convertImagesToLbx(images, dataBuffer, "LucernMod");
        }
        if (ok)
        {
            ok = figuresLbx.replaceRecord(0 + direction, dataBuffer);
        }
    }

    // Save UNITS2.LBX
    if (ok)
    {
        ok = units2Lbx.save(units2Lbx.getFilename());
    }

    // Save FIGURES9.LBX
    if (ok)
    {
        ok = figuresLbx.save(figuresLbx.getFilename());
    }

    if (!ok)
    {
        statusBar()->showMessage(tr("Failed to apply Lucern mod"));
    }
    else
    {
        statusBar()->showMessage(tr("Lucern mod applied"));
    }
}

void DialogTools::on_pushButton_SelectInitialSpells_clicked()
{
    DialogSelectInitialSpells* dialog = new DialogSelectInitialSpells(MainWindow::getInstance());
    dialog->show();
}

QStatusBar* DialogTools::statusBar()
{
    MainWindow* mainWindow = MainWindow::getInstance();
	return mainWindow->statusBar();
}

void DialogTools::on_pushButton_ExternalAI_clicked()
{
    DialogExternalAI* dialog = new DialogExternalAI(MainWindow::getInstance());
    dialog->show();
}

void DialogTools::on_pushButton_ConjureArtifact_clicked()
{
    DialogAddArtifact* dialog = new DialogAddArtifact(MainWindow::getInstance());
    dialog->show();
}

void DialogTools::on_pushButton_SelectRaces_clicked()
{
    DialogSelectRaces* dialog = new DialogSelectRaces(MainWindow::getInstance());
    dialog->show();
}

void DialogTools::on_pushButton_BuildingTree_clicked()
{
    auto dialog = new MoM::DialogBuildingTree(MainWindow::getInstance());
    dialog->show();
}
