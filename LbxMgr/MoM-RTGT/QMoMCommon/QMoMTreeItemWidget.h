#ifndef QMOMTREEITEMWIDGET_H
#define QMOMTREEITEMWIDGET_H

#include <QActionGroup>
#include <QTreeWidgetItem>

#include "QMoMResources.h"
#include "QMoMSharedPointers.h"

///////////////////////////////////////////

class QMoMTreeItemWidgetBase : public QTreeWidgetItem
{
public:
    enum { COL_Feature, COL_Value, COL_Comment };

    QMoMTreeItemWidgetBase(const QMoMGamePtr& game, const QString& feature, const QString& value);
    virtual void* getMoMPointer() const;
    virtual QList<QAction*> requestActions(QObject*);
    virtual void slotAction();
    virtual QString toString() const;
protected:
    QMoMGamePtr m_game;
};

///////////////////////////////////////////

template<typename Number>
class NumberTreeItem : public QMoMTreeItemWidgetBase
{
public:
    NumberTreeItem(const QMoMGamePtr& game, const QString& feature, Number* t, Number mask = 0);
    virtual void setData(int column, int role, const QVariant &value);
    virtual QString toString() const;
    virtual void* getMoMPointer() const
    {
        return m_ptr;
    }
private:
    Number* m_ptr;
    Number m_mask;
    unsigned m_shift;
};


template<typename Number>
NumberTreeItem<Number>::NumberTreeItem(const QMoMGamePtr& game, const QString& feature, Number* t, Number mask) :
    QMoMTreeItemWidgetBase(game, feature, ""),
    m_ptr(t),
    m_mask(mask),
    m_shift(0)
{
    for (unsigned i = 0; (0 != mask) && (i < 32); ++i)
    {
        if (((mask >> i) << i) != mask)
        {
            break;
        }
        m_shift = i;
    }
    setFlags(flags() | Qt::ItemIsEditable);
    QMoMTreeItemWidgetBase::setData(COL_Value, Qt::EditRole, toString());
}

template<typename Number>
void NumberTreeItem<Number>::setData(int column, int role, const QVariant &value)
{
    if ((COL_Value == column) && (Qt::EditRole == role))
    {
        Number newValue = static_cast<Number>(value.toInt());
        if (0 != m_mask)
        {
            unsigned tmp = ((unsigned)*m_ptr & ~m_mask);
            newValue = static_cast<Number>(tmp | (((unsigned)value.toInt() << m_shift) & m_mask));
        }
        (void)m_game->commitData(m_ptr, &newValue, sizeof(Number));
        QMoMTreeItemWidgetBase::setData(COL_Value, role, toString());
    }
    else
    {
        QMoMTreeItemWidgetBase::setData(column, role, value);
    }
}

template<typename Number>
QString NumberTreeItem<Number>::toString() const
{
    Number value = *m_ptr;
    if (0 != m_mask)
    {
        value =static_cast<Number>(((unsigned)*m_ptr & m_mask) >> m_shift);
    }
    return toQStr(value);
}

/////////////////////////////////////////

template<typename Enum>
class EnumTreeItem : public QMoMTreeItemWidgetBase
{
public:
    EnumTreeItem(const QMoMGamePtr& game, const QString& feature, Enum* e, Enum max);

    virtual void* getMoMPointer() const
    {
        return m_ptr;
    }

    virtual void setData(int column, int role, const QVariant &value);

    virtual QList<QAction*> requestActions(QObject* parent);
    virtual void slotAction();

    virtual QString toString() const;

private:
    void addAction(Enum e);
    void updateIcon(Enum e);

    // Configuration
    Enum* m_ptr;
    Enum m_max;

    // Status

    // Keep track of the action group
    // m_actionGroup is deleted by its parent (the context menu)
    QActionGroup* m_actionGroup;
};

template<typename Enum>
EnumTreeItem<Enum>::EnumTreeItem(const QMoMGamePtr& game, const QString& feature, Enum* e, Enum max) :
    QMoMTreeItemWidgetBase(game, feature, ""),
    m_ptr(e),
    m_max(max),
    m_actionGroup(0)
{
    setFlags(flags() | Qt::ItemIsEditable);
    QMoMTreeItemWidgetBase::setData(COL_Value, Qt::EditRole, toString());
    updateIcon(*m_ptr);
}

template<typename Enum>
void EnumTreeItem<Enum>::setData(int column, int role, const QVariant &value)
{
    if ((COL_Value == column) && (Qt::EditRole == role))
    {
        Enum newValue = static_cast<Enum> (value.toInt());
        (void)m_game->commitData(m_ptr, &newValue, sizeof(Enum));
        QMoMTreeItemWidgetBase::setData(COL_Value, role, toString());
        updateIcon(*m_ptr);
    }
    else
    {
        QMoMTreeItemWidgetBase::setData(column, role, value);
    }
}

template<typename Enum>
QList<QAction*> EnumTreeItem<Enum>::requestActions(QObject* parent)
{
    m_actionGroup = new QActionGroup(parent);

    for (Enum e = (Enum)0; e < m_max; MoM::inc(e))
    {
        addAction(e);
    }

    return m_actionGroup->actions();
}

template<typename Enum>
void EnumTreeItem<Enum>::slotAction()
{
    QAction* action = m_actionGroup->checkedAction();
    setData(COL_Value, Qt::EditRole, action->data());
}

template<typename Enum>
QString EnumTreeItem<Enum>::toString() const
{
    return MoM::QMoMResources::instance().getName(*m_ptr);
}

