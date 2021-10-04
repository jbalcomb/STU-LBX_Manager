// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include "DialogCalculatorAddress.h"
#include "ui_DialogCalculatorAddress.h"

#include <stdio.h>  // sscanf

#include "MainWindow.h"
#include "MoMExeMagic.h"
#include "MoMExeWizards.h"
#include "MoMProcess.h"
#include "MoMUtility.h"

DialogCalculatorAddress::DialogCalculatorAddress(QWidget *parent) :
    QMoMDialogBase(parent),
    ui(new Ui::DialogCalculatorAddress),
    m_updating(false)
{
    ui->setupUi(this);
    postInitialize();
}

DialogCalculatorAddress::~DialogCalculatorAddress()
{
    preFinalize();
    delete ui;
}

void DialogCalculatorAddress::update(const QWidget* originator)
{
    if (m_updating)
        return;
    MoM::UpdateLock lock(m_updating);

    // Process input:
    // - deduce file if applicable
    // - deduce offsetExe if applicable (for file)
    // - deduce offsetMem if applicable (for memory)

    size_t exeOffset = 0;
    size_t memOffset = 0;

    if (originator == ui->lineEdit_OffsetExe)
    {
        bool ok = false;
        exeOffset = ui->lineEdit_OffsetExe->text().toULong(&ok, 16);
        convertExetoMemOffset(exeOffset, memOffset);
    }
    if (originator == ui->lineEdit_OffsetIDA)
    {
        convertIDAtoExeOffset(ui->lineEdit_OffsetIDA->text(), exeOffset);
        QString sExeOffset;
        convertExetoExeString(exeOffset, sExeOffset);
        convertExetoMemOffset(exeOffset, memOffset);
    }

    if (originator == ui->lineEdit_OffsetMem)
    {
        bool ok = false;
        memOffset = ui->lineEdit_OffsetMem->text().toULong(&ok, 16);
        convertMemtoExeOffset(memOffset, exeOffset);
    }
    if (originator == ui->lineEdit_OffsetDOS)
    {
        convertDOStoMemOffset(ui->lineEdit_OffsetDOS->text(), memOffset);
        convertMemtoExeOffset(memOffset, exeOffset);
    }

    // TODO: Consider the momPointer

    if (originator != ui->lineEdit_OffsetExe)
    {
        QString sExeOffset;
        if (0 != exeOffset)
        {
            sExeOffset = QString("%0").arg(exeOffset, 5, 16, QChar('0'));
        }
        ui->lineEdit_OffsetExe->setText(sExeOffset);
    }
    if (originator != ui->lineEdit_OffsetIDA)
    {
        QString sIDAOffset;
        convertExetoIDAString(exeOffset, sIDAOffset);
        ui->lineEdit_OffsetIDA->setText(sIDAOffset);
    }
    if (originator != ui->lineEdit_CurrentFileValue)
    {
        ui->lineEdit_CurrentFileValue->clear();
        MoM::MoMExeBase* exeBase = getExeBase();
        if ((0 != exeBase) && (exeOffset + 1 < exeBase->getExeSize()))
        {
            const uint8_t* pointer = exeBase->getExeContents() + exeOffset;
            updateCurrentValue(ui->lineEdit_CurrentFileValue, pointer);
        }
    }

    const MoM::MoMProcess* momProcess = 0;
    if (0 != m_game)
    {
        momProcess = m_game->getMoMProcess();
    }
    if (0 != momProcess)
    {
        ui->lineEdit_ArtmoneyData->setText(QString("Base=%0 Size=%1 Seg0=%2 DS=%3")
            .arg((size_t)momProcess->getBaseAddress(), 8, 16, QChar('0'))
            .arg((size_t)momProcess->getBaseAddressSize(), 8, 16, QChar('0'))
            .arg((size_t)momProcess->getOffsetSegment0(), 4, 16, QChar('0'))
            .arg((size_t)momProcess->getOffsetDatasegment(), 4, 16, QChar('0')));
    }
    if (originator != ui->lineEdit_OffsetMem)
    {
        QString sMemOffset;
        convertMemtoMemString(memOffset, sMemOffset);
        ui->lineEdit_OffsetMem->setText(sMemOffset);
    }
    if (originator != ui->lineEdit_OffsetDOS)
    {
        QString sDOSOffset;
        convertMemtoDOSString(memOffset, sDOSOffset);
        ui->lineEdit_OffsetDOS->setText(sDOSOffset);
    }
    if (originator != ui->lineEdit_CurrentMemValue)
    {
        ui->lineEdit_CurrentMemValue->clear();
        if ((0 != momProcess) && (memOffset >= (size_t)momProcess->getBaseAddress() + momProcess->getOffsetDatasegment())
                 && (memOffset +1 < (size_t)momProcess->getBaseAddress() + momProcess->getBaseAddressSize()))
        {
            const uint8_t* pointer = const_cast<MoM::MoMProcess*>(momProcess)->getSeg0Pointer() + (memOffset - (size_t)momProcess->getBaseAddress() - momProcess->getOffsetSegment0());
            updateCurrentValue(ui->lineEdit_CurrentMemValue, pointer);
        }
    }
}

