#include "MoMController.h"

#include "MoMTerrain.h"

namespace MoM
{

int MoMTerrain::sDoubleFoodBonus[eTerrainCategory_MAX] =
{
    0,  // TERRAINCATEGORY_Ocean = 0,
    3,  // TERRAINCATEGORY_Grasslands = 1,
    1,  // TERRAINCATEGORY_Forest = 2,
    0,  // TERRAINCATEGORY_Mountain = 3,
    0,  // TERRAINCATEGORY_Desert = 4,
    0,  // TERRAINCATEGORY_Swamp = 5,
    0,  // TERRAINCATEGORY_Tundra = 6,
    1,  // TERRAINCATEGORY_Shore = 7,
    4,  // TERRAINCATEGORY_RiverMouth = 8,
    1,  // TERRAINCATEGORY_Hills = 9,
    1,  // TERRAINCATEGORY_Plains = 10,
    4,  // TERRAINCATEGORY_River = 11,
    0,  // TERRAINCATEGORY_Volcano = 12,
    4,  // TERRAINCATEGORY_SorceryNode = 13,
    5,  // TERRAINCATEGORY_NatureNode = 14,
    0,  // TERRAINCATEGORY_ChaosNode = 15,
    4,  // TERRAINCATEGORY_Lake = 16,
};

int MoMTerrain::sDoubleHammerBonus[eTerrainCategory_MAX] =
{
    0,  // TERRAINCATEGORY_Ocean = 0,
    0,  // TERRAINCATEGORY_Grasslands = 1,
    3,  // TERRAINCATEGORY_Forest = 2,
    5,  // TERRAINCATEGORY_Mountain = 3,
    3,  // TERRAINCATEGORY_Desert = 4,      // TODO: Different types of Desert???
    0,  // TERRAINCATEGORY_Swamp = 5,
    0,  // TERRAINCATEGORY_Tundra = 6,
    0,  // TERRAINCATEGORY_Shore = 7,
    0,  // TERRAINCATEGORY_RiverMouth = 8,
    3,  // TERRAINCATEGORY_Hills = 9,
    0,  // TERRAINCATEGORY_Plains = 10,
    0,  // TERRAINCATEGORY_River = 11,
    0,  // TERRAINCATEGORY_Volcano = 12,
    0,  // TERRAINCATEGORY_SorceryNode = 13,
    0,  // TERRAINCATEGORY_NatureNode = 14,
    5,  // TERRAINCATEGORY_ChaosNode = 15,
    0,  // TERRAINCATEGORY_Lake = 16,
};

eTerrainCategory MoMTerrain::getTerrainCategory(eTerrainType terrainType)
{
    // TODO: River mouth = river adjacent to shore???

    // From code for easy reference ;)
    eTerrainCategory value = eTerrainCategory_MAX;
    if (terrainType >= 0x25A)
    {
        value = TERRAINCATEGORY_Tundra;
    }
    else if (terrainType >= 0x259)
    {
        value = TERRAINCATEGORY_Ocean;
    }
    else if (terrainType > 0x1D8)
    {
        value = TERRAINCATEGORY_Shore;
    }
    else if (terrainType > 0x1D3)
    {
        value = TERRAINCATEGORY_River;
    }
    else if (terrainType > 0x1C3)
    {
        value = TERRAINCATEGORY_Shore;
    }
    else if (terrainType > 0x123)
    {
        value = TERRAINCATEGORY_Desert;
    }
    else if (terrainType > 0x112)
    {
        value = TERRAINCATEGORY_Hills;
    }
    else if (terrainType > 0x102)
    {
        value = TERRAINCATEGORY_Mountain;
    }
    else if (terrainType > 0xE8)
    {
        value = TERRAINCATEGORY_River;
    }
    else if (terrainType > 0xC4)
    {
        value = TERRAINCATEGORY_Lake;
    }
    else if (terrainType > 0xB8)
    {
        value = TERRAINCATEGORY_River;
    }
    else if (terrainType < 0xA2)
    {
        if (terrainType == 0x01)
        {
            value = TERRAINCATEGORY_Grasslands;
        }
        else
        {
            value = TERRAINCATEGORY_Shore;
        }
    }
    else
    {
        switch (terrainType)
        {
        case 0xA2: // 162
        case 0xAC: // 172
        case 0xAD: // 173
        case 0xB4: // 180
            value = TERRAINCATEGORY_Grasslands; break;

        case 0xA3: // 163
        case 0xB7: // 183
        case 0xB8: // 184
            value = TERRAINCATEGORY_Forest; break;

        case 0xA4: // 164
            value = TERRAINCATEGORY_Mountain; break;

        case 0xA5: // 165
            value = TERRAINCATEGORY_Desert; break;
        case 0xAE: // 174
        case 0xAF: // 175
        case 0xB0: // 176
            value = TERRAINCATEGORY_Desert; break;

        case 0xA6: // 166
        case 0xB1: // 177
        case 0xB2: // 178
            value = TERRAINCATEGORY_Swamp; break;

        case 0xA7: // 167
        case 0xB5: // 181
        case 0xB6: // 182
            value = TERRAINCATEGORY_Tundra; break;

        case 0xA8: // 168
            value = TERRAINCATEGORY_SorceryNode; break;

        case 0xA9: // 169
            value = TERRAINCATEGORY_NatureNode; break;

        case 0xAA: // 170
            value = TERRAINCATEGORY_ChaosNode; break;

        case 0xAB: // 171
            value = TERRAINCATEGORY_Hills; break;

        case 0xB3: // 179
            value = TERRAINCATEGORY_Volcano; break;

        default:
            assert(false && "Bad eTerrainCategory");
        }
    }
    /*
    eTerrainCategory value = (eTerrainCategory)-1;
    switch (terrainType)
    {
    case ocean1:
    case ocean2:        value = TERRAINCATEGORY_Ocean; break;

    case grasslands0:
    case grasslands1:
    case grasslands2:
    case grasslands3:
    case grasslands4:   value = TERRAINCATEGORY_Grasslands; break;

    case forest1:
    case forest2:
    case forest3:       value = TERRAINCATEGORY_Forest; break;

    case mountain1:     value = TERRAINCATEGORY_Mountain; break;

    case desert1:
    case desert2:
    case desert3:
    case desert4:       value = TERRAINCATEGORY_Desert; break;

    case swamp1:
    case swamp2:
    case swamp3:        value = TERRAINCATEGORY_Swamp; break;

    case tundra1:
    case tundra2:
    case tundra3:       value = TERRAINCATEGORY_Tundra; break;

    case hills1:        value = TERRAINCATEGORY_Hills; break;

    case volcano:       value = TERRAINCATEGORY_Volcano; break;

    case grasslands_w_sorcery_node: value = TERRAINCATEGORY_SorceryNode; break;

    case forest_w_nature_node:      value = TERRAINCATEGORY_NatureNode; break;

    case volcano_w_chaos_node:      value = TERRAINCATEGORY_ChaosNode; break;

    default:            // Handled later
                        break;
    }

    if (value != (eTerrainCategory)-1)
    {
        // Done
    }
    else if ((terrainType >= shore1_first) && (terrainType <= shore1_last))
    {
        value = TERRAINCATEGORY_Shore;
    }
    else if ((terrainType >= river1a_first) && (terrainType <= river1a_last))
    {
        value = TERRAINCATEGORY_River;
    }
    else if ((terrainType >= lake1_first) && (terrainType <= lake1_last))
    {
        value = TERRAINCATEGORY_Lake;
    }
    else if ((terrainType >= river2a_first) && (terrainType <= river2a_last))
    {
        value = TERRAINCATEGORY_River;
    }
    else if ((terrainType >= mountain2_first) && (terrainType <= mountain2_last))
    {
        value = TERRAINCATEGORY_Mountain;
    }
    else if ((terrainType >= hills2_first) && (terrainType <= hills2_last))
    {
        value = TERRAINCATEGORY_Hills;
    }
    else if ((terrainType >= desert5_first) && (terrainType <= desert5_last))
    {
        value = TERRAINCATEGORY_Desert;
    }
    else if ((terrainType >= shore4_first) && (terrainType <= shore4_last))
    {
        value = TERRAINCATEGORY_Shore;
    }
    else if ((terrainType >= river3_first) && (terrainType <= river3_last))
    {
        value = TERRAINCATEGORY_River;
    }
    else if ((terrainType >= shore5_first) && (terrainType <= shore5_last))
    {
        value = TERRAINCATEGORY_Shore;
    }
    else if ((terrainType >= tundra4_first) && (terrainType <= tundra4_last))
    {
        value = TERRAINCATEGORY_Tundra;
    }
    else
    {
        // Not found
    }
*/
    return value;
}

MoMTerrain::MoMTerrain(MoMGameBase* game, const MoMLocation& loc) :
    m_game(game),
    m_location(loc)
{
}

void MoMTerrain::setLocation(const MoMLocation& loc)
{
    m_location = loc;
}

Battle_Unit *MoMTerrain::getBattleUnit() const
{
    if (m_location.m_Map != MoMLocation::MAP_battle)
        return 0;
    Battle_Unit* value = 0;
    Battle_Unit* firstInactiveUnit = 0;
    for (int battleUnitNr = 0; battleUnitNr < m_game->getNrBattleUnits(); ++battleUnitNr)
    {
        Battle_Unit* battleUnit = m_game->getBattleUnit(battleUnitNr);
        if (0 == battleUnit)
            break;
        if ((battleUnit->m_xPos == m_location.m_XPos) && (battleUnit->m_yPos == m_location.m_YPos))
        {
            if (battleUnit->m_Active == BATTLEUNITACTIVE_alive)
            {
                value = battleUnit;
                break;
            }
            else if (0 == firstInactiveUnit)
            {
                firstInactiveUnit = battleUnit;
            }
            else
            {
            }
        }
    }
    if (0 == value)
    {
        value = firstInactiveUnit;
    }
    return value;
}

eTerrainBonusDeposit MoMTerrain::getBonus() const
{
    eTerrainBonusDeposit value = (eTerrainBonusDeposit)-1;
    if (0 == m_game)
        return value;
    eTerrainBonusDeposit* ptr = m_game->getTerrainBonus(m_location);
    if (0 != ptr)
    {
        value = *ptr;
    }
    return value;
}

eTerrainCategory MoMTerrain::getCategory() const
{
    return getTerrainCategory(getType());
}

Terrain_Changes MoMTerrain::getChanges() const
{
    Terrain_Changes value = Terrain_Changes();
    if (0 == m_game)
        return value;
    Terrain_Changes* ptr = m_game->getTerrainChange(m_location);
    if (0 != ptr)
    {
        value = *ptr;
    }
    return value;
}

City *MoMTerrain::getCity() const
{
    City* value = 0;
    for (int cityNr = 0; cityNr < m_game->getNrCities(); ++cityNr)
    {
        City* city = m_game->getCity(cityNr);
        if (0 == city)
            break;
        if ((city->m_Plane == m_location.m_Plane)
                && (city->m_XPos == m_location.m_XPos)
                && (city->m_YPos == m_location.m_YPos))
        {
            value = city;
            break;
        }
    }
    return value;
}

uint8_t MoMTerrain::getExplored() const
{
    uint8_t value = 0;
    if (0 == m_game)
        return value;
    uint8_t* ptr = m_game->getTerrainExplored(m_location);
    if (0 != ptr)
    {
        value = *ptr;
    }
    return value;
}

int MoMTerrain::getBasicFoodBonus() const
{
    if (getChanges().corruption)
        return 0;
    eTerrainCategory category = getCategory();
    int value = 0;
    if (toUInt(category) < eTerrainCategory_MAX)
    {
        // TODO: Check if sorcery node and nature node give bonus without breaking it
        // TODO: Check if lair/tower give bonus
        value = sDoubleFoodBonus[category];
    }

    value *= 2;     // Effectively 4 times the actual value

    if (isSharedBetweenCities())
    {
        value /= 2;
    }

    return value;
}

int MoMTerrain::getGoldBonus(bool minersGuild, bool dwarven) const
{
    if (getChanges().corruption)
        return 0;

    eTerrainBonusDeposit bonus = getBonus();
    int value = 0;
    switch (bonus & 0x0F)
    {
    case DEPOSIT_Silver_Ore:    value = 2; break;
    case DEPOSIT_Gold_Ore:      value = 3; break;
    case DEPOSIT_Gems:          value = 5; break;
    default:                    ;
    }

    if (dwarven)
    {
        value *= 2;
    }
    if (minersGuild)
    {
        value = value * 3 / 2;
    }

    if (isSharedBetweenCities())
    {
        value /= 2;
    }

    return value;
}

Tower_Node_Lair *MoMTerrain::getLair() const
{
    Tower_Node_Lair* value = 0;
    for (int lairNr = 0; toUInt(lairNr) < gMAX_NODES_LAIRS_TOWERS; ++lairNr)
    {
        Tower_Node_Lair* lair = m_game->getLair(lairNr);
        if (0 == lair)
            break;
        if ((lair->m_Plane == m_location.m_Plane)
                && (lair->m_XPos == m_location.m_XPos)
                && (lair->m_YPos == m_location.m_YPos))
        {
            value = lair;
            break;
        }
    }
    return value;
}

int MoMTerrain::getProductionPercentage(bool gaiasBlessing) const
{
    if (getChanges().corruption)
        return 0;

    // TODO: Terrain AE-B0,124-1C3 (desert) give 1 1/2 hammers???
    //       While A5 (desert) and B3 (volcano) do NOT give hammers???
    eTerrainCategory category = getCategory();
    int value = 0;
    if (toUInt(category) < eTerrainCategory_MAX)
    {
        // TODO: Check if sorcery node and nature node give bonus without breaking it
        // TODO: Check if lair/tower give bonus
        value = sDoubleHammerBonus[category];
        if ((category == TERRAINCATEGORY_Forest) || (category == TERRAINCATEGORY_NatureNode))
        {
            if (gaiasBlessing)
            {
                value *= 2;
            }
        }
    }

    if (isSharedBetweenCities())
    {
        value /= 2;
    }

    return value;
}

eTerrainType MoMTerrain::getType() const
{
    eTerrainType value = (eTerrainType)-1;
    if (0 == m_game)
        return value;
    eTerrainType* ptr = m_game->getTerrainType(m_location);
    if (0 != ptr)
    {
        value = *ptr;
    }
    return value;
}

std::vector<int> MoMTerrain::getUnits() const
{
    std::vector<int> units;
    if (0 != m_game)
    {
        m_game->findUnitsAtLocation(m_location, units);
    }
    return units;
}

bool MoMTerrain::isRiver() const
{
    eTerrainCategory category = getCategory();
    return (category == TERRAINCATEGORY_River) || (category == TERRAINCATEGORY_RiverMouth);
}

bool MoMTerrain::isSea() const
{
    eTerrainCategory category = getCategory();
    return (category == TERRAINCATEGORY_Ocean) || (category == TERRAINCATEGORY_Shore) || (category == TERRAINCATEGORY_Lake);
}

bool MoMTerrain::isSharedBetweenCities() const
{
    int count = 0;
    for (int cityNr = 0; cityNr < m_game->getNrCities(); ++cityNr)
    {
        const City* city = m_game->getCity(cityNr);
        if (0 == city)
            break;
        if ((city->m_Plane == m_location.m_Plane)
                && (Abs(city->m_XPos - m_location.m_XPos) + Abs(city->m_YPos - m_location.m_YPos) <= 3)
                && (Max(Abs(city->m_XPos - m_location.m_XPos), Abs(city->m_YPos - m_location.m_YPos)) <= 2)
                )
        {
            count++;
            if (count >= 2)
            {
                return true;
            }
        }
    }
    return false;
}


}
