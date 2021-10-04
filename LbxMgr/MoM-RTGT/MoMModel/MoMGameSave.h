// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#include "MoMGameBase.h"
#include "MoMLbxBase.h"
#include "MoMExeMagic.h"
#include "MoMExeWizards.h"
#include "MoMResources.h"

namespace MoM {

class MoMGameSave : public MoMGameBase
{
public:
    MoMGameSave() : MoMGameBase() {}
    virtual ~MoMGameSave() {}

    virtual bool load(const char* filename);
    virtual bool save(const char* filename);

protected:
    virtual void closeGame() throw() {}

    virtual bool commitData(void* ptr, const void* pNewValue, size_t size);

#define RETURN_SAVEGAME_MEMBER(field) \
    if (m_SaveGame.size() < sizeof(SaveGame)) \
        return 0; \
    if (m_SaveGame.size() < sizeof(SaveGame_CasterOfMagic)) \
        return ((SaveGame*)m_SaveGame.data())->field; \
    else \
       return ((SaveGame_CasterOfMagic*)m_SaveGame.data())->field;
#define RETURN_SAVEGAME_MEMBER_ADDRESS(field) \
    if (m_SaveGame.size() < sizeof(SaveGame)) \
        return 0; \
    if (m_SaveGame.size() < sizeof(SaveGame_CasterOfMagic)) \
        return &((SaveGame*)m_SaveGame.data())->field; \
    else \
       return &((SaveGame_CasterOfMagic*)m_SaveGame.data())->field;

    virtual uint8_t* getArtifacts_in_game()
    {
        RETURN_SAVEGAME_MEMBER(m_Artifacts_in_Game);
    }

    virtual Building_Data* getBuildingData() const
    {
        if (0 == m_BuilddatLbx.get())
            return 0;
        uint8_t* data = m_BuilddatLbx->getRecord(0);
        return (Building_Data*)(data + 4);
    }

    virtual Hero_Choice* getChosen_Hero_Names()
    {
        RETURN_SAVEGAME_MEMBER(m_Chosen_Hero_Names);
    }

    virtual City* getCities() const
    {
        RETURN_SAVEGAME_MEMBER(m_Cities);
    }

    virtual Events_Status* getEvents_Status()
    {
        RETURN_SAVEGAME_MEMBER_ADDRESS(m_Events_Status);
    }

    virtual Fortress* getFortresses()
    {
        RETURN_SAVEGAME_MEMBER_ADDRESS(m_Fortresses[0]);
    }

    virtual std::string getGameDirectory() const;

    virtual Game_Data_Save* getGameData_SaveGame()
    {
        RETURN_SAVEGAME_MEMBER_ADDRESS(m_Game_Data);
    }

    virtual uint16_t* getGameTurn()
    {
        RETURN_SAVEGAME_MEMBER_ADDRESS(m_Game_Data.m_Current_Turn);
    }

    virtual Hero_stats* getList_Hero_stats(ePlayer playerNr)
    {

        RETURN_SAVEGAME_MEMBER(m_List_Hero_stats[playerNr].a);
    }

    virtual Hero_Stats_Initializer* getList_Hero_Stats_Initializer()
    {
        MoMMagicDataSegment* pMagicDataSegment = getMagicDataSegment();
        if (0 == pMagicDataSegment)
            return 0;
        return pMagicDataSegment->m_Hero_Stats_Initializers;
    }

    virtual Item* getItems()
    {
        RETURN_SAVEGAME_MEMBER(m_Items);
    }

    virtual Tower_Node_Lair* getLairs()
    {
        RETURN_SAVEGAME_MEMBER(m_Arcanus_Towers);
    }

    virtual char* getMoMVersion()
    {
        auto baseVersion = MoMGameBase::getMoMVersion();
        if (0 != baseVersion && '\0' != *baseVersion)
            return baseVersion;

        if (sizeof(SaveGame) == m_size_SaveGame)
        {
            static char editableBuf[] = "1.31-1.40n";
            return editableBuf;
        }
        if (sizeof(SaveGame_CasterOfMagic) == m_size_SaveGame)
        {
            static char editableBuf[] = "6.0?";
            return editableBuf;
        }
        if (m_size_SaveGame < sizeof(SaveGame_CasterOfMagic))
        {
            static char editableBuf[] = "1.50+ (file has in between size)";
            return editableBuf;
        }
        if (m_size_SaveGame > sizeof(SaveGame_CasterOfMagic))
        {
            static char editableBuf[] = "6.0 (bigger file)";
            return editableBuf;
        }
        return 0;
    }

    virtual const char* getNameByOffset(DS_Offset offset)
    {
        char* namebuffer = (char*)getDataSegment();
        if (0 == namebuffer)
            return 0;
        // TODO: Check range and terminating zero
        return namebuffer + offset;
    }

    virtual Node_Attr* getNodeAttributes()
    {
        RETURN_SAVEGAME_MEMBER_ADDRESS(m_Arcanus_Node_Attr[0]);
    }

    virtual uint16_t* getNumber_of_Cities()
    {
        RETURN_SAVEGAME_MEMBER_ADDRESS(m_Game_Data.m_Number_of_Cities);
    }

