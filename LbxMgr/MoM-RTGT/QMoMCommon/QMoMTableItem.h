#ifndef QMOMTABLEITEM_H
#define QMOMTABLEITEM_H

#include <QActionGroup>
#include <QTableWidgetItem>

#include "QMoMResources.h"
#include "QMoMSharedPointers.h"

///////////////////////////////////////////

enum eShowNumber
{
    SHOWNUMBER_normal,          // -1    0    1
    SHOWNUMBER_alwaysPlus,      // -1   +0   +1
    SHOWNUMBER_positivePlus,    // -1    0   +1
    SHOWNUMBER_noZero,          // -1         1
    SHOWNUMBER_plusAndNoZero,   // -1        +1
    SHOWNUMBER_hex,             // FF   00   01
    SHOWNUMBER_halfMove         // -0.5 0.0 0.5
};

enum eShowEnum
{
    SHOWENUM_normal,
    SHOWENUM_minusOne,
    SHOWENUM_noZero,
    SHOWENUM_minusOneAndnoZero
};

class QMoMTableItemBase : public QTableWidgetItem
{
public:
    QMoMTableItemBase(const QMoMGamePtr& game, const QString& text = "");
    virtual QList<QAction*> requestActions(QObject* parent);
    virtual void slotAction();
    virtual QString toString() const;

    static QString formatNumber(int number, eShowNumber showNumber, int width = 0);
protected:
    QMoMGamePtr m_game;
};

///////////////////////////////////////////

template<typename Number>
class NumberTableItem : public QMoMTableItemBase
{
public:
    // g++ complains that it cannot bind an uint16_t to a reference in this template, probably due to some alignment issue,
    // but it does accept a pointer. :).
    NumberTableItem(const QMoMGamePtr& game, Number* t, int width, eShowNumber showNumber = SHOWNUMBER_normal);
    virtual void setData(int role, const QVariant &value);
    virtual QString toString() const;

private:
    Number* m_ptr;
    int m_width;
    eShowNumber m_showNumber;
};


template<typename Number>
NumberTableItem<Number>::NumberTableItem(const QMoMGamePtr& game, Number* t, int width, eShowNumber showNumber) :
    QMoMTableItemBase(game),
    m_ptr(t),
    m_width(width),
    m_showNumber(showNumber)
{
    setFlags(flags() | Qt::ItemIsEditable);
    QTableWidgetItem::setData(Qt::EditRole, toString());
}

template<typename Number>
void NumberTableItem<Number>::setData(int role, const QVariant &value)
{
    switch (role)
    {
    case Qt::EditRole:
        {
            Number newValue = static_cast<Number> (value.toInt());
            if (m_showNumber == SHOWNUMBER_halfMove)
            {
                newValue = static_cast<Number> (2 * value.toDouble());
            }
            (void)m_game->commitData(m_ptr, &newValue, sizeof(Number));
            QTableWidgetItem::setData(role, toString());
        }
        break;
    default:
        QTableWidgetItem::setData(role, value);
        break;
    }
}

template<typename Number>
QString NumberTableItem<Number>::toString() const
{
    return formatNumber((int)*m_ptr, m_showNumber, m_width);
}

///////////////////////////////////////////

class TextTableItem : public QMoMTableItemBase
{
public:
    TextTableItem(const QMoMGamePtr& game, char* text, size_t size);
    virtual void setData(int role, const QVariant &value);
    virtual QString toString() const;
private:
    char* m_ptr;
    size_t m_size;
};


inline TextTableItem::TextTableItem(const QMoMGamePtr& game, char* text, size_t size) :
    QMoMTableItemBase(game),
    m_ptr(text),
    m_size(size)
{
    setFlags(flags() | Qt::ItemIsEditable);
    QTableWidgetItem::setData(Qt::EditRole, toString());
}

inline void TextTableItem::setData(int role, const QVariant &value)
{
    switch (role)
    {
    case Qt::EditRole:
        {
            std::string newValue(m_size, '\0');
            strncpy(&newValue[0], value.toByteArray().data(), m_size - 1);
            (void)m_game->commitData(m_ptr, newValue.data(), m_size);
            QTableWidgetItem::setData(role, toString());
        }
        break;
    default:
        QTableWidgetItem::setData(role, value);
        break;
    }
}

inline QString TextTableItem::toString() const
{
    std::string str(m_ptr, m_size);
    return QString(str.c_str());
}

/////////////////////////////////////////

template<typename Enum>
class EnumTableItemBase : public QMoMTableItemBase
{
public:
    EnumTableItemBase(const QMoMGamePtr& game, Enum* e, eShowEnum showEnum = SHOWENUM_normal, unsigned bitmask = 0);

    virtual void setData(int role, const QVariant &value);
    virtual QList<QAction*> requestActions(QObject* parent) = 0;
    virtual void slotAction();
    virtual QString toString() const;

protected:
    void addAction(Enum e);

