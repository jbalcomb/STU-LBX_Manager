#include <QtCore/QString>
#include <QtTest/QtTest>

#include <MainWindow.h>
#include <ui_MainWindow.h>

class Test_MoMTweaker_MainWindow : public QObject
{
    Q_OBJECT
    
public:
    Test_MoMTweaker_MainWindow();
    
private Q_SLOTS:
    void testCase_Nominal();
    void testCase_Failures();
    void testCase_Connect_MagicExe();
    void testCase_Connect_WizardsExe();
};

Test_MoMTweaker_MainWindow::Test_MoMTweaker_MainWindow()
{
}

void Test_MoMTweaker_MainWindow::testCase_Nominal()
{
    MainWindow w;
    w.show();

    QTestEventList eventsConnect;
    eventsConnect.addMouseClick(Qt::LeftButton, 0, QPoint(), 1000);
    // Start thread to wait for QMessageBox and to close that window
//    eventsConnect.simulate(w.ui->pushButton_Connect);
    // Wait (with a timeout) for QMessageBox to appear and then close it
    // If timeout
        // Close windows and emit failure
    // If successfully closed QMessageBox
        // Check if messages match
    QCOMPARE(w.statusBar()->currentMessage(), QString("Game connection failed"));
}

void Test_MoMTweaker_MainWindow::testCase_Failures()
{
    QVERIFY2(true, "Failure");
}

void Test_MoMTweaker_MainWindow::testCase_Connect_MagicExe()
{
    MainWindow w;
    w.show();

    // Start DosBox with MAGIC.EXE

    QTestEventList eventsConnect;
    eventsConnect.addMouseClick(Qt::LeftButton, 0, QPoint(), 1000);
//    eventsConnect.simulate(w.ui->pushButton_Connect);
    QCOMPARE(w.statusBar()->currentMessage(), QString("Game connected"));

    // Close DosBox
}

void Test_MoMTweaker_MainWindow::testCase_Connect_WizardsExe()
{
    MainWindow w;
    w.show();

    // Start DosBox with WIZARDS.EXE

    QTestEventList eventsConnect;
    eventsConnect.addMouseClick(Qt::LeftButton, 0, QPoint(), 1000);
//    eventsConnect.simulate(w.ui->pushButton_Connect);
    QCOMPARE(w.statusBar()->currentMessage(), QString("Game connected"));

    // Close DosBox
}

int runTests_Test_MoMTweaker(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTEST_DISABLE_KEYPAD_NAVIGATION
    Test_MoMTweaker_MainWindow tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_momtweaker_mainwindow.moc"
