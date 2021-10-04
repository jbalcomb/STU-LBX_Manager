// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef DIALOGCALCULATORADDRESS_H
#define DIALOGCALCULATORADDRESS_H

#include "QMoMDialogBase.h"

namespace MoM {
    class MoMExeBase;
}

namespace Ui {
    class DialogCalculatorAddress;
}

class DialogCalculatorAddress : public QMoMDialogBase
{
    Q_OBJECT

public:
    explicit DialogCalculatorAddress(QWidget *parent = 0);
    ~DialogCalculatorAddress();

public slots:
    void slot_gameChanged(const QMoMGamePtr& game);
    void slot_gameUpdated();
    void slot_addressChanged(const void* momPointer);

private:
    void convertDOStoMemOffset(const QString& sDOSOffset, size_t& memOffset);
    void convertExetoExeString(size_t exeOffset, QString& sExeOffset);
    void convertExetoIDAString(size_t exeOffset, QString& sIDAOffset);
    void convertExetoMemOffset(size_t exeOffset, size_t& memOffset);
    void convertIDAtoExeOffset(const QString& sIDAOffset, size_t& exeOffset);
    void convertMemtoExeOffset(size_t memOffset, size_t& exeOffset);
    void convertMemtoMemString(size_t memOffset, QString& sMemOffset);
    void convertMemtoDOSString(size_t memOffset, QString& sDOSOffset);
    MoM::MoMExeBase* getExeBase();

    void update(const QWidget* originator = 0);
    void updateValues(const QWidget* originator = 0);
    void updateCurrentValue(class QLineEdit* lineEdit, const uint8_t* pointer);

private:
    // CONFIG

    // STATUS
    Ui::DialogCalculatorAddress *ui;

    bool m_updating;

private slots:
    void on_lineEdit_CurrentFileValue_returnPressed();
    void on_lineEdit_CurrentMemValue_returnPressed();
    void on_lineEdit_OffsetMem_textChanged(const QString& arg1);
    void on_lineEdit_OffsetIDA_textChanged(const QString& arg1);
    void on_lineEdit_OffsetExe_textChanged(const QString& arg1);
    void on_lineEdit_OffsetDOS_textChanged(const QString& arg1);
};

#endif // DIALOGCALCULATORADDRESS_H
