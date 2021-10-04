// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef MOMGAMEBASE_H
#define MOMGAMEBASE_H

#include <memory>

#include "MoMLbxBase.h"     // TODO: Why does forward not work?
#include "MoMLocation.h"
#include "MoMTemplate.h"
#include "MoMUtility.h"

namespace MoM {

class MoMGameBase
{
public:
    struct SaveGameRef { uint8_t* pointer = 0; size_t size = 0; };

    MoMGameBase() {}
    virtual ~MoMGameBase();

    virtual void closeGame() throw() = 0;

    //! Contains the last error that applies whenever a method returning
    //! a bool failed.
    virtual std::string errorString() const
    {
        return m_errorString;
    }

    virtual bool load(const char*)
    {
        setErrorString("not implemented");
        return false;
    }

    virtual bool save(const char*)
    {
        setErrorString("not implemented");
        return false;
    }

    //! Commit the data identified by ptr
    bool commitData(void* ptr, size_t size)
    {
        return commitData(ptr, ptr, size);
    }

    //! Commit new data at pNewValue at the location identified by ptr
    //! The data at ptr is only updated if the commit was successful
    virtual bool commitData(void* ptr, const void* pNewValue, size_t size) = 0;

    void findUnitsAtLocation(const MoMLocation& location, std::vector<int>& units);

    Battle_Unit* getBattleUnit(int battleUnitNr)
    {
        Battle_Unit* battleUnits = getBattle_Units();
        if ((0 == battleUnits) || !inRange(battleUnitNr, getNrBattleUnits()) || !inRange(battleUnitNr, gMAX_BATTLE_UNITS))
            return 0;
        return &battleUnits[battleUnitNr];
    }

    Building_Data* getBuildingData(eBuilding building) const
    {
        Building_Data* buildingData = getBuildingData();
        if ((0 == buildingData) || !inRange(building, eBuilding_array_MAX))
            return 0;
        return &buildingData[building];
    }

    City* getCity(int cityNr) const
    {
        City* cities = getCities();
        if ((0 == cities) || !inRange(cityNr, gMAX_CITIES))
            return 0;
        return &cities[cityNr];
    }

    virtual std::string getGameDirectory() const = 0;

    Hero_stats* getHeroStats(ePlayer playerNr, eUnit_Type heroNr)
    {
        Hero_stats* listHeroStats = getList_Hero_stats(playerNr);
        if ((0 == listHeroStats) || !inRange(heroNr, gMAX_HERO_TYPES))
            return 0;
        return &listHeroStats[heroNr];
    }
    Hero_Stats_Initializer* getHeroStatsInitializer(eUnit_Type heroNr)
    {
        Hero_Stats_Initializer* listHeroStatsInitializer = getList_Hero_Stats_Initializer();
        if ((0 == listHeroStatsInitializer) || !inRange(heroNr, gMAX_HERO_TYPES))
            return 0;
        return &listHeroStatsInitializer[heroNr];
    }

    void getHeroSlotTypes(eHero_TypeCode heroTypeCode, eSlot_Type16 heroSlotTypes[3]);
    eHero_TypeCode getHeroTypeCode(MoM::eSlot_Type16 slotSword, MoM::eSlot_Type16 slotShield, MoM::eSlot_Type16 slotRing);

    Hired_Hero* getHiredHero(ePlayer playerNr, int slotNr)
    {
        Wizard* wizard = getWizard(playerNr);
        if ((0 == wizard) || !inRange(slotNr, gMAX_HIRED_HEROES))
            return 0;
        return &wizard->m_Heroes_hired_by_wizard[slotNr];
    }
    Hired_Hero* getHiredHero(ePlayer playerNr, eUnit_Type unitType);
    Hired_Hero* getHiredHero(const Unit* unit)
    {
        if (0 == unit)
            return 0;
        Wizard* wizard = getWizard(unit->m_Owner);
        if ((0 == wizard) || !inRange(unit->m_Hero_Slot_Number, gMAX_HIRED_HEROES))
            return 0;
        return &wizard->m_Heroes_hired_by_wizard[unit->m_Hero_Slot_Number];
    }

