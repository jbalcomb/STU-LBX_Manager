#include "MoMCore.h"

#include "MoMGameCustom.h"
#include "MoMGameMemory.h"

MoM::MoMGameBase* gGame = 0;

MoM::MoMGameBase* connectToMoM()
{
    MoM::MoMGameBase* game = 0;

    std::auto_ptr<MoM::MoMProcess> momProcess(new MoM::MoMProcess);
    std::auto_ptr<MoM::MoMGameMemory> memGame(new MoM::MoMGameMemory);
    std::auto_ptr<MoM::MoMGameCustom> customGame(new MoM::MoMGameCustom);

    bool ok = momProcess->findProcessAndData();
    if (ok && memGame->openGame(momProcess))
    {
        game = memGame.release();
    }
    else if (ok && customGame->openGame(momProcess))
    {
        game = customGame.release();
    }
    else
    {
        ok = false;
    }

    // Keep track of the connected game
    gGame = game;

    return game;
}
