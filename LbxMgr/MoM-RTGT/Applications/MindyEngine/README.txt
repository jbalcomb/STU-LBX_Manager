This is an engine for external AI for MoM.
It contains an example that shows Mindy's python engine.

FILES
-----
README.txt              This file.
mom.py                  The python interface to MoM.
_mom.pyd                The C++ wrapper that mom.py uses to access MoM.
example_momai.py        Example AI engine.
example_momgame.py      Example showing how the MoMGameBase object can be used.
example_momhook.py      Example showing how the MoMHookManager object can be used.
example_momprocess.py   Example showing how the MoMProcess object can be used.

INSTALL
-------
To run it you need Python, which you can download here:
    http://www.python.org/download/releases/
I've used version 3.2.3 (17 MB) to build and test.

Copy the files _mom.pyd, mom.py, and example*.py to the directory
    C:\python32\Libs\site-packages

RUN
---
1. Start MoM and preferably enter combat.
2. Start the Python GUI
3. Select File/Open Module from the menu, type "example_momai", and click OK.
   The example should open in an editor window
4. Select Run/Run Module from the menu in the new editor window.
   The example will first show a couple of examples.
   Afterward it will run the engine.
5. Whenever you are in combat, the python rules will take over AI!
6. To abort press Ctrl+C. The engine will stop within a second.

Alternatively, any of the other examples can be loaded and run.