void DialogCalculatorAddress::updateValues(const QWidget *originator)
{
    if (m_updating)
        return;
    MoM::UpdateLock lock(m_updating);

    if (originator != ui->lineEdit_CurrentFileValue)
    {
        bool ok = false;
        size_t exeOffset = ui->lineEdit_OffsetExe->text().toULong(&ok, 16);

        ui->lineEdit_CurrentFileValue->clear();
        MoM::MoMExeBase* exeBase = getExeBase();
        if ((0 != exeBase) && (exeOffset + 1 < exeBase->getExeSize()))
        {
            const uint8_t* pointer = exeBase->getExeContents() + exeOffset;
            updateCurrentValue(ui->lineEdit_CurrentFileValue, pointer);
        }
    }

    const MoM::MoMProcess* momProcess = 0;
    if (0 != m_game)
    {
        momProcess = m_game->getMoMProcess();
    }
    if (originator != ui->lineEdit_CurrentMemValue)
    {
        bool ok = false;
        size_t memOffset = ui->lineEdit_OffsetMem->text().toULong(&ok, 16);

        ui->lineEdit_CurrentMemValue->clear();
        if ((0 != momProcess) && (memOffset >= (size_t)momProcess->getBaseAddress() + momProcess->getOffsetDatasegment())
                 && (memOffset +1 < (size_t)momProcess->getBaseAddress() + momProcess->getBaseAddressSize()))
        {
            const uint8_t* pointer = const_cast<MoM::MoMProcess*>(momProcess)->getSeg0Pointer() + (memOffset - (size_t)momProcess->getBaseAddress() - momProcess->getOffsetSegment0());
            updateCurrentValue(ui->lineEdit_CurrentMemValue, pointer);
        }
    }
}

void DialogCalculatorAddress::on_lineEdit_CurrentFileValue_returnPressed()
{
    int intValue = 0;
    const char* buf = ui->lineEdit_CurrentFileValue->text().toUtf8().data();
    bool ok = (1 == sscanf(buf, "%d", &intValue));
    if (!ok)
        return;
    uint8_t u8Value = static_cast<uint8_t>(intValue);

    size_t exeOffset = 0;
    exeOffset = ui->lineEdit_OffsetExe->text().toULong(&ok, 16);
    if (!ok)
        return;


    MoM::MoMExeBase* exeBase = getExeBase();
    if ((0 != exeBase) && (exeOffset + 1 < exeBase->getExeSize()))
    {
        uint8_t* pointer = exeBase->getExeContents() + exeOffset;
        (void)m_game->commitData(pointer, &u8Value, sizeof(u8Value));
        updateValues();
    }
}

void DialogCalculatorAddress::on_lineEdit_CurrentMemValue_returnPressed()
{
    int intValue = 0;
    const char* buf = ui->lineEdit_CurrentMemValue->text().toUtf8().data();
    bool ok = (1 == sscanf(buf, "%d", &intValue));
    if (!ok)
        return;
    uint8_t u8Value = static_cast<uint8_t>(intValue);

    size_t memOffset = 0;
    memOffset = ui->lineEdit_OffsetMem->text().toULong(&ok, 16);
    if (!ok)
        return;

    MoM::MoMProcess* momProcess = m_game->getMoMProcess();
    if ((0 != momProcess) && (memOffset >= (size_t)momProcess->getBaseAddress() + momProcess->getOffsetDatasegment())
             && (memOffset +1 < (size_t)momProcess->getBaseAddress() + momProcess->getBaseAddressSize()))
    {
        uint8_t* pointer = momProcess->getSeg0Pointer() + (memOffset - (size_t)momProcess->getBaseAddress() - momProcess->getOffsetSegment0());
        (void)m_game->commitData(pointer, &u8Value, sizeof(u8Value));
        updateValues();
    }
}