    ItemDataLbx* getItemDataLbx(int itemDataNr);

    ItemPowLbx* getItemPowLbx(int itemPowNr);

    Item* getItem(int itemNr)
    {
        Item* items = getItems();
        if ((0 == items) || !inRange(itemNr, gMAX_ITEMS_VALID))
            return 0;
        return &items[itemNr];
    }

    Battle_Unit* getLevelBonusHero(int level)
    {
        uint8_t* data = getLevelBonusesHero();
        if ((0 == data) || !inRange(level, gMAX_LEVELS_HERO))
            return 0;
        return (Battle_Unit*)&data[level * 0x16];
    }

    Battle_Unit* getLevelBonusUnit(int level)
    {
        uint8_t* data = getLevelBonusesUnit();
        if ((0 == data) || !inRange(level, gMAX_LEVELS_UNIT))
            return 0;
        return (Battle_Unit*)&data[level * 0x16];
    }

    Tower_Node_Lair* getLair(int lairNr)
    {
        Tower_Node_Lair* lairs = getLairs();
        if ((0 == lairs) || !inRange(lairNr, gMAX_NODES_LAIRS_TOWERS))
            return 0;
        return &lairs[lairNr];
    }

    virtual char* getMoMVersion()
    {
        char* copyrightAndVersion = 0;
        if (0 != getDataSegment())
        {
            copyrightAndVersion = getDataSegment()->m_Copyright_and_Version;
        }
        else if (0 != this->getMagicDataSegment())
        {
            copyrightAndVersion = getMagicDataSegment()->m_Copyright1_and_Version;
        }

        char* version_v = 0;
        char* version_V = 0;
        if (0 != copyrightAndVersion)
        {
            version_v = strstr(copyrightAndVersion, " v");
            version_V = strstr(copyrightAndVersion, " V");
        }

        if ((0 != version_v) && (strlen(version_v) > 3) && (version_v[3] == '.'))
        {
            return (version_v + 2);
        }
        else if ((0 != version_V) && (strlen(version_V) > 3) && (version_V[3] == '.'))
        {
            return (version_V + 2);
        }
        else
        {
            static char nullVersion[8] = "";
            return nullVersion;
        }
    }

    virtual const char* getNameByOffset(DS_Offset)
    {
        return 0;
    }

    virtual Node_Attr* getNodeAttr(int nodeNr)
    {
        Node_Attr* nodeAttr = getNodeAttributes();
        if ((0 == nodeAttr) || !inRange(nodeNr, gMAX_NODES))
            return 0;
        return &nodeAttr[nodeNr];
    }

