## ---------------------------------------------------------------------------
## Copyright:   2011 Klaas van Aarsen
## License:     GPL
## Author:      I like Serena (aka Klaas van Aarsen)
## Created:     2010-05-01
## ---------------------------------------------------------------------------

INCLUDEPATH += \
    Applications/MoMTweaker \
    MoMControl \
    MoMModel \
    MoMModel/Generated \
    Platform \
    QMoMCommon \
    QMoMCommon/Generated \
    QMoMDialogs \
    Utility \
    better-enums-0.11.2 \
    giflib-5.0.4/lib

HEADERS += \
    Applications/MoMTweaker/MainWindow.h \
    MoMControl/MoMCatnip.h \
    MoMControl/MoMController.h \
    MoMControl/MoMHookManager.h \
    MoMModel/Generated/MoMGenerated.h \
    MoMModel/MoMCombat.h \
    MoMModel/MoMExeMagic.h \
    MoMModel/MoMExeWizards.h \
    MoMModel/MoMGameBase.h \
    MoMModel/MoMGameCustom.h \
    MoMModel/MoMGameMemory.h \
    MoMModel/MoMGameSave.h \
    MoMModel/MoMLbxBase.h \
    MoMModel/MoMLocation.h \
    MoMModel/MoMResources.h \
    MoMModel/MoMTemplate.h \
    MoMModel/MoMTerrain.h \
    MoMModel/MoMUnit.h \
    Platform/MoMCommon.h \
    Platform/MoMExeBase.h \
    Platform/MoMProcess.h \
    QMoMCommon/QMapGraphicsView.h \
    QMoMCommon/QMoMLazyIcon.h \
    QMoMCommon/QMoMLbx.h \
    QMoMCommon/QMoMMapScene.h \
    QMoMCommon/QMoMMapTile.h \
    QMoMCommon/QMoMResources.h \
    QMoMCommon/QMoMSettings.h \
    QMoMCommon/QMoMSharedPointers.h \
    QMoMCommon/QMoMUnitTile.h \
    QMoMCommon/QMoMUtility.h \
    QMoMCommon/Generated/QMoMGenerated.h \
    Utility/MoMUtility.h \
    Utility/TeeRedirecter.h \
    QMoMCommon/QMoMTableItem.h \
    QMoMCommon/QMoMTreeItemModel.h \
    QMoMCommon/QMoMTreeItemWidget.h \
    MoMModel/MoMFli.h \
    QMoMCommon/QMoMGifHandler.h \
    QMoMCommon/QMoMAnimation.h \
    QMoMCommon/QMoMAnimationTile.h \
    better-enums-0.11.2/enum.h \
    giflib-5.0.4/lib/gif_stdint.h \
    giflib-5.0.4/lib/gif_unistd.h \
    giflib-5.0.4/lib/gif_lib_private.h \
    giflib-5.0.4/lib/gif_hash.h \
    giflib-5.0.4/lib/gif_lib.h \
    MoMControl/MoMLevelBonuses.h \
    QMoMCommon/UnitModel.h \
    QMoMDialogs/DialogAddUnit.h \
    QMoMDialogs/DialogBuildingTree.h \
    QMoMDialogs/DialogCalculatorAddress.h \
    QMoMDialogs/DialogExternalAI.h \
    QMoMDialogs/DialogLbxEditor.h \
    QMoMDialogs/DialogMap.h \
    QMoMDialogs/DialogSelectInitialSpells.h \
    QMoMDialogs/DialogTables.h \
    QMoMDialogs/DialogTools.h \
    MoMModel/MoMCity.h \
    QMoMDialogs/DialogAddArtifact.h \
    QMoMDialogs/DialogSelectRaces.h \
    QMoMCommon/QMoMDialogBase.h \
    QMoMDialogs/DialogManageCities.h \
    MoMControl/MoMManageCities.h

