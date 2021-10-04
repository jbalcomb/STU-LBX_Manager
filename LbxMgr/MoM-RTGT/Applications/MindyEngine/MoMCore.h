#ifndef MOMCORE_H
#define MOMCORE_H

namespace MoM {
    class MoMGameBase;
}

/// Keeps track of connected game
extern MoM::MoMGameBase* gGame;

MoM::MoMGameBase* connectToMoM();

#endif // MOMCORE_H
