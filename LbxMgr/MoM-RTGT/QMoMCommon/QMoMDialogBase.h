#ifndef QMOMDIALOGBASE_H
#define QMOMDIALOGBASE_H

#include <QDialog>

#include "QMoMSharedPointers.h"

class QMoMDialogBase : public QDialog
{
public:
    explicit QMoMDialogBase(QWidget *parent);
    ~QMoMDialogBase();

public slots:
    virtual void slot_gameChanged(const QMoMGamePtr& game) = 0;
    virtual void slot_gameUpdated() = 0;

protected:
    void postInitialize();
    void preFinalize();

protected:
    QMoMGamePtr m_game;
    QFont m_font;
};

#endif // QMOMDIALOGBASE_H