    eShowEnum getShowEnum() const
    {
        return m_showEnum;
    }

    Enum getValue() const
    {
        return *m_ptr;
    }

    QList<QAction*> getActions()
    {
        assert(0 != m_actionGroup);
        return m_actionGroup->actions();
    }

    void createActionGroup(QObject* parent)
    {
        // Ownership is transfered to the parent, who will delete it.
        m_actionGroup = new QActionGroup(parent);
    }

private:

    // Configuration
    Enum* m_ptr;
    unsigned m_bitmask;
    eShowEnum m_showEnum;

    // Status

    // Keep track of the action group
    // m_actionGroup is deleted by its parent (the context menu)
    QActionGroup* m_actionGroup;
};

template<typename Enum>
EnumTableItemBase<Enum>::EnumTableItemBase(const QMoMGamePtr& game, Enum* e, eShowEnum showEnum, unsigned bitmask) :
    QMoMTableItemBase(game),
    m_ptr(e),
    m_bitmask(bitmask),
    m_showEnum(showEnum),
    m_actionGroup()
{
    QTableWidgetItem::setData(Qt::EditRole, toString());
}

template<typename Enum>
void EnumTableItemBase<Enum>::setData(int role, const QVariant &value)
{
    switch (role)
    {
    case Qt::EditRole:
        {
            unsigned unsignedValue = value.toUInt();
            if (0 != m_bitmask)
            {
                unsignedValue = ((*m_ptr & ~m_bitmask) | (unsignedValue & m_bitmask));
            }
            Enum newValue = static_cast<Enum>(unsignedValue);
            (void)m_game->commitData(m_ptr, &newValue, sizeof(Enum));
            QTableWidgetItem::setData(role, toString());
        }
        break;
    default:
        QTableWidgetItem::setData(role, value);
        break;
    }
}

