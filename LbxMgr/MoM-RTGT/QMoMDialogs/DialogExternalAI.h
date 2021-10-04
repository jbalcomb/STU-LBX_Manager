#ifndef DIALOGEXTERNALAI_H
#define DIALOGEXTERNALAI_H

#include "MoMHookManager.h"
#include "QMoMDialogBase.h"

namespace Ui {
class DialogExternalAI;
}

class DialogExternalAI : public QMoMDialogBase
{
    Q_OBJECT
    
public:
    explicit DialogExternalAI(QWidget *parent = 0);
    ~DialogExternalAI();
    
private slots:
    void on_pushButton_InsertHook_clicked();
    void on_pushButton_RaiseHook_clicked();
    void on_pushButton_WaitForHook_clicked();
    void on_pushButton_RetractHook_clicked();
    void on_pushButton_ReleaseHook_clicked();
    void slot_gameChanged(const QMoMGamePtr& game);
    void slot_gameUpdated();

private:
    Ui::DialogExternalAI *ui;

    MoM::MoMHookManager* hookManager;
};

#endif // DIALOGEXTERNALAI_H
