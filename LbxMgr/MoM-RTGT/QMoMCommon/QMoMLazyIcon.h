#ifndef QMOMLAZYICON_H
#define QMOMLAZYICON_H

#include "QMoMResources.h"
#include "QMoMSharedPointers.h"

namespace MoM
{

class QMoMLazyIconBase
{
public:
    explicit QMoMLazyIconBase()
    {
    }
    virtual ~QMoMLazyIconBase()
    {
    }
    virtual const QIcon data() const = 0;
    virtual bool resolve() = 0;
};

template<class T>
class QMoMLazyIcon : public QMoMLazyIconBase
{
public:
    explicit QMoMLazyIcon(const T& t, int scale = 1) :
        QMoMLazyIconBase(),
        m_ref(t),
        m_scale(scale),
        m_icon(),
        m_resolved(false)
    {
    }
    const QIcon data() const
    {
        if (m_icon.isNull())
            return QIcon();
        else
            return *m_icon;
    }
    bool resolve()
    {
        bool resolution = false;
        if (!m_resolved)
        {
            m_icon = QMoMResources::instance().getIcon(m_ref, m_scale);
            m_resolved = true;
            resolution = (!m_icon.isNull() && !m_icon->isNull());
        }
        return resolution;
    }
    void setData(const T& t)
    {
        if (m_ref != t)
        {
            m_ref = t;
            m_resolved = false;
            m_icon.clear();
        }
    }

private:
    T m_ref;
    int m_scale;
    QMoMIconPtr m_icon;
    bool m_resolved;
};

template<>
class QMoMLazyIcon<QString> : public QMoMLazyIconBase
{
public:
    explicit QMoMLazyIcon(const QString& ref = QString()) :
        QMoMLazyIconBase(),
        m_ref(ref),
        m_icon(),
        m_resolved(false)
    {
    }
    const QIcon data() const
    {
        if (m_icon.isNull())
            return QIcon();
        else
            return *m_icon;
    }
    bool resolve()
    {
        bool resolution = false;
        if (!m_resolved)
        {
            m_icon = QMoMIconPtr(new QIcon(m_ref));
            m_resolved = true;
            resolution = (!m_icon.isNull() && !m_icon->isNull());
        }
        return resolution;
    }
    void setData(const QString& ref)
    {
        if (m_ref != ref)
        {
            m_ref = ref;
            m_resolved = false;
            m_icon.clear();
        }
    }
private:
    QString m_ref;
    QMoMIconPtr m_icon;
    bool m_resolved;
};

template<>
class QMoMLazyIcon<QIcon> : public QMoMLazyIconBase
{
public:
    explicit QMoMLazyIcon(const QIcon& ref) :
        QMoMLazyIconBase(),
        m_ref(ref),
        m_icon(),
        m_resolved(false)
    {
    }
    const QIcon data() const
    {
        if (m_icon.isNull())
            return QIcon();
        else
            return *m_icon;
    }
    bool resolve()
    {
        bool resolution = false;
        if (!m_resolved)
        {
            m_icon = QMoMIconPtr(new QIcon(m_ref));
            m_resolved = true;
            resolution = (!m_icon.isNull() && !m_icon->isNull());
        }
        return resolution;
    }
    void setData(const QIcon& ref)
    {
        m_ref = ref;
        m_resolved = false;
        m_icon.clear();
    }
private:
    QIcon m_ref;
    QMoMIconPtr m_icon;
    bool m_resolved;
};

}

#endif // QMOMLAZYICON_H
