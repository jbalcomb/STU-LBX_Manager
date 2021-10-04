## ---------------------------------------------------------------------------
## Copyright:   2011 Klaas van Aarsen
## License:     GPL
## Author:      I like Serena (aka Klaas van Aarsen)
## Created:     2010-05-01
## ---------------------------------------------------------------------------

include(Common.pri)

TEMPLATE = app

VERSION = 0.3.1

TARGET = MoMCustomizer

SOURCES += \
    Applications/MoMCustomizer/main.cpp

DISTFILES += \
    Applications/MoMCustomizer/CHANGES.txt

## ---------------------------------------------------------------------------

INCLUDEPATH += \
    Applications/MoMCustomizer \
    MoMControl \
    MoMModel \
    MoMModel/Generated \
    Platform \
    QMoMCommon \
    QMoMCommon/Generated \
    QMoMDialogs \
    Utility \
    giflib-5.0.4/lib

HEADERS += Applications/MoMCustomizer/MainWindow.h \
    MoMModel/Generated/MoMGenerated.h \
    MoMModel/MoMExeMagic.h \
    MoMModel/MoMGameCustom.h \
    MoMModel/MoMLbxBase.h \
    MoMModel/MoMTemplate.h \
    Platform/MoMCommon.h \
    Platform/MoMExeBase.h \
    Platform/MoMProcess.h \
    QMoMCommon/QMoMAnimation.h \
    QMoMCommon/QMoMGifHandler.h \
    QMoMCommon/QMoMLbx.h \
    QMoMCommon/QMoMResources.h \
    Utility/MoMUtility.h \
    Utility/TeeRedirecter.h \
    giflib-5.0.4/lib/gif_stdint.h \
    giflib-5.0.4/lib/gif_unistd.h \
    giflib-5.0.4/lib/gif_lib_private.h \
    giflib-5.0.4/lib/gif_hash.h \
    giflib-5.0.4/lib/gif_lib.h \
    Applications/MoMCustomizer/FormSelectWizard.h \
    Applications/MoMCustomizer/FormGameOptions.h \
    Applications/MoMCustomizer/DialogWizard.h \
    Applications/MoMCustomizer/FormWizardsName.h \
    Applications/MoMCustomizer/FormMagicPickScreen.h \
    Applications/MoMCustomizer/FormSelectSpells.h \
    Applications/MoMCustomizer/FormSelectRaces.h \
    Applications/MoMCustomizer/FormSelectBanner.h

SOURCES += \
    Applications/MoMCustomizer/MainWindow.cpp \
    MoMModel/Generated/MoMGenerated.cpp \
    MoMModel/MoMExeMagic.cpp \
    MoMModel/MoMGameBase.cpp \
    MoMModel/MoMGameCustom.cpp \
    MoMModel/MoMLbxBase.cpp \
    Platform/MoMExeBase.cpp \
    Platform/MoMProcessCommon.cpp \
    Platform/MoMProcess$${PLATF}.cpp \
    QMoMCommon/QMoMAnimation.cpp \
    QMoMCommon/QMoMGifHandler.cpp \
    QMoMCommon/QMoMLbx.cpp \
    QMoMCommon/QMoMResources.cpp \
    Utility/MoMUtility.cpp \
    giflib-5.0.4/lib/quantize.cpp \
    giflib-5.0.4/lib/gifalloc.cpp \
    giflib-5.0.4/lib/gif_hash.cpp \
    giflib-5.0.4/lib/gif_font.cpp \
    giflib-5.0.4/lib/gif_err.cpp \
    giflib-5.0.4/lib/egif_lib.cpp \
    giflib-5.0.4/lib/dgif_lib.cpp \
    Applications/MoMCustomizer/FormSelectWizard.cpp \
    Applications/MoMCustomizer/FormGameOptions.cpp \
    Applications/MoMCustomizer/DialogWizard.cpp \
    Applications/MoMCustomizer/FormWizardsName.cpp \
    Applications/MoMCustomizer/FormMagicPickScreen.cpp \
    Applications/MoMCustomizer/FormSelectSpells.cpp \
    Applications/MoMCustomizer/FormSelectRaces.cpp \
    Applications/MoMCustomizer/FormSelectBanner.cpp

FORMS += Applications/MoMCustomizer/MainWindow.ui \
    Applications/MoMCustomizer/FormSelectWizard.ui \
    Applications/MoMCustomizer/FormGameOptions.ui \
    Applications/MoMCustomizer/DialogWizard.ui \
    Applications/MoMCustomizer/FormWizardsName.ui \
    Applications/MoMCustomizer/FormMagicPickScreen.ui \
    Applications/MoMCustomizer/FormSelectSpells.ui \
    Applications/MoMCustomizer/FormSelectRaces.ui \
    Applications/MoMCustomizer/FormSelectBanner.ui

RESOURCES += Applications/MoMCustomizer/Resources/QMoMCustomizer.qrc \
    Resources/InteractiveGameTweaker.qrc
