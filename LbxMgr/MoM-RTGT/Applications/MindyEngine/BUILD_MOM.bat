REM "c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat"
c:\swigwin-2.0.8\swig.exe -python -c++ -I../MoMCore -I../MoMEditorTemplate mom.i
REM cl /LD /EHsc /o _mom.pyd /I c:\Python32\include /I..\MoMCore /I..\MoMEditorTemplate mom_wrap.cxx c:\Python32\libs\python32.lib ..\MoMCore\msvc2008\bin\MoMCore.lib
REM c:\Python32\python.exe example_mom.py