template<typename Enum>
void EnumTableItemBase<Enum>::addAction(Enum e)
{
    assert(m_actionGroup != 0);

    auto rawName = prettyQStr(e);
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
void EnumTableItemBase<Enum>::slotAction()
{
    QAction* action = m_actionGroup->checkedAction();
    setData(Qt::EditRole, action->data());
}

template<typename Enum>
QString EnumTableItemBase<Enum>::toString() const
{
    unsigned unsignedValue = static_cast<unsigned>(*m_ptr);
    if (0 != m_bitmask)
    {
        unsignedValue = (unsignedValue & m_bitmask);
    }
    Enum enumValue = static_cast<Enum>(unsignedValue);
    QString str = MoM::QMoMResources::instance().getName(enumValue);

    if (((m_showEnum == SHOWENUM_minusOne) || (m_showEnum == SHOWENUM_minusOneAndnoZero))
            && (enumValue == (Enum)-1))
    {
        str = "";
    }
    else if (((m_showEnum == SHOWENUM_noZero) || (m_showEnum == SHOWENUM_minusOneAndnoZero))
            && (enumValue == (Enum)0))
    {
        str = "-";
    }
    return str;
}

/////////////////////////////////////////

template<typename Enum>
class EnumTableItem : public EnumTableItemBase<Enum>
{
public:
    EnumTableItem(const QMoMGamePtr& game, Enum* e, Enum max, eShowEnum showEnum = SHOWENUM_normal, unsigned bitmask = 0);
    EnumTableItem(const QMoMGamePtr& game, Enum* e, Enum min, Enum max, eShowEnum showEnum = SHOWENUM_normal);

    virtual QList<QAction*> requestActions(QObject* parent);

private:
    // Configuration
    Enum m_min;
    Enum m_max;

    // Status
};

template<typename Enum>
EnumTableItem<Enum>::EnumTableItem(const QMoMGamePtr& game, Enum* e, Enum max, eShowEnum showEnum, unsigned bitmask) :
    EnumTableItemBase<Enum>(game, e, showEnum, bitmask),
    m_min(),
    m_max(max)
{
    QTableWidgetItem::setData(Qt::EditRole, this->toString());
}

template<typename Enum>
EnumTableItem<Enum>::EnumTableItem(const QMoMGamePtr& game, Enum* e, Enum min, Enum max, eShowEnum showEnum) :
    EnumTableItemBase<Enum>(game, e, showEnum, 0),
    m_min(min),
    m_max(max)
{
    QTableWidgetItem::setData(Qt::EditRole, this->toString());
}

template<typename Enum>
QList<QAction*> EnumTableItem<Enum>::requestActions(QObject* parent)
{
    this->createActionGroup(parent);

    if (((this->getShowEnum() == SHOWENUM_minusOne) || (this->getShowEnum() == SHOWENUM_minusOneAndnoZero)) && ((int16_t)m_min >= 0))
    {
        this->addAction((Enum)-1);
    }

    for (int16_t index = (int16_t)m_min; index < (int16_t)m_max; ++index)
    {
        Enum e = (Enum)index;
        this->addAction(e);
    }

    return this->getActions();
}

/////////////////////////////////////////

template<typename Enum>
class EnumTableItemList : public EnumTableItemBase<Enum>
{
public:
    EnumTableItemList(const QMoMGamePtr& game, Enum* e, const QList<Enum>& listEnums);

    virtual QList<QAction*> requestActions(QObject* parent);

private:
    // Configuration
    QList<Enum> m_listEnums;

    // Status
};

template<typename Enum>
EnumTableItemList<Enum>::EnumTableItemList(const QMoMGamePtr& game, Enum* e, const QList<Enum>& listEnums) :
    EnumTableItemBase<Enum>(game, e, SHOWENUM_normal, 0),
    m_listEnums(listEnums)
{
    QTableWidgetItem::setData(Qt::EditRole, this->toString());
}

template<typename Enum>
QList<QAction*> EnumTableItemList<Enum>::requestActions(QObject* parent)
{
    this->createActionGroup(parent);

    foreach (Enum e, m_listEnums)
    {
        this->addAction(e);
    }

    return this->getActions();
}

/////////////////////////////////////////

template<typename Bitmask, typename Enum>
class BitmaskTableItem : public QMoMTableItemBase
{
public:
    // g++ complains that it cannot bind an uint16_t to a reference in this template, probably due to some alignment issue,
    // but it does accept a pointer. :).
    BitmaskTableItem(const QMoMGamePtr& game, Bitmask* bitmask, Enum min, Enum max, unsigned maskbits = 0);

    virtual void setData(int role, const QVariant &value);

    virtual QList<QAction*> requestActions(QObject* parent);

    virtual void slotAction();

private:
    void addAction(Enum e);
    bool has(Enum e) const;
    QString toString();

    // Configuration
    Bitmask* m_ptr;
    Enum m_min;
    Enum m_max;
    unsigned m_maskbits;

    // Status

    // Keep track of the action group
    // m_actionGroup is deleted by its parent (the context menu)
    QActionGroup* m_actionGroup;
};

template<typename Bitmask, typename Enum>
BitmaskTableItem<Bitmask, Enum>::BitmaskTableItem(const QMoMGamePtr& game, Bitmask* bitmask, Enum min, Enum max, unsigned maskbits) :
    QMoMTableItemBase(game),
    m_ptr(bitmask),
    m_min(min),
    m_max(max),
    m_maskbits(maskbits),
    m_actionGroup()
{
    QTableWidgetItem::setData(Qt::EditRole, toString());
}

template<typename Bitmask, typename Enum>
void BitmaskTableItem<Bitmask, Enum>::setData(int role, const QVariant &value)
{
    switch (role)
    {
    case Qt::EditRole:
        {
            unsigned unsignedValue = value.toUInt();
            if (0 != m_maskbits)
            {
                unsignedValue = ((*m_ptr & ~m_maskbits) | (unsignedValue & m_maskbits));
            }
            Bitmask newValue = static_cast<Bitmask>(unsignedValue);
            (void)m_game->commitData(m_ptr, &newValue, sizeof(Bitmask));
            QTableWidgetItem::setData(role, toString());
        }
        break;
    default:
        QTableWidgetItem::setData(role, value);
        break;
    }
}

template<typename Bitmask, typename Enum>
void BitmaskTableItem<Bitmask, Enum>::addAction(Enum e)
{
    assert(m_actionGroup != 0);

    auto rawName = prettyQStr(e);
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
QList<QAction*> BitmaskTableItem<Bitmask, Enum>::requestActions(QObject* parent)
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
void BitmaskTableItem<Bitmask, Enum>::slotAction()
{
    Bitmask bitmask = 0;
    Enum e = m_min;
    for (int i = 0; i < m_actionGroup->actions().count(); ++i, MoM::inc(e))
    {
        if (m_actionGroup->actions().at(i)->isChecked())
        {
            Enum e = static_cast<Enum>(m_actionGroup->actions().at(i)->data().toUInt());
            bitmask |= (1 << ((unsigned)e - (unsigned)m_min));
        }
    }
    setData(Qt::EditRole, QVariant((unsigned)bitmask));
}

template<typename Bitmask, typename Enum>
bool BitmaskTableItem<Bitmask, Enum>::has(Enum e) const
{
    Bitmask mask = (1 << ((unsigned)e - (unsigned)m_min));
    if (0 != m_maskbits)
    {
        mask = (mask & m_maskbits);
    }
    return ((*m_ptr & mask) != 0);
}

template<typename Bitmask, typename Enum>
QString BitmaskTableItem<Bitmask, Enum>::toString()
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

//////////////////////////////////////

#endif // QMOMTABLEITEM_H
