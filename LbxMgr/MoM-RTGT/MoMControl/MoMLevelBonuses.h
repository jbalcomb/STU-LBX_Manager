#ifndef MOMLEVELBONUSES_H
#define MOMLEVELBONUSES_H

namespace MoM {

class MoMLevelBonuses
{
public:
    MoMLevelBonuses(class MoMGameBase* game) :
        m_game(game)
    {
    }

    bool isCodeInstalled();
    bool installCode();

private:
    class MoMGameBase* m_game;
};

}

#endif // MOMLEVELBONUSES_H
