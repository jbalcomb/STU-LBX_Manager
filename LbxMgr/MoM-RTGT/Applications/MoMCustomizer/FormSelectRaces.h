#ifndef FORMSELECTRACES_H
#define FORMSELECTRACES_H

#include <QWidget>

namespace Ui {
class FormSelectRaces;
}

class FormSelectRaces : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormSelectRaces(QWidget *parent = 0);
    ~FormSelectRaces();
    
signals:
    void signal_accepted();

private:
    Ui::FormSelectRaces *ui;
};

#endif // FORMSELECTRACES_H
