// ---------------------------------------------------------------------------
// Copyright:   2011-2020 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2011-04-03
// Modified:    2020-03-07
// ---------------------------------------------------------------------------

SUPPORTED PLATFORMS
===================
Windows
Linux

DEPENDENCIES
============
Qt          Needed for GUI components
perl        Needed to regenerate sources


DEPLOYING
=========
In both Windows and Linux you need the Qt dynamic libraries available.

In Windows you can do this by putting Qt5Core.dll, Qt5Gui.dll, and Qt5Widgets.dll in the same 
directory as the executables and any redistributables of the compiler as well.

In Linux you can do this by putting libQtCore.so, libQtGui.so, and libQtWidgets.so in 
the same directory as the executables and adding the current directory to 
the environment variable LD_LIBRARY_PATH.


BUILDING FROM SOURCES
=====================
1. Download QtCreator.
2. Open one of the projects, for instance Applications\MoMTweaker\MoMTweaker.pro.
3. Build & Run.
4. If you make changes to the structures that map to the game, you'll have 
   regenerate some sources:
        perl scripts/generate_code.pl