void DialogCalculatorAddress::on_lineEdit_OffsetExe_textChanged(const QString&)
{
    update(ui->lineEdit_OffsetExe);
}

void DialogCalculatorAddress::on_lineEdit_OffsetIDA_textChanged(const QString&)
{
    update(ui->lineEdit_OffsetIDA);
}

void DialogCalculatorAddress::on_lineEdit_OffsetMem_textChanged(const QString&)
{
    update(ui->lineEdit_OffsetMem);
}

void DialogCalculatorAddress::on_lineEdit_OffsetDOS_textChanged(const QString&)
{
    update(ui->lineEdit_OffsetDOS);
}

void DialogCalculatorAddress::slot_gameChanged(const QMoMGamePtr& game)
{
    m_game = game;
    updateValues(QApplication::focusWidget());
}

void DialogCalculatorAddress::slot_gameUpdated()
{
    updateValues(QApplication::focusWidget());
}

void DialogCalculatorAddress::slot_addressChanged(const void* momPointer)
{
    if (m_updating)
        return;
    MoM::UpdateLock lock(m_updating);

    if (0 == m_game)
        return;

    // Clear old fields
    ui->lineEdit_OffsetExe->clear();
    ui->lineEdit_OffsetIDA->clear();
    ui->lineEdit_CurrentFileValue->clear();

    ui->lineEdit_OffsetMem->clear();
    ui->lineEdit_OffsetDOS->clear();
    ui->lineEdit_CurrentMemValue->clear();

    // TODO: Properly distinguish wizards.exe and magic.exe

    QString dosStr;
    dosStr = QString("void* %0").arg((unsigned long)momPointer, 8, 16, QChar('0'));
	QString memStr;

	const MoM::MoMProcess* momProcess = m_game->getMoMProcess();
	if ((0 != momProcess))
	{
		memStr = QString("Base=%0 Size=%1 Seg0=%2 DS=%3")
			.arg((size_t)momProcess->getBaseAddress(), 8, 16, QChar('0'))
			.arg((size_t)momProcess->getBaseAddressSize(), 8, 16, QChar('0'))
			.arg((size_t)momProcess->getOffsetSegment0(), 4, 16, QChar('0'))
			.arg((size_t)momProcess->getOffsetDatasegment(), 4, 16, QChar('0'));
	}

    const MoM::MoMDataSegment* wizardsDataSegment = m_game->getDataSegment();
    // TODO: Sharper boundary check on end-of-memory
    if ((0 != wizardsDataSegment) && (momPointer >= wizardsDataSegment) && (momPointer < (uint8_t*)wizardsDataSegment + 0xFFFFF))
    {
        unsigned dsOffset = ((uint8_t*)momPointer - (uint8_t*)wizardsDataSegment);
        dosStr = QString(" ds:%0").arg(dsOffset, 4, 16, QChar('0'));

        if (dsOffset <= sizeof(MoM::MoMDataSegment))
		{
            // Triggers updates
            ui->lineEdit_OffsetIDA->setText(QString("dseg:%0").arg(dsOffset, 4, 16, QChar('0')));
            ui->comboBox_File->setCurrentIndex(2);  // WIZARDS.EXE
		}
    }

    const MoM::MoMMagicDataSegment* magicDataSegment = m_game->getMagicDataSegment();
    // TODO: Sharper boundary check on end-of-memory
    if ((0 != magicDataSegment) && (momPointer >= magicDataSegment) && (momPointer < (uint8_t*)magicDataSegment + 0xFFFFF))
    {
        unsigned dsOffset = ((uint8_t*)momPointer - (uint8_t*)magicDataSegment);
        dosStr = QString(" ds:%0").arg(dsOffset, 4, 16, QChar('0'));

        if (dsOffset <= sizeof(MoM::MoMMagicDataSegment))
		{
            // Triggers updates
            ui->lineEdit_OffsetIDA->setText(QString("dseg:%0").arg(dsOffset, 4, 16, QChar('0')));
            ui->comboBox_File->setCurrentIndex(1);  // MAGIC.EXE
        }
    }

    const uint8_t* seg0Pointer = m_game->getSeg0Pointer();
    if ((0 != seg0Pointer) && (momPointer >= seg0Pointer))
    {
        int seg0Offset = ((uint8_t*)momPointer - (uint8_t*)seg0Pointer);
		// TODO: Sharper boundary check on end-of-memory
		if (seg0Offset <= 0xFFFFF)
		{
			dosStr += QString(" %0:%1").arg(seg0Offset / MoM::gPARAGRAPH_SIZE, 4, 16, QChar('0')).arg(seg0Offset % MoM::gPARAGRAPH_SIZE, 1, 16, QChar('0'));
			if (0 != momProcess)
			{
				size_t memOffset = (size_t)(momProcess->getBaseAddress() + momProcess->getOffsetSegment0() + seg0Offset);
				memStr = QString("%0 (%1)").arg(memOffset, 8, 16, QChar('0')).arg(memStr);
			}
		}
    }

	MoM::MoMExeWizards* wizardsExe = m_game->getWizardsExe();
    if ((0 != wizardsExe) && (momPointer >= wizardsExe->getExeContents()) && (momPointer < wizardsExe->getExeContents() + wizardsExe->getExeSize()))
    {
        int wizardsExeOffset = ((uint8_t*)momPointer - (uint8_t*)wizardsExe->getExeContents());
        dosStr += QString(" wizards.exe:%0").arg(wizardsExeOffset, 5, 16, QChar('0'));

        // Triggers updates
        ui->lineEdit_OffsetExe->setText(QString("%0").arg(wizardsExeOffset, 5, 16, QChar('0')));
        ui->comboBox_File->setCurrentIndex(2);  // WIZARDS.EXE
    }

    MoM::MoMExeMagic* magicExe = m_game->getMagicExe();
    if ((0 != magicExe) && (momPointer >= magicExe->getExeContents()) && (momPointer < magicExe->getExeContents() + magicExe->getExeSize()))
    {
        int magicExeOffset = ((uint8_t*)momPointer - (uint8_t*)magicExe->getExeContents());
        dosStr += QString(" magic.exe:%0").arg(magicExeOffset, 5, 16, QChar('0'));
        ui->comboBox_File->setCurrentIndex(1);  // MAGIC.EXE
    }

    auto saveGameRef = m_game->getSaveGame();
    if ((0 != saveGameRef.pointer) && (momPointer >= saveGameRef.pointer)
            && (momPointer < saveGameRef.pointer + saveGameRef.size))
    {
        int saveGameOffset = ((uint8_t*)momPointer - (uint8_t*)saveGameRef.pointer);
        dosStr += QString(" save:%0").arg(saveGameOffset, 5, 16, QChar('0'));
    }

    ui->lineEdit_OffsetDOS->setText(dosStr);
	ui->lineEdit_OffsetMem->setText(memStr);

//    updateCurrentValue(ui->lineEdit_CurrentFileValue, (const uint8_t*)momPointer);
}