    int getNrBattleUnits()
    {
        if (0 == getNumber_of_Battle_Units())
            return 0;
        return *getNumber_of_Battle_Units();
    }
    bool setNrBattleUnits(int value)
    {
        if (0 == getNumber_of_Battle_Units())
            return false;
        uint16_t newValue = (uint16_t)value;
        return commitData(getNumber_of_Battle_Units(), &newValue, sizeof(*getNumber_of_Battle_Units()));
    }
    int getNrCities()
    {
        if (0 == getNumber_of_Cities())
            return 0;
        return *getNumber_of_Cities();
    }
    bool setNrCities(int value)
    {
        if (0 == getNumber_of_Cities())
            return false;
        uint16_t newValue = (uint16_t)value;
        return commitData(getNumber_of_Cities(), &newValue, sizeof(*getNumber_of_Cities()));
    }
    int getNrItemDataLbx();
    int getNrItemPowLbx();
    int getNrItemsInGame()
    {
        Item* items = getItems();
        if (0 == items)
            return 0;
        int nrItems = 0;
        for (size_t itemNr = 0; itemNr < MoM::gMAX_ITEMS_IN_GAME; ++itemNr)
        {
            if ((0 != items[itemNr].m_Cost) && (-1 != items[itemNr].m_Cost))
            {
                nrItems++;
            }
        }
        return nrItems;
    }
    int getNrItemsValid()
    {
        Item* items = getItems();
        if (0 == items)
            return 0;
        int nrItems = 0;
        for (size_t itemNr = 0; itemNr < MoM::gMAX_ITEMS_VALID; ++itemNr)
        {
            if (0 != items[itemNr].m_Cost)
            {
                nrItems++;
            }
        }
        return nrItems;
    }
    int getNrUnits()
    {
        if (0 == getNumber_of_Units())
            return 0;
        return *getNumber_of_Units();
    }
    bool setNrUnits(int value)
    {
        if (0 == getNumber_of_Units())
            return false;
        uint16_t newValue = (uint16_t)value;
        return commitData(getNumber_of_Units(), &newValue, sizeof(*getNumber_of_Units()));
    }
    int getNrWizards()
    {
        if (0 == getNumber_of_Wizards())
            return 0;
        return *getNumber_of_Wizards();
    }
    bool setNrWizards(int value)
    {
        if (0 == getNumber_of_Wizards())
            return false;
        uint16_t newValue = (uint16_t)value;
        return commitData(getNumber_of_Wizards(), &newValue, sizeof(*getNumber_of_Wizards()));
    }

    ePlayer getPlayerNr(const Hero_stats* heroStats)
    {
        ePlayer value = (ePlayer)-1;
        for (ePlayer playerNr = (ePlayer)0; toUInt(playerNr) < gMAX_VALID_WIZARDS; inc(playerNr))
        {
            Hero_stats* firstHeroStats = getList_Hero_stats(playerNr);
            unsigned uUnitTypeNr = toUInt(heroStats - firstHeroStats);
            if (uUnitTypeNr < gMAX_HERO_TYPES)
            {
                value = playerNr;
                break;
            }
        }
        return value;
    }
    ePlayer getPlayerNr(const Hired_Hero* hiredHero)
    {
        ePlayer value = (ePlayer)-1;
        for (ePlayer playerNr = (ePlayer)0; toUInt(playerNr) < gMAX_VALID_WIZARDS; inc(playerNr))
        {
            Wizard* wizard = getWizard(playerNr);
            if (0 == wizard)
                continue;
            Hired_Hero* firstHiredHero = wizard->m_Heroes_hired_by_wizard;
            if (toUInt(hiredHero - firstHiredHero) < gMAX_HIRED_HEROES)
            {
                value = playerNr;
                break;
            }
        }
        return value;
    }

    Race_Data* getRaceData(eRace race)
    {
		MoMDataSegment* dataSegment = getDataSegment();
        if ((0 == dataSegment) || !inRange(race, gMAX_RACES))
            return 0;
		return &dataSegment->m_Race_Data[race];
    }

    std::string getRaceName(eRace race);

    eRealm_Type getRealmRace(eRace race) const;
    eRealm_Type getRealmRangedType(eRanged_Type rangedType) const;
    eRealm_Type getRealmSpecialAttack(eUnitAbility specialAttack) const;

    virtual std::string getSources() const = 0;

    Spell_Data* getSpellData(eSpell spell)
    {
        Spell_Data* spellData = getSpellData();
        if ((0 == spellData) || !inRange(spell, MoM::eSpell_MAX))
            return 0;
        return &spellData[spell];
    }

