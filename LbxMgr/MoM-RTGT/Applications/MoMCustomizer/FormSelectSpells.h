#ifndef FORMSELECTSPELLS_H
#define FORMSELECTSPELLS_H

#include <QWidget>

namespace Ui {
class FormSelectSpells;
}

class FormSelectSpells : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormSelectSpells(QWidget *parent = 0);
    ~FormSelectSpells();
    
private slots:
    void on_pushButton_OK_clicked();

signals:
    void signal_accepted();

private:
    Ui::FormSelectSpells *ui;
};

#endif // FORMSELECTSPELLS_H
