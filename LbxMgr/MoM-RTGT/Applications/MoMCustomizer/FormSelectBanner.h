#ifndef FORMSELECTBANNER_H
#define FORMSELECTBANNER_H

#include <QWidget>

namespace Ui {
class FormSelectBanner;
}

class FormSelectBanner : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormSelectBanner(QWidget *parent = 0);
    ~FormSelectBanner();
    
signals:
    void signal_accepted();

private:
    Ui::FormSelectBanner *ui;
};

#endif // FORMSELECTBANNER_H