    eTerrainBonusDeposit* getTerrainBonus(const MoMLocation& loc)
    {
        return getTerrainBonus(loc.m_Plane, loc.m_XPos, loc.m_YPos);
    }
    eTerrainBonusDeposit* getTerrainBonus(ePlane plane, int x, int y)
    {
        eTerrainBonusDeposit* data = getTerrain_Bonuses();
		if ((0 == data) || !inRange(plane, MoM::ePlane_MAX) || !inRange(x, MoM::gMAX_MAP_COLS) || !inRange(y, MoM::gMAX_MAP_ROWS))
            return 0;
        return (data + (static_cast<int>(plane) * gMAX_MAP_ROWS + y) * gMAX_MAP_COLS + x);
    }
    Terrain_Changes* getTerrainChange(const MoMLocation& loc)
    {
        return getTerrainChange(loc.m_Plane, loc.m_XPos, loc.m_YPos);
    }
    Terrain_Changes* getTerrainChange(ePlane plane, int x, int y)
    {
        Terrain_Changes* data = getTerrain_Changes();
		if ((0 == data) || !inRange(plane, MoM::ePlane_MAX) || !inRange(x, MoM::gMAX_MAP_COLS) || !inRange(y, MoM::gMAX_MAP_ROWS))
            return 0;
        return (data + (static_cast<int>(plane) * gMAX_MAP_ROWS + y) * gMAX_MAP_COLS + x);
    }
    uint8_t* getTerrainExplored(const MoMLocation& loc)
    {
        return getTerrainExplored(loc.m_Plane, loc.m_XPos, loc.m_YPos);
    }
    uint8_t* getTerrainExplored(ePlane plane, int x, int y)
    {
        uint8_t* data = getTerrain_Explored();
		if ((0 == data) || !inRange(plane, MoM::ePlane_MAX) || !inRange(x, MoM::gMAX_MAP_COLS) || !inRange(y, MoM::gMAX_MAP_ROWS))
            return 0;
        return (data + (static_cast<int>(plane) * gMAX_MAP_ROWS + y) * gMAX_MAP_COLS + x);
    }
    int8_t* getTerrainLandMassID(const MoMLocation& loc)
    {
        return getTerrainLandMassID(loc.m_Plane, loc.m_XPos, loc.m_YPos);
    }
    int8_t* getTerrainLandMassID(ePlane plane, int x, int y)
    {
        int8_t* data = getTerrain_LandMassID();
		if ((0 == data) || !inRange(plane, MoM::ePlane_MAX) || !inRange(x, MoM::gMAX_MAP_COLS) || !inRange(y, MoM::gMAX_MAP_ROWS))
            return 0;
        return (data + (static_cast<int>(plane) * gMAX_MAP_ROWS + y) * gMAX_MAP_COLS + x);
    }
    int8_t* getTerrainMovement(const MoMLocation& loc, eMovement movement)
    {
        return getTerrainMovement(loc.m_Plane, loc.m_XPos, loc.m_YPos, movement);
    }
    int8_t* getTerrainMovement(ePlane plane, int x, int y, eMovement movement)
    {
        Map_Movement* data = getTerrain_Movements();
		if ((0 == data) || !inRange(plane, MoM::ePlane_MAX) || !inRange(x, MoM::gMAX_MAP_COLS) || !inRange(y, MoM::gMAX_MAP_ROWS) || !inRange(movement, MoM::eMovement_MAX))
            return 0;
        MapRow_Movement* mapRow = &data[plane].m_Unused_Row[0];
        mapRow += static_cast<unsigned>(movement) * gMAX_MAP_ROWS;
        return &mapRow[y].m_Moves[x];
    }
    eTerrainType* getTerrainType(const MoMLocation& loc)
    {
        return getTerrainType(loc.m_Plane, loc.m_XPos, loc.m_YPos);
    }
    eTerrainType* getTerrainType(ePlane plane, int x, int y)
    {
        eTerrainType* data = getTerrain_Types();
		if ((0 == data) || !inRange(plane, MoM::ePlane_MAX) || !inRange(x, MoM::gMAX_MAP_COLS) || !inRange(y, MoM::gMAX_MAP_ROWS))
            return 0;
        return (data + (static_cast<int>(plane) * gMAX_MAP_ROWS + y) * gMAX_MAP_COLS + x);
    }

