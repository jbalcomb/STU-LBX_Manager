#ifndef FORMWIZARDSNAME_H
#define FORMWIZARDSNAME_H

#include <QWidget>

namespace Ui {
class FormWizardsName;
}

class FormWizardsName : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormWizardsName(QWidget *parent = 0);
    ~FormWizardsName();
    
private slots:

private:
    Ui::FormWizardsName *ui;
};

#endif // FORMWIZARDSNAME_H