    virtual uint16_t* getNumber_of_Units()
    {
        RETURN_SAVEGAME_MEMBER_ADDRESS(m_Game_Data.m_Number_of_Units);
    }

    virtual uint16_t* getNumber_of_Wizards()
    {
        RETURN_SAVEGAME_MEMBER_ADDRESS(m_Game_Data.m_Number_of_Wizards);
    }

    virtual std::string getSources() const;

    virtual Spell_Data* getSpellData()
    {
        if (0 == m_SpelldatLbx.get())
            return 0;
        return getLbxSpellData(*m_SpelldatLbx);
    }

    virtual uint16_t* getTaxUnrestTable()
    {
        MoMDataSegment* pMoMDataSegment = getDataSegment();
        if (0 == pMoMDataSegment)
            return 0;
        return pMoMDataSegment->m_Tax_Unrest_Table;
    }

    virtual eTerrainBonusDeposit* getTerrain_Bonuses()
    {
        RETURN_SAVEGAME_MEMBER_ADDRESS(m_Arcanus_Bonus_Row[0].m_Bonus_Deposit[0]);
    }
    virtual Terrain_Changes* getTerrain_Changes()
    {
        RETURN_SAVEGAME_MEMBER_ADDRESS(m_Arcanus_Terrain_Changes_Row[0].m_Terrain_Changes[0]);
    }
    virtual uint8_t* getTerrain_Explored()
    {
        RETURN_SAVEGAME_MEMBER_ADDRESS(m_Arcanus_Exploration_Row[0].m_Explored[0]);
    }
    virtual int8_t* getTerrain_LandMassID()
    {
        RETURN_SAVEGAME_MEMBER_ADDRESS(m_Arcanus_LandMassID_Row[0].m_LandMassID[0]);
    }
    virtual Map_Movement* getTerrain_Movements()
    {
        RETURN_SAVEGAME_MEMBER_ADDRESS(m_Arcanus_Movement);
    }
    virtual eTerrainType* getTerrain_Types()
    {
        RETURN_SAVEGAME_MEMBER_ADDRESS(m_Arcanus_Map_Row[0].m_Tile[0]);
    }

    virtual Unit* getUnits()
    {
        RETURN_SAVEGAME_MEMBER(m_Unit);
    }

    virtual Unit_Type_Data* getUnit_Types()
    {
        MoMDataSegment* pMoMDataSegment = getDataSegment();
        if (0 == pMoMDataSegment)
            return 0;
        return &pMoMDataSegment->m_Hero_Types.Dwarf;
    }

    virtual int8_t* getUnrest_Table(eRace homeRace)
    {
        if (0 == m_TerrstatLbx.get())
            return 0;
        uint8_t* data = m_TerrstatLbx->getRecord(1);
        return (int8_t*)(data + homeRace * gMAX_RACES + 4);
    }

    virtual Wizard* getWizards()
    {
        RETURN_SAVEGAME_MEMBER(m_Wizards);
    }

    virtual bool isOpen() const
    {
        // TODO
        return true;
    }

//private:
    virtual MoMDataSegment* getDataSegment()
    {
        if (0 == m_WizardsExe.get())
            return 0;
        return (MoMDataSegment*)m_WizardsExe->getDataSegment();
    }

    virtual MoMMagicDataSegment* getMagicDataSegment()
    {
        if (0 == m_MagicExe.get())
            return 0;
        return (MoMMagicDataSegment*)m_MagicExe->getDataSegment();
    }

    virtual MoMExeMagic* getMagicExe()
    {
        return m_MagicExe.get();
    }

    virtual uint8_t* getMagicExeContents()
    {
        if (0 == m_MagicExe.get())
            return 0;
        return m_MagicExe->getExeContents();
    }

    virtual uint8_t* getMagicOverlay(size_t ovlNr);

    virtual SaveGameRef getSaveGame()
    {
        return { m_SaveGame.data(), m_SaveGame.size() };
    }

    virtual MoMExeWizards* getWizardsExe()
    {
        return m_WizardsExe.get();
    }

    virtual uint8_t* getWizardsExeContents()
    {
        if (0 == m_WizardsExe.get())
            return 0;
        return m_WizardsExe->getExeContents();
    }

    virtual uint8_t* getWizardsOverlay(size_t ovlNr);

#undef RETURN_SAVEGAME_MEMBER
#undef RETURN_SAVEGAME_MEMBER_ADDRESS

private:
    bool loadLbx(const std::string& filename, MoMLbxBase* lbxBase);
    bool saveLbx(const std::string& lbxTitle, MoMLbxBase* lbxBase, const std::string& filename);

private:
    std::vector<uint8_t> m_SaveGame;
    std::size_t m_size_SaveGame = 0;
    std::string m_filename_SaveGame;
    std::unique_ptr<MoMExeMagic> m_MagicExe;
    std::unique_ptr<MoMExeWizards> m_WizardsExe;
    std::string m_filename_MagicExe;
    std::string m_filename_WizardsExe;
    std::unique_ptr<MoMLbxBase> m_BuilddatLbx;
    std::unique_ptr<MoMLbxBase> m_SpelldatLbx;
    std::unique_ptr<MoMLbxBase> m_TerrstatLbx;
    std::string m_GameDirectory;
};

}
