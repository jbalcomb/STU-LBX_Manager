#ifndef FORMSELECTWIZARD_H
#define FORMSELECTWIZARD_H

#include <QWidget>

#include <QMoMSharedPointers.h>

namespace Ui {
class FormSelectWizard;
}

class FormSelectWizard : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormSelectWizard(QWidget *parent, QMoMGamePtr& game);
    ~FormSelectWizard();
    
private slots:
    void on_pushButton_Custom_clicked();

    void on_pushButton_Wizard_0_clicked();

signals:
    void signal_accepted();

private:
    Ui::FormSelectWizard *ui;
    QMoMGamePtr m_game;
};

#endif // FORMSELECTWIZARD_H
