#ifndef FORMGAMEOPTIONS_H
#define FORMGAMEOPTIONS_H

#include <QWidget>

#include <QMoMSharedPointers.h>

namespace Ui {
class FormGameOptions;
}

class FormGameOptions : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormGameOptions(QWidget *parent, QMoMGamePtr& game);
    ~FormGameOptions();
    
private slots:
    void on_pushButton_OK_clicked();

    void on_pushButton_Cancel_clicked();

signals:
    void signal_accepted();
    void signal_rejected();

private:
    Ui::FormGameOptions *ui;
    QMoMGamePtr m_game;
};

#endif // FORMGAMEOPTIONS_H
