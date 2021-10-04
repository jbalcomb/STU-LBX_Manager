#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include <QSettings>
#include <QSlider>
#include <QSplitter>
#include <QTableWidget>
#include <QTreeWidget>
#include <QWidget>

#include <iostream>

#include "QMoMSettings.h"

// TODO:
// - Save/restore column widths of tree view and table view
// - Set font??

void QMoMSettings::initialize(const QString &applicationName)
{
    // Settings for the configuration
    QCoreApplication::setOrganizationName("MoMRTGT");
    QCoreApplication::setOrganizationDomain("sourceforge.net/projects/momrtgt/");
    QCoreApplication::setApplicationName(applicationName);
}

void QMoMSettings::readSettingsWindow(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    QVariant value = settings.value("pos");
    if (!value.isNull())
    {
        window->move(settings.value("pos").toPoint());
        window->resize(settings.value("size").toSize());
        recurseRead(settings, window);
    }
    settings.endGroup();
}

void QMoMSettings::writeSettingsWindow(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    settings.setValue("pos", window->pos());
    settings.setValue("size", window->size());
    recurseWrite(settings, window);
    settings.endGroup();
}

void QMoMSettings::readSettingsControl(QWidget *control)
{
    QSettings settings;

    settings.beginGroup(control->objectName());
    recurseRead(settings, control);
    settings.endGroup();
}

void QMoMSettings::writeSettingsControl(QWidget *control)
{
    QSettings settings;

    settings.beginGroup(control->objectName());
    recurseWrite(settings, control);
    settings.endGroup();
}

void QMoMSettings::recurseRead(QSettings& settings, QObject* object)
{
    QVariant value;
    if (!object->objectName().isEmpty())
    {
        value = settings.value(object->objectName());
    }

    QCheckBox* checkbox = dynamic_cast<QCheckBox*>(object);
    QComboBox* combobox = dynamic_cast<QComboBox*>(object);
    QFileDialog* filedialog = dynamic_cast<QFileDialog*>(object);
    QMainWindow* mainWindow = dynamic_cast<QMainWindow*>(object);
    QSlider* slider = dynamic_cast<QSlider*>(object);
    QSplitter* splitter = dynamic_cast<QSplitter*>(object);
    QTableWidget* tablewidget = dynamic_cast<QTableWidget*>(object);
    QTreeWidget* treewidget = dynamic_cast<QTreeWidget*>(object);

    if (value.isValid())
    {
        if (0 != checkbox)
        {
            checkbox->setChecked(value.toBool());
        }
        if (0 != combobox)
        {
            combobox->setCurrentIndex(value.toInt());
        }
        if (0 != filedialog)
        {
            // qDebug() << "QFileDialog" << filedialog->directory().absolutePath();
            filedialog->setDirectory(value.toString());
        }
        if (0 != slider)
        {
            slider->setValue(value.toInt());
        }
        if (0 != splitter)
        {
            splitter->restoreState(value.toByteArray());
        }
        if (0 != tablewidget)
        {
            QStringList columns = value.toString().split(",");
            for (int i = 0; (i < tablewidget->columnCount()) && (i < columns.count()); ++i)
            {
                tablewidget->setColumnWidth(i, columns.at(i).toInt());
            }
        }
        if (0 != treewidget)
        {
            QStringList columns = value.toString().split(",");
            for (int i = 0; (i < treewidget->columnCount()) && (i < columns.count()); ++i)
            {
                treewidget->setColumnWidth(i, columns.at(i).toInt());
            }
        }
    }

    if (0 != mainWindow)
    {
        mainWindow->restoreGeometry(settings.value("geometry").toByteArray());
        mainWindow->restoreState(settings.value("windowState").toByteArray());
    }

    bool recurse = ((0 == filedialog) && (0 == tablewidget) && (0 == treewidget));
    if (recurse)
    {
        foreach(QObject* child, object->children())
        {
            recurseRead(settings, child);
        }
    }
}

void QMoMSettings::recurseWrite(QSettings& settings, QObject* object)
{
    QCheckBox* checkbox = dynamic_cast<QCheckBox*>(object);
    if (0 != checkbox)
    {
        settings.setValue(checkbox->objectName(), checkbox->isChecked());
    }
    QComboBox* combobox = dynamic_cast<QComboBox*>(object);
    if (0 != combobox)
    {
        settings.setValue(combobox->objectName(), combobox->currentIndex());
    }
    QFileDialog* filedialog = dynamic_cast<QFileDialog*>(object);
    if (0 != filedialog)
    {
        // qDebug() << "QFileDialog" << filedialog->directory().absolutePath();
        settings.setValue(filedialog->objectName(), filedialog->directory().absolutePath());
        // Do not recurse
        return;
    }
    QMainWindow* mainWindow = dynamic_cast<QMainWindow*>(object);
    if (0 != mainWindow)
    {
        settings.setValue("geometry", mainWindow->saveGeometry());
        settings.setValue("windowState", mainWindow->saveState());
    }
    QSlider* slider = dynamic_cast<QSlider*>(object);
    if (0 != slider)
    {
        settings.setValue(slider->objectName(), slider->value());
    }
    QSplitter* splitter = dynamic_cast<QSplitter*>(object);
    if (0 != splitter)
    {
        settings.setValue(splitter->objectName(), splitter->saveState());
    }
    QTableWidget* tablewidget = dynamic_cast<QTableWidget*>(object);
    if (0 != tablewidget)
    {
        QStringList columns;
        for (int i = 0; i < tablewidget->columnCount(); ++i)
        {
            columns << QString("%0").arg(tablewidget->columnWidth(i));
        }
        settings.setValue(tablewidget->objectName(), columns.join(","));
        // Do not recurse
        return;
    }
    QTreeWidget* treewidget = dynamic_cast<QTreeWidget*>(object);
    if (0 != treewidget)
    {
        QStringList columns;
        for (int i = 0; i < treewidget->columnCount(); ++i)
        {
            columns << QString("%0").arg(treewidget->columnWidth(i));
        }
        settings.setValue(treewidget->objectName(), columns.join(","));
        // Do not recurse
        return;
    }

    foreach(QObject* child, object->children())
    {
        recurseWrite(settings, child);
    }
}
