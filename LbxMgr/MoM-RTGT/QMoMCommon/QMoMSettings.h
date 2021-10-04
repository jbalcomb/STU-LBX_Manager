#ifndef QMOMSETTINGS_H
#define QMOMSETTINGS_H

class QMoMSettings
{
public:
    /// \brief  Initializes the QSettings configuration
    /// \pre    The QApplication objects must have been created to
    ///         make sure the right directory is used.
    static void initialize(const QString& applicationName);

    /// \brief  Read and execute the common settings for a window
    ///         The objectName() of the window is used as key.
    static void readSettingsWindow(QWidget* window);

    /// \brief  Write the common settings for a window
    ///         The objectName() of the window is used as key.
    static void writeSettingsWindow(QWidget* window);

    /// \brief  Read and execute the common settings for a specific control
    ///         The objectName() of the control is used as key.
    ///         Note that it can be changed with setObjectName().
    static void readSettingsControl(QWidget* control);

    /// \brief  Write the common settings for a specific control
    ///         The objectName() of the control is used as key.
    static void writeSettingsControl(QWidget* control);

private:
    static void recurseRead(class QSettings& settings, QObject* object);
    static void recurseWrite(class QSettings& settings, QObject* object);
};

#endif // QMOMSETTINGS_H
