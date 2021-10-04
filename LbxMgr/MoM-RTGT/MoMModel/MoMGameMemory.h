// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#include "MoMGameBase.h"
#include "MoMProcess.h"
#include "MoMExeWizards.h"

namespace MoM {

class MoMGameMemory :
    public MoMGameBase
{
public:
    MoMGameMemory(void);
    virtual ~MoMGameMemory();

    bool openGame(std::unique_ptr<MoMProcess>& momProcess);

    virtual bool load(const char* filename);
    virtual bool save(const char* filename);

protected:
    virtual void closeGame() throw();

    virtual bool commitData(void* ptr, const void* pNewValue, size_t size);

    virtual std::string getGameDirectory() const;

    virtual eGameState* getGameState();
    virtual uint16_t* getGameTurn();
    virtual const char* getNameByOffset(DS_Offset offset);

    virtual std::string getSources() const
    {
        return "Connected game " + m_filename_WizardsExe;
    }

    virtual bool isBattleInProgress() const;

    virtual bool isOpen() const
    {
        return ((0 != m_process.get()) && m_process->isOpen());
    }

    virtual bool readData();

private:
    template< typename T >
    T* derefHeapPointer(const EXE_Reloc& pointer, size_t nr_elements) const
    {
        assert(0 != m_process.get());
        if ((EXE_Reloc*)0 == validateStaticPointer((EXE_Reloc*)&pointer, 1))
            return 0;
        MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
        if (0 == pMoMDataSegment)
            return 0;
        T* ptr = (T*)m_process->derefPointer(pointer, nr_elements * sizeof(T));
        if ((uint8_t*)ptr < (uint8_t*)pMoMDataSegment + sizeof(MoMDataSegment))
        {
            ptr = 0;
        }
        return ptr;
    }

    template< typename T >
    T* validateStaticPointer(T* ptr, size_t nr_elements) const
    {
        assert(0 != m_process.get());
        MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
        if (0 == pMoMDataSegment)
            return 0;
        if (!m_process->validPointer(ptr, nr_elements * sizeof(T)))
            return 0;
        if ((uint8_t*)ptr < (uint8_t*)pMoMDataSegment)
            return 0;
        if ((uint8_t*)ptr + nr_elements * sizeof(T) > (uint8_t*)pMoMDataSegment + sizeof(MoMDataSegment))
            return 0;

        return ptr;
    }

    virtual uint8_t* getArtifacts_in_game();
    virtual Available_spell_page* getAvailable_spell_pages();
    virtual Battlefield* getBattlefield();
    virtual Battle_Unit* getBattleUnitViewed();
    virtual Battle_Unit* getBattle_Units();
    virtual Building_Data* getBuildingData() const;
    virtual Hero_Choice* getChosen_Hero_Names();
    virtual City* getCities() const;
    virtual Events_Status* getEvents_Status();
    virtual Fortress* getFortresses();
    virtual WizardsExe_Game_Data* getGameData_WizardsExe();
    virtual Game_Settings* getGameSettings();
    virtual eSpell16* getHero_spells();
    virtual Hero_stats* getList_Hero_stats(ePlayer playerNr);
    virtual Item* getItems();
    virtual Tower_Node_Lair* getLairs();
    virtual Node_Attr* getNodeAttributes();
    virtual uint16_t* getNumber_of_Battle_Units();
    virtual uint16_t* getNumber_of_Cities();
    virtual uint16_t* getNumber_of_Units();
    virtual uint16_t* getNumber_of_Wizards();
    virtual Spell_Data* getSpellData();
    virtual Spells_Cast_in_Battle* getSpells_Cast_in_Battle();
    virtual uint16_t* getTaxUnrestTable();

    virtual eTerrainBonusDeposit* getTerrain_Bonuses();
    virtual Terrain_Changes* getTerrain_Changes();
    virtual uint8_t* getTerrain_Explored();
    virtual int8_t* getTerrain_LandMassID();
    // TODO: Weird. It appears that the terrain-movements are all zero in memory.
    //       Presumably they are swapped out or something.
    //       Either way, we can't do anything with it.
    //       Commented out for now, so it is treated as not accessible
    //virtual Map_Movement* getTerrain_Movements();
    virtual Map_Movement* getTerrain_Movements_copy();
    virtual eTerrainType* getTerrain_Types();

    virtual Unit* getUnits();
    virtual Unit_Type_Data* getUnit_Types();
    virtual int8_t* getUnrest_Table(eRace homeRace);
    virtual Wizard* getWizards();


    virtual MoMDataSegment* getDataSegment();
    virtual class MoMProcess* getMoMProcess()
	{
		return m_process.get();
	}
    virtual const uint8_t* getSeg0Pointer()
    {
        if (0 == m_process.get())
            return 0;
        return m_process->getSeg0Pointer();
    }
    virtual uint8_t* getWizardsExeContents()
    {
        if (0 == m_WizardsExe.get())
            return 0;
        return m_WizardsExe->getExeContents();
    }
    virtual MoMDataSegment* getWizardsExeDataSegment()
    {
        if (0 == m_WizardsExe.get())
            return 0;
        return (MoMDataSegment*)m_WizardsExe->getDataSegment();
    }
    virtual uint8_t* getWizardsOverlay(size_t ovlNr);

    virtual MoMExeWizards* getWizardsExe()
    {
        return m_WizardsExe.get();
    }

    std::unique_ptr<MoMProcess> m_process;
    std::unique_ptr<MoMExeWizards> m_WizardsExe;
    std::string m_filename_WizardsExe;
};

}