void DialogCalculatorAddress::convertDOStoMemOffset(const QString& sDOSOffset, size_t& memOffset)
{
    memOffset = 0;

    const MoM::MoMProcess* momProcess = m_game->getMoMProcess();
    if (0 == momProcess)
        return;

    unsigned segField = 0;
    unsigned offsetField = 0;
    if (2 == sscanf(sDOSOffset.toUtf8().data(), " %x:%x", &segField, &offsetField))
    {
        memOffset = (size_t)momProcess->getBaseAddress() + momProcess->getOffsetSegment0() + segField * MoM::gPARAGRAPH_SIZE + offsetField;
    }
    else if ((1 == sscanf(sDOSOffset.toUtf8().data(), " dseg:%x", &offsetField))
             || (1 == sscanf(sDOSOffset.toUtf8().data(), " ds:%x", &offsetField))
             || (1 == sscanf(sDOSOffset.toUtf8().data(), " DS:%x", &offsetField)))
    {
        memOffset = (size_t)momProcess->getBaseAddress() + momProcess->getOffsetDatasegment() + offsetField;
    }
    else
    {
    }
}

void DialogCalculatorAddress::convertExetoExeString(size_t exeOffset, QString& sExeOffset)
{
    sExeOffset.clear();
    if (0 != exeOffset)
    {
        sExeOffset = QString("%0").arg(exeOffset, 5, 16, QChar('0'));
    }
}

