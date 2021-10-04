#ifndef FORMMAGICPICKSCREEN_H
#define FORMMAGICPICKSCREEN_H

#include <QWidget>

namespace Ui {
class FormMagicPickScreen;
}

class FormMagicPickScreen : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormMagicPickScreen(QWidget *parent = 0);
    ~FormMagicPickScreen();
    
signals:
    void signal_accepted();

private slots:
    void on_pushButton_OK_clicked();

private:
    Ui::FormMagicPickScreen *ui;
};

#endif // FORMMAGICPICKSCREEN_H
