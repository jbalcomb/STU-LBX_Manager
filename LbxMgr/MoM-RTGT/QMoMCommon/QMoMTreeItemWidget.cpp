#include "QMoMTreeItemWidget.h"

QMoMTreeItemWidgetBase::QMoMTreeItemWidgetBase(const QMoMGamePtr& game, const QString& feature, const QString& value) :
    QTreeWidgetItem(),
    m_game(game)
{
    setText(COL_Feature, feature);
    setText(COL_Value, value);
}

void* QMoMTreeItemWidgetBase::getMoMPointer() const
{
    return 0;
}

QList<QAction*> QMoMTreeItemWidgetBase::requestActions(QObject*)
{
    return QList<QAction*>();
}

void QMoMTreeItemWidgetBase::slotAction()
{
}

QString QMoMTreeItemWidgetBase::toString() const
{
    return QTreeWidgetItem::text(COL_Value);
}
