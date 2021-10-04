#ifndef DIALOGADDARTIFACT_H
#define DIALOGADDARTIFACT_H

#include <QDialog>

class QGraphicsScene;
class QGraphicsSimpleTextItem;

namespace Ui {
class DialogAddArtifact;
}

#include "QMoMDialogBase.h"

class DialogAddArtifact : public QMoMDialogBase
{
    Q_OBJECT
    
public:
    explicit DialogAddArtifact(QWidget *parent = 0);
    ~DialogAddArtifact();
    
public slots:
    void slot_gameChanged(const QMoMGamePtr& game);
    void slot_gameUpdated();

private slots:
    void on_pushButton_AddArtifact_clicked();

    void on_comboBox_Artifacts_currentIndexChanged(int index);

private:
    QGraphicsSimpleTextItem* addText(const QPointF& pos, const QString& text, const QString& helpText);

    QGraphicsScene* m_scene;

    Ui::DialogAddArtifact *ui;
};

#endif // DIALOGADDARTIFACT_H
