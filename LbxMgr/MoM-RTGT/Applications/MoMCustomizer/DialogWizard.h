#ifndef DIALOGWIZARD_H
#define DIALOGWIZARD_H

#include <QDialog>

#include <QMoMSharedPointers.h>

namespace Ui {
class DialogWizard;
}

class DialogWizard : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogWizard(QWidget *parent = 0);
    ~DialogWizard();
    
protected:
    virtual void keyPressEvent(class QKeyEvent* event);

private slots:
    void slot_accepted();
    void slot_rejected();
    void slot_selectGameOptions();
    void slot_selectWizard();
    void slot_selectName();
    void slot_selectMagicPicks();
    void slot_selectSpells();
    void slot_selectRaces();
    void slot_selectBanner();

signals:
    void signal_accepted();
    void signal_rejected();

private:
    Ui::DialogWizard *ui;
    QWidget* m_form;
    QMoMGamePtr m_game;
};

#endif // DIALOGWIZARD_H