void DialogCalculatorAddress::convertExetoIDAString(size_t exeOffset, QString& sIDAOffset)
{
    sIDAOffset.clear();

    const MoM::MoMExeBase* exeBase = getExeBase();

    size_t segNr = 0;
    uint16_t segOffset = 0;
    size_t stubNr = 0;
    uint16_t stubOffset = 0;
    size_t ovlNr = 0;
    uint16_t ovlOffset = 0;
    uint16_t dsegOffset = 0;

    if ((0 != exeBase) && exeBase->convertExeOffset_to_SegOffset(exeOffset, segNr, segOffset))
    {
        sIDAOffset = QString("seg%0:%1").arg(segNr, 3, 10, QChar('0')).arg(segOffset, 4, 16, QChar('0'));
    }
    else if ((0 != exeBase) && exeBase->convertExeOffset_to_StubOffset(exeOffset, stubNr, stubOffset))
    {
        sIDAOffset = QString("stub%0:%1").arg(stubNr, 3, 10, QChar('0')).arg(stubOffset, 4, 16, QChar('0'));
    }
    else if ((0 != exeBase) && exeBase->convertExeOffset_to_DsegOffset(exeOffset, dsegOffset))
    {
        sIDAOffset = QString("dseg:%0").arg(dsegOffset, 4, 16, QChar('0'));
    }
    else if ((0 != exeBase) && exeBase->convertExeOffset_to_OvlOffset(exeOffset, ovlNr, ovlOffset))
    {
        sIDAOffset = QString("ovr%0:%1").arg(ovlNr, 3, 10, QChar('0')).arg(ovlOffset, 4, 16, QChar('0'));
    }
    else
    {
    }
}

void DialogCalculatorAddress::convertExetoMemOffset(size_t exeOffset, size_t& memOffset)
{
    memOffset = 0;

    const MoM::MoMExeBase* exeBase = getExeBase();
    const MoM::MoMProcess* momProcess = m_game->getMoMProcess();

    uint16_t dsegOffset = 0;
    if ((0 != exeBase) && (0 != momProcess) && exeBase->convertExeOffset_to_DsegOffset(exeOffset, dsegOffset))
    {
        memOffset = (size_t)momProcess->getBaseAddress() + momProcess->getOffsetDatasegment() + dsegOffset;
    }
    else if ((0 != exeBase) && (0 != momProcess) && (exeOffset >= exeBase->getOffsetLoadModule()) && (exeOffset < exeBase->getOffsetDataSegment()))
    {
        memOffset = (size_t)momProcess->getBaseAddress() + momProcess->getOffsetCode() + (exeOffset - exeBase->getOffsetLoadModule());
    }
}

void DialogCalculatorAddress::convertIDAtoExeOffset(const QString& sIDAOffset, size_t& exeOffset)
{
    exeOffset = 0;
    if (0 == m_game)
        return;
    MoM::MoMExeBase* exeBase = getExeBase();

    unsigned segField = 0;
    unsigned stubField = 0;
    unsigned ovrField = 0;
    unsigned offsetField = 0;
    if (2 == sscanf(sIDAOffset.toUtf8().data(), " seg%u:%x", &segField, &offsetField))
    {
        size_t segNr = segField;
        uint16_t segOffset = offsetField;
        if ((0 != exeBase) && exeBase->convertSegOffset_to_ExeOffset(segNr, segOffset, exeOffset))
        {
        }
    }
    else if (2 == sscanf(sIDAOffset.toUtf8().data(), " stub%u:%x", &stubField, &offsetField))
    {
        size_t stubNr = stubField;
        uint16_t stubOffset = offsetField;
        if ((0 != exeBase) && exeBase->convertStubOffset_to_ExeOffset(stubNr, stubOffset, exeOffset))
        {
        }
    }
    else if (2 == sscanf(sIDAOffset.toUtf8().data(), " ovr%u:%x", &ovrField, &offsetField))
    {
        size_t ovlNr = ovrField;
        uint16_t ovlOffset = offsetField;
        if ((0 != exeBase) && exeBase->convertOvlOffset_to_ExeOffset(ovlNr, ovlOffset, exeOffset))
        {
        }
    }
    else if ((1 == sscanf(sIDAOffset.toUtf8().data(), " dseg:%x", &offsetField))
            || (1 == sscanf(sIDAOffset.toUtf8().data(), " ds:%x", &offsetField)))
    {
        uint16_t dsegOffset = offsetField;
        if ((0 != exeBase) && exeBase->convertDsegOffset_to_ExeOffset(dsegOffset, exeOffset))
        {
        }
    }
}

