// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include "MainWindow.h"
#include "QMoMSettings.h"
#include "TeeRedirecter.h"

#include <QApplication>

#include <cassert>

static_assert(__cplusplus >= 201103L, "c++11 required");

int main(int argc, char *argv[])
{
    const char* output_filename = "cout_cerr.log";
    std::cout << "Teeing output to " << output_filename << std::endl;
    TeeRedirecter redirecter(output_filename, true);
    std::cout << "__cplusplus=" << __cplusplus << std::endl;

    // Run application
    QApplication a(argc, argv);

    // Settings for the configuration
    QMoMSettings::initialize("MoMTweaker");

    MainWindow w;
    w.show();
    return a.exec();
}