    Unit* getUnit(int unitNr)
    {
        Unit* units = getUnits();
		if ((0 == units) || !inRange(unitNr, MoM::gMAX_UNITS))
            return 0;
        return &units[unitNr];
    }
    Unit_Type_Data* getUnitTypeData(eUnit_Type unitTypeNr)
    {
        Unit_Type_Data* unitTypes = getUnit_Types();
        if ((0 == unitTypes) || !inRange(unitTypeNr, MoM::eUnit_Type_MAX))
            return 0;
        return &unitTypes[unitTypeNr];
    }
    eUnit_Type getUnitTypeNr(const Hero_stats* heroStats)
    {
        eUnit_Type unitTypeNr = (eUnit_Type)-1;
        for (ePlayer playerNr = (ePlayer)0; toUInt(playerNr) < gMAX_VALID_WIZARDS; inc(playerNr))
        {
            Hero_stats* firstHeroStats = getList_Hero_stats(playerNr);
            unsigned value = toUInt(heroStats - firstHeroStats);
            if (value < gMAX_HERO_TYPES)
            {
                unitTypeNr = (eUnit_Type)value;
                break;
            }
        }
        return unitTypeNr;
    }
    eUnit_Type getUnitTypeNr(const Hero_Stats_Initializer* heroStatsInitializer)
    {
        eUnit_Type unitTypeNr = (eUnit_Type)-1;
        Hero_Stats_Initializer* firstHeroStats = getList_Hero_Stats_Initializer();
        unsigned value = toUInt(heroStatsInitializer - firstHeroStats);
        if (value < gMAX_HERO_TYPES)
        {
            unitTypeNr = (eUnit_Type)value;
        }
        return unitTypeNr;
    }
    eUnit_Type getUnitTypeNr(const Unit_Type_Data* unitType)
    {
        eUnit_Type unitTypeNr = (eUnit_Type)-1;
        Unit_Type_Data* unitTypes = getUnit_Types();
        if (0 != unitTypes)
        {
            unitTypeNr = (eUnit_Type)(unitType - unitTypes);
        }
        return unitTypeNr;
    }
    Wizard* getWizard(int wizardNr)
    {
        Wizard* wizards = getWizards();
		if ((0 == wizards) || !inRange(wizardNr, MoM::gMAX_WIZARD_RECORDS))
            return 0;
        return &wizards[wizardNr];
    }

    virtual bool isBattleInProgress() const
    {
        return false;
    }

    virtual bool isOpen() const = 0;