// Private

template<typename Enum>
void EnumTreeItem<Enum>::addAction(Enum e)
{
    assert(m_actionGroup != 0);

    QString rawName = prettyQStr(e);
    if (!rawName.isEmpty() && (rawName[0] == '<') && (e != *m_ptr))
    {
        // Skip <Unknown> entries, unless one of them is selected
    }
    else
    {
        auto name = MoM::QMoMResources::instance().getName(e);
        QAction* action = new QAction(name, m_actionGroup);
        QMoMIconPtr iconPtr = MoM::QMoMResources::instance().getIcon(e);
        if (!iconPtr.isNull())
        {
            action->setIcon(*iconPtr);
        }
        action->setCheckable(true);
        action->setData(QVariant((int)e));
        if (e == *m_ptr)
        {
            action->setChecked(true);
        }
    }
}

template<typename Enum>
void EnumTreeItem<Enum>::updateIcon(Enum e)
{
    QMoMIconPtr iconPtr = MoM::QMoMResources::instance().getIcon(e);
    if (!iconPtr.isNull())
    {
        QMoMTreeItemWidgetBase::setIcon(COL_Value, *iconPtr);
    }
}

/////////////////////////////////////////

template<typename Bitmask, typename Enum>
class BitmaskTreeItem : public QMoMTreeItemWidgetBase
{
public:
    BitmaskTreeItem(const QMoMGamePtr& game, const QString& feature, Bitmask* bitmask, Enum min, Enum max);

    virtual void* getMoMPointer() const
    {
        return m_ptr;
    }

    virtual void setData(int column, int role, const QVariant &value);

    virtual QList<QAction*> requestActions(QObject* parent);
    virtual void slotAction();

    virtual QString toString();

private:
    void addAction(Enum e);
    bool has(Enum e) const;

    // Configuration
    Bitmask* m_ptr;
    Enum m_min;
    Enum m_max;

    // Status

    // Keep track of the action group
    // m_actionGroup is deleted by its parent (the context menu)
    QActionGroup* m_actionGroup;
};

template<typename Bitmask, typename Enum>
BitmaskTreeItem<Bitmask, Enum>::BitmaskTreeItem(const QMoMGamePtr& game, const QString& feature, Bitmask* bitmask, Enum min, Enum max) :
    QMoMTreeItemWidgetBase(game, feature, ""),
    m_ptr(bitmask),
    m_min(min),
    m_max(max),
    m_actionGroup()
{
    setFlags(flags() | Qt::ItemIsEditable);
    QMoMTreeItemWidgetBase::setData(COL_Value, Qt::EditRole, toString());
}

template<typename Bitmask, typename Enum>
void BitmaskTreeItem<Bitmask, Enum>::setData(int column, int role, const QVariant &value)
{
    if ((COL_Value == column) && (Qt::EditRole == role))
    {
        Bitmask newValue = static_cast<Bitmask>(value.toUInt());
        (void)m_game->commitData(m_ptr, &newValue, sizeof(Bitmask));
        QMoMTreeItemWidgetBase::setData(COL_Value, role, toString());
    }
    else
    {
        QMoMTreeItemWidgetBase::setData(column, role, value);
    }
}

template<typename Bitmask, typename Enum>
void BitmaskTreeItem<Bitmask, Enum>::addAction(Enum e)
{
    assert(m_actionGroup != 0);

    QString rawName = prettyQStr(e);
    if (!rawName.isEmpty() && (rawName[0] == '<') && !has(e))
    {
        // Skip <Unknown> entries, unless one of them is selected
    }
    else
    {
        auto name = MoM::QMoMResources::instance().getName(e);
        QAction* action = new QAction(name, m_actionGroup);
        action->setCheckable(true);
        action->setData(QVariant((int)e));
        if (has(e))
        {
            action->setChecked(true);
        }
    }
}

template<typename Bitmask, typename Enum>
QList<QAction*> BitmaskTreeItem<Bitmask, Enum>::requestActions(QObject* parent)
{
    m_actionGroup = new QActionGroup(parent);
    m_actionGroup->setExclusive(false);
    for (Enum e = m_min; e < m_max; MoM::inc(e))
    {
        addAction(e);
    }
    return m_actionGroup->actions();
}

template<typename Bitmask, typename Enum>
void BitmaskTreeItem<Bitmask, Enum>::slotAction()
{
    Bitmask bitmask = 0;
    Enum e = m_min;
    for (int i = 0; i < m_actionGroup->actions().count(); ++i, MoM::inc(e))
    {
        if (m_actionGroup->actions().at(i)->isChecked())
        {
            bitmask |= (1 << i);
        }
    }
    setData(COL_Value, Qt::EditRole, QVariant((unsigned)bitmask));
}

template<typename Bitmask, typename Enum>
bool BitmaskTreeItem<Bitmask, Enum>::has(Enum e) const
{
    Bitmask mask = (1 << ((unsigned)e - (unsigned)m_min));
    return ((*m_ptr & mask) != 0);
}

template<typename Bitmask, typename Enum>
QString BitmaskTreeItem<Bitmask, Enum>::toString()
{
    QString result;
    for (Enum e = m_min; e < m_max; MoM::inc(e))
    {
        if (has(e))
        {
            if (!result.isEmpty())
            {
                result += ", ";
            }
            QString name = MoM::QMoMResources::instance().getName(e);
            name.replace("Immunity", "Imm");
            result += name;
        }
    }
    return result;
}

///////////////////////////////////////////

#endif // QMOMTREEITEMWIDGET_H
