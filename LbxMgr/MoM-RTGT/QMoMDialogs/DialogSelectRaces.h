#ifndef DIALOGSELECTRACES_H
#define DIALOGSELECTRACES_H

#include "QMoMDialogBase.h"

namespace Ui {
class DialogSelectRaces;
}

class DialogSelectRaces : public QMoMDialogBase
{
    Q_OBJECT
    
public:
    explicit DialogSelectRaces(QWidget *parent);
    ~DialogSelectRaces();
    
public slots:
    virtual void slot_gameChanged(const QMoMGamePtr& game);
    virtual void slot_gameUpdated();

private slots:
    void on_pushButton_ReplaceRace_clicked();

private:
    Ui::DialogSelectRaces *ui;
};

#endif // DIALOGSELECTRACES_H
