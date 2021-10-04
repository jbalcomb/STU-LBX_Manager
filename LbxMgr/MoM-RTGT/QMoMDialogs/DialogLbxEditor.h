#ifndef DIALOGLBXEDITOR_H
#define DIALOGLBXEDITOR_H

#include <QDialog>
#include <QFileDialog>
class QAbstractButton;
class QGraphicsScene;
class QGraphicsView;

#include "MoMFli.h"
#include "MoMLbxBase.h"
#include "QMoMAnimation.h"
#include "QMoMLbx.h"
#include "QMoMSharedPointers.h"

namespace Ui {
class DialogLbxEditor;
}

class DialogLbxEditor : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogLbxEditor(QWidget *parent = 0);
    ~DialogLbxEditor();

private:
    QString constructComboBoxTitle(int lbxIndex, int subIndex);
    QString constructFileTitle(const QString& baseName, int lbxIndex, int subIndex);
    QString constructFrameFilename(const QString& bitmapFilename, int frameNr);
    bool decodeGlyph(const std::vector<uint8_t>& data, int fontIndex, int characterIndex, int left, int top, QMoMImagePtr& image);
    void exportFont(size_t lbxIndex, const QString& directory);
    void loadBitmap(const QString& filename);
    void loadLbx(const QString& filename);
    void loadPaletteForFile(const QString& filename);
    void listBitmapFiles(const QString& directory);
    void processFont(int lbxIndex);
    void processPalette(int lbxIndex);
    void updateBitmapImage(const QString& bitmapfilename);
    void updateImage(QGraphicsView* view, const QMoMAnimation& curAnimation, int line = 0, bool clearImage = true);
    void updateLbxImage(int lbxIndex);
    void updateLbxText(int lbxIndex, int lbxSubIndex);

private slots:
    void on_comboBox_FileIndex_currentIndexChanged(const QString &arg1);
    void on_comboBox_LbxIndex_currentIndexChanged(int index);
    void on_pushButton_Load_clicked();
    void on_pushButton_Replace_clicked();
    void on_pushButton_SavePics_clicked();
    void on_pushButton_ConvertAll_clicked();

private:
    Ui::DialogLbxEditor *ui;

private:
    QGraphicsScene* m_sceneBitmap;
    QGraphicsScene* m_sceneLbx;
    QFileDialog* m_filedialogLoad;
    QFileDialog* m_filedialogSave;
    MoM::QMoMPalette m_colorTable;
    QString m_bitmapDirectory;
    QString m_lbxDirectory;
    QString m_bitmapFilename;
    QString m_lbxFilename;
    MoM::MoMLbxBase m_lbx;
    MoM::MoMFli m_fli;
    QVector<QMoMAnimation> m_lbxAnimations;
    QMoMAnimation m_bitmapAnimation;
};


#endif // DIALOGLBXEDITOR_H