void DialogCalculatorAddress::convertMemtoExeOffset(size_t memOffset, size_t& exeOffset)
{
    exeOffset = 0;
    const MoM::MoMProcess* momProcess = m_game->getMoMProcess();
    MoM::MoMExeBase* exeBase = getExeBase();
    if ((0 != momProcess) && (0 != exeBase))
    {
        if ((memOffset >= (size_t)momProcess->getBaseAddress() + momProcess->getOffsetCode())
                && (memOffset < (size_t)momProcess->getBaseAddress() + momProcess->getOffsetDatasegment() + 0xFFFF))
        {
//            uint16_t dsegOffset = (uint16_t)(memOffset - (size_t)momProcess->getBaseAddress() - momProcess->getOffsetDatasegment());
//            (void)exeBase->convertDsegOffset_to_ExeOffset(dsegOffset, exeOffset);
            exeOffset = exeBase->getOffsetLoadModule() + (memOffset - (size_t)momProcess->getBaseAddress() - momProcess->getOffsetCode());
        }
    }
}

void DialogCalculatorAddress::convertMemtoMemString(size_t memOffset, QString& sMemOffset)
{
    sMemOffset.clear();

    if (0 != memOffset)
    {
        sMemOffset = QString("%0").arg(memOffset, 8, 16, QChar('0'));
    }
}

void DialogCalculatorAddress::convertMemtoDOSString(size_t memOffset, QString& sDOSOffset)
{
    sDOSOffset.clear();
    if (0 == m_game)
        return;

    const MoM::MoMProcess* momProcess = m_game->getMoMProcess();
    if (0 != momProcess)
    {
        size_t seg0Pointer = (size_t)momProcess->getBaseAddress() + (size_t)momProcess->getOffsetSegment0();
        size_t dsegPointer = (size_t)momProcess->getBaseAddress() + (size_t)momProcess->getOffsetDatasegment();
        if ((memOffset >= dsegPointer) && (memOffset < dsegPointer + 0xFFFF))
        {
            sDOSOffset += QString("DS:%0").arg(memOffset - dsegPointer, 4, 16, QChar('0'));
        }
        if ((memOffset >= seg0Pointer) && (memOffset < seg0Pointer + 0xFFFFF))
        {
            size_t seg0Offset = memOffset - seg0Pointer;
            sDOSOffset += QString(" %0:%1").arg(seg0Offset / MoM::gPARAGRAPH_SIZE, 4, 16, QChar('0')).arg(seg0Offset % MoM::gPARAGRAPH_SIZE, 1, 16, QChar('0'));
        }
    }
}

MoM::MoMExeBase *DialogCalculatorAddress::getExeBase()
{
    MoM::MoMExeBase* exeBase = 0;
    if (0 == m_game)
    {
    }
    else if (ui->comboBox_File->currentText() == "MAGIC.EXE")
    {
        exeBase = m_game->getMagicExe();
    }
    else if (ui->comboBox_File->currentText() == "WIZARDS.EXE")
    {
        exeBase = m_game->getWizardsExe();
    }
    else
    {
    }
    return exeBase;
}

void DialogCalculatorAddress::updateCurrentValue(QLineEdit* lineEdit, const uint8_t* pointer)
{
    QString textCurrentValue;
    if (0 != pointer)
    {
        uint8_t lo = *pointer;
        uint8_t hi = *(pointer + 1);
        int16_t value = *(const int16_t*)pointer;
        textCurrentValue = QString("%0h %1h / %2 %3 / %4")
                .arg((unsigned)lo, 2, 16, QChar('0')).arg((unsigned)hi, 2, 16, QChar('0'))
                .arg((int)(int8_t)lo).arg((int)(int8_t)hi)
                .arg((int)value);
    }
    lineEdit->setText(textCurrentValue);
}
