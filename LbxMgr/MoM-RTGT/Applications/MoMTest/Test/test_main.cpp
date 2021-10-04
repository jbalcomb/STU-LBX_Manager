#include <QtCore/QString>
#include <QtTest/QtTest>

#include "TeeRedirecter.h"

int runTests_MoMCombat(int argc, char *argv[]);
int runTests_MoMGameBase(int argc, char *argv[]);
int runTests_Test_MoMTweaker(int argc, char *argv[]);


int main(int argc, char *argv[])
{
    const char* output_filename = "cout_cerr.log";
    std::cout << "Teeing output to " << output_filename << std::endl;
    TeeRedirecter redirecter(output_filename, true);

    int exitCode = 0;
    if (0 == exitCode) exitCode = runTests_MoMCombat(argc, argv);
    if (0 == exitCode) exitCode = runTests_MoMGameBase(argc, argv);
    if (0 == exitCode) exitCode = runTests_Test_MoMTweaker(argc, argv);
    return exitCode;
}
