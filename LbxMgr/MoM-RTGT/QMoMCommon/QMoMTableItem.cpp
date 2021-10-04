#include "QMoMTableItem.h"

///////////////////////////////////////////

QMoMTableItemBase::QMoMTableItemBase(const QMoMGamePtr& game, const QString& text) :
    QTableWidgetItem(text),
    m_game(game)
{
    // Not editable by default
    setFlags(flags() & ~Qt::ItemIsEditable);
    // Centered by default
    setTextAlignment(Qt::AlignCenter);
}

QList<QAction*> QMoMTableItemBase::requestActions(QObject*)
{
    return QList<QAction*>();
}

void  QMoMTableItemBase::slotAction()
{
}

QString  QMoMTableItemBase::toString() const
{
    return QTableWidgetItem::text();
}

QString QMoMTableItemBase::formatNumber(int number, eShowNumber showNumber, int width)
{
    QString result = QString("%0").arg(number);
    switch (showNumber)
    {
    case SHOWNUMBER_normal:
        // Nothing further to do
        break;
    case SHOWNUMBER_alwaysPlus:
        if (number >= 0)
        {
            result = "+" + result;
        }
        break;
    case SHOWNUMBER_positivePlus:
        if (number > 0)
        {
            result = "+" + result;
        }
        break;
    case SHOWNUMBER_noZero:
        if (number == 0)
        {
            result = "";
        }
        break;
    case SHOWNUMBER_plusAndNoZero:
        if (number == 0)
        {
            result = "";
        }
        else if (number > 0)
        {
            result = "+" + result;
        }
        break;
    case SHOWNUMBER_hex:
        result = QString("0x%0").arg((unsigned)number, width, 16, QChar('0'));
        break;
    case SHOWNUMBER_halfMove:
        if (number == 0)
        {
            result = "";
        }
        else
        {
            result = QString("%0").arg((double)number / 2, width, 'f', 1);
        }
        break;
    default:
        assert(0 && "Unknown value if eShowNumber");
    }

    result = QString("%0").arg(result, width);

    return result;
}

///////////////////////////////////////////