SOURCES += \
    Applications/MoMTweaker/MainWindow.cpp \
    MoMControl/MoMCatnip.cpp \
    MoMControl/MoMCombat.cpp \
    MoMControl/MoMController.cpp \
    MoMControl/MoMHookManager.cpp \
    MoMModel/Generated/MoMGenerated.cpp \
    MoMModel/MoMExeMagic.cpp \
    MoMModel/MoMExeWizards.cpp \
    MoMModel/MoMFli.cpp \
    MoMModel/MoMGameBase.cpp \
    MoMModel/MoMGameCustom.cpp \
    MoMModel/MoMGameMemory.cpp \
    MoMModel/MoMGameSave.cpp \
    MoMModel/MoMLbxBase.cpp \
    MoMModel/MoMTerrain.cpp \
    MoMModel/MoMUnit.cpp \
    Platform/MoMExeBase.cpp \
    Platform/MoMProcessCommon.cpp \
    Platform/MoMProcess$${PLATF}.cpp \
    QMoMCommon/Generated/QMoMGenerated.cpp \
    QMoMCommon/QMoMAnimation.cpp \
    QMoMCommon/QMoMAnimationTile.cpp \
    QMoMCommon/QMoMGifHandler.cpp \
    QMoMCommon/QMapGraphicsView.cpp \
    QMoMCommon/QMoMLbx.cpp \
    QMoMCommon/QMoMMapScene.cpp \
    QMoMCommon/QMoMMapTile.cpp \
    QMoMCommon/QMoMResources.cpp \
    QMoMCommon/QMoMSettings.cpp \
    QMoMCommon/QMoMTableItem.cpp \
    QMoMCommon/QMoMTreeItemModel.cpp \
    QMoMCommon/QMoMTreeItemWidget.cpp \
    QMoMCommon/QMoMUnitTile.cpp \
    QMoMCommon/QMoMUtility.cpp \
    Utility/MoMUtility.cpp \
    giflib-5.0.4/lib/quantize.cpp \
    giflib-5.0.4/lib/gifalloc.cpp \
    giflib-5.0.4/lib/gif_hash.cpp \
    giflib-5.0.4/lib/gif_font.cpp \
    giflib-5.0.4/lib/gif_err.cpp \
    giflib-5.0.4/lib/egif_lib.cpp \
    giflib-5.0.4/lib/dgif_lib.cpp \
    MoMControl/MoMLevelBonuses.cpp \
    QMoMDialogs/DialogAddUnit.cpp \
    QMoMDialogs/DialogBuildingTree.cpp \
    QMoMDialogs/DialogCalculatorAddress.cpp \
    QMoMDialogs/DialogExternalAI.cpp \
    QMoMDialogs/DialogLbxEditor.cpp \
    QMoMDialogs/DialogMap.cpp \
    QMoMDialogs/DialogSelectInitialSpells.cpp \
    QMoMDialogs/DialogTables.cpp \
    QMoMDialogs/DialogTools.cpp \
    QMoMCommon/UnitModel.cpp \
    MoMModel/MoMCity.cpp \
    QMoMDialogs/DialogAddArtifact.cpp \
    QMoMDialogs/DialogSelectRaces.cpp \
    QMoMCommon/QMoMDialogBase.cpp \
    QMoMDialogs/DialogManageCities.cpp \
    MoMControl/MoMManageCities.cpp

FORMS += \
    Applications/MoMTweaker/MainWindow.ui \
    QMoMDialogs/DialogAddUnit.ui \
    QMoMDialogs/DialogBuildingTree.ui \
    QMoMDialogs/DialogCalculatorAddress.ui \
    QMoMDialogs/DialogExternalAI.ui \
    QMoMDialogs/DialogLbxEditor.ui \
    QMoMDialogs/DialogMap.ui \
    QMoMDialogs/DialogSelectInitialSpells.ui \
    QMoMDialogs/DialogTables.ui \
    QMoMDialogs/DialogTools.ui \
    QMoMDialogs/DialogAddArtifact.ui \
    QMoMDialogs/DialogSelectRaces.ui \
    QMoMDialogs/DialogManageCities.ui

RESOURCES += Resources/InteractiveGameTweaker.qrc