    virtual bool readData()
    {
        return true;
    }

//
// TODO: Make all methods below protected/private
//

public:
    virtual Available_spell_page* getAvailable_spell_pages()
    {
        return 0;
    }
    virtual uint8_t* getArtifacts_in_game()
    {
        return 0;
    }
public:
    virtual Battlefield* getBattlefield()
    {
        return 0;
    }
    virtual Battle_Unit* getBattleUnitViewed()
    {
        return 0;
    }
protected:
    virtual Battle_Unit* getBattle_Units()
    {
        return 0;
    }
    virtual Building_Data* getBuildingData() const
    {
        return 0;
    }
public:
    virtual Hero_Choice* getChosen_Hero_Names()
    {
        return 0;
    }
protected:
    virtual City* getCities() const = 0;
public:
    virtual Events_Status* getEvents_Status()
    {
        return 0;
    }
    virtual Fortress* getFortresses()
    {
        return 0;
    }
    virtual WizardsExe_Game_Data* getGameData_WizardsExe()
    {
        return 0;
    }
    virtual Game_Data_Save* getGameData_SaveGame()
    {
        return 0;
    }
    virtual Game_Settings* getGameSettings()
    {
        return 0;
    }
    virtual eGameState* getGameState()
    {
        return 0;
    }
    virtual uint16_t* getGameTurn()
    {
        return 0;
    }
    virtual eSpell16* getHero_spells()
    {
        return 0;
    }
protected:
    virtual Hero_stats* getList_Hero_stats(ePlayer playerNr) = 0;
    virtual Hero_Stats_Initializer* getList_Hero_Stats_Initializer()
    {
        return 0;
    }
    virtual Item* getItems() = 0;
    virtual Tower_Node_Lair* getLairs() = 0;
    virtual uint8_t* getLevelBonusesHero();
    virtual uint8_t* getLevelBonusesUnit();
    virtual Node_Attr* getNodeAttributes() = 0;
public:
    // Needs to be public so we can add it to a treeview
    virtual uint16_t* getNumber_of_Battle_Units()
    {
        return 0;
    }
protected:
    virtual uint16_t* getNumber_of_Cities() = 0;
    virtual uint16_t* getNumber_of_Units() = 0;
    virtual uint16_t* getNumber_of_Wizards() = 0;
    virtual Spell_Data* getSpellData()
    {
        return 0;
    }
public:
    virtual Spells_Cast_in_Battle* getSpells_Cast_in_Battle()
    {
        return 0;
    }
public:
    virtual uint16_t* getTaxUnrestTable()
    {
        return 0;
    }
protected:
    virtual eTerrainBonusDeposit* getTerrain_Bonuses()
    {
        return 0;
    }
    virtual Terrain_Changes* getTerrain_Changes()
    {
        return 0;
    }
    virtual uint8_t* getTerrain_Explored()
    {
        return 0;
    }
    virtual int8_t* getTerrain_LandMassID()
    {
        return 0;
    }
public:
    virtual Map_Movement* getTerrain_Movements()
    {
        return 0;
    }
    virtual Map_Movement* getTerrain_Movements_copy()
    {
        return 0;
    }
protected:
    virtual eTerrainType* getTerrain_Types()
    {
        return 0;
    }

    virtual Unit* getUnits() = 0;
    virtual Unit_Type_Data* getUnit_Types()
    {
        return 0;
    }
public:
    virtual int8_t* getUnrest_Table(eRace /* homeRace */)
    {
        return 0;
    }
protected:
    virtual Wizard* getWizards() = 0;

public:
    virtual MoMDataSegment* getDataSegment()
    {
        return 0;
    }
    virtual class MoMExeMagic* getMagicExe()
    {
        return 0;
    }
    virtual MoMMagicDataSegment* getMagicDataSegment()
    {
        return 0;
    }
    virtual uint8_t* getMagicExeContents()
    {
        return 0;
    }
    virtual MoMMagicDataSegment* getMagicExeDataSegment()
    {
        return 0;
    }
    virtual uint8_t* getMagicOverlay(size_t /* ovlNr */)
    {
        return 0;
    }
    virtual class MoMProcess* getMoMProcess()
	{
		return 0;
	}
    virtual SaveGameRef getSaveGame()
    {
        return {};
    }
    virtual const uint8_t* getSeg0Pointer()
    {
        return 0;
    }
    virtual class MoMExeWizards* getWizardsExe()
    {
        return 0;
    }
    virtual uint8_t* getWizardsExeContents()
    {
        return 0;
    }
    virtual MoMDataSegment* getWizardsExeDataSegment()
    {
        return 0;
    }
    virtual uint8_t* getWizardsOverlay(size_t /* ovlNr */)
    {
        return 0;
    }

protected:
    virtual void setErrorString(const std::string& value)
    {
        m_errorString = value;
    }

protected:
    std::unique_ptr<MoMLbxBase> m_ItemDataLbx;
    std::unique_ptr<MoMLbxBase> m_ItemPowLbx;

private:
    // NOT IMPLEMENTED
    MoMGameBase(const MoMGameBase& rhs);
    MoMGameBase& operator=(const MoMGameBase& rhs);

    // STATUS
    std::string m_errorString;
};

}

#endif
