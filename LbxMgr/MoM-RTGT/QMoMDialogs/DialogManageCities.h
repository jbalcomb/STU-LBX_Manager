#ifndef DIALOGMANAGECITIES_H
#define DIALOGMANAGECITIES_H

#include <QMainWindow>
class QAbstractButton;
class QModelIndex;

#include "QMoMSharedPointers.h"

namespace MoM {
    enum eResource;
}

namespace Ui {
class DialogManageCities;
}

class DialogManageCities : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit DialogManageCities(QWidget *parent = 0);
    ~DialogManageCities();
    
private slots:
    void on_buttonBox_clicked(QAbstractButton* button);
    void on_tableWidget_Cities_cellChanged(int row, int column);
    void on_tableWidget_Cities_clicked(const QModelIndex &index);
    void on_tableWidget_Cities_customContextMenuRequested(const QPoint &pos);
    void slot_gameChanged(const QMoMGamePtr& game);
    void slot_gameUpdated();
    void slot_ItemActionCities();
    void slot_ItemActionSummary();

    void on_tableWidget_Summary_customContextMenuRequested(const QPoint &pos);

private:
    static QIcon getResourceIcon(MoM::eResource resource, int value);
    void update();

    QMoMGamePtr m_game;
    bool m_updating;
    int m_columnFarmers;
    int m_columnBuy;

private:
    Ui::DialogManageCities *ui;
};

#endif // DIALOGMANAGECITIES_H
