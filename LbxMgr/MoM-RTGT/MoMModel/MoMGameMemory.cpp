// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include <assert.h>
#include <fstream>
#include <iostream>
#include <string.h> // memcmp()

#include "MoMController.h"
#include "MoMUtility.h"
#include "MoMExeWizards.h"

#include "MoMGameMemory.h"

namespace MoM {

namespace {

const size_t gMAX_NAME_SIZE = 256;

}

typedef uint8_t UnknownBuf[100];

struct MoMGamePointers
{
    char*       m_Offsets_SkillNames[eSkill_MAX];   // ds:0130
    char*       m_Offsets_CitySizeNames[eCity_Size_MAX]; // ds:0156
    char*       m_Offsets_CitySpecialNames[7];      // ds:0162
    char*       m_Offset_Arcane;                    // ds:1ED8  // UNREFERENCED???
    char*       m_Offsets_RealmNames_5[5];          // ds:1EDA
    char*       m_Offsets_CityEnchantmentNames[eCityEnchantments_MAX];   // ds:1EE4
    char*       m_Offsets_PersonalityNames[ePersonality_MAX];   // ds:1FBC
    char*       m_Offsets_ObjectiveNames[eObjective_MAX];       // ds:1FD4
    char*       m_Offsets_UnitLevelNames[gMAX_LEVELS_UNIT];          // ds:3F46
    char*       m_Offsets_HeroLevelNames[gMAX_LEVELS_HERO];          // ds:3F52

    UnknownBuf*             m_EMS_data_reference_point; // 760C
    Spell_Data*             m_addr_Spell_Data ; // 912C
    Item*                   m_addr_Items  ; // 9136
    UnknownBuf*        m_addr_Artifacts_in_Game   ; // 913A
    Battle_Unit*            m_addr_Battle_Unit_View  ; // 9226
    Battle_Unit*            m_addr_Battle_Unit  ; // 922A
    Spells_Cast_in_Battle*  m_addr_Spells_Cast_in_Battle   ; // 922E
    Hero_stats*             m_addr_Hero_stat[6]   ; // 9232
    Battlefield*            m_addr_Battlefield    ; // 9274
    UnknownBuf*        dword_3FD50 ; // 92B0
    UnknownBuf*        addr_3FDE6  ; // 9346
    Building_Data*          m_addr_Building_Data ; // 938C
    UnknownBuf*        dword_3FE32 ; // 9392
    UnknownBuf*        word_3FF00  ; // 9460
    UnknownBuf*        word_3FFBE  ; // 951E
    UnknownBuf*        word_3FFC8  ; // 9528
    UnknownBuf*        word_403FE  ; // 995E
    UnknownBuf*        word_40428  ; // 9988
    Events_Status*          m_addr_events ; // 9998
    UnknownBuf*        dword_40730 ; // 9C90
    UnknownBuf*        dword_4073C ; // 9C9C
    UnknownBuf*        dword_40740 ; // 9CA0
    UnknownBuf*        m_addr_terrain_Movement_copy   ; // 9CA4
    UnknownBuf*        dword_40748 ; // 9CA8
    Map_Movement*           m_addr_Terrain_Movement ; // 9CAC
    MapRow_Exploration*     m_addr_Terrain_Explored    ; // 9CB0
    MapRow_Terrain_Changes* m_addr_Terrain_Changes  ; // 9CB4
    MapRow_Bonus*           m_addr_Terrain_Bonuses    ; // 9CB8
    City*                   m_addr_Cities ; // 9CBC
    Tower_Node_Lair*        m_addr_Lairs_data   ; // 9CC0
    Tower_Attr*             m_addr_tower_attr ; // 9CC4
    Fortress*               m_addr_fortress_data    ; // 9CC8
    Node_Attr*              m_addr_Nodes_Attr    ; // 9CCC
    MapRow_LandMassID*      m_addr_Terrain_LandMassID  ; // 9CD0
    MapRow_Terrain*         m_addr_Terrain_Types ; // 9CDC
    int8_t*                 m_addr_Unrest_Table[gMAX_RACES]  ; // 9CE0
    UnknownBuf*        dword_407CA ; // 9D2A
    UnknownBuf*        dword_407CE ; // 9D2E
    UnknownBuf*        dw_Nr_in_stack  ; // 9E8C
    UnknownBuf*        dw_stack_place  ; // 9E90
    UnknownBuf*        dw_pla_stacks_loc   ; // 9E94
    UnknownBuf*        dw_Y_stacks_loc ; // 9E98
    UnknownBuf*        dw_X_stacks_loc ; // 9E9C
    UnknownBuf*        dw_bx_6160  ; // 9EA0
    UnknownBuf*        dw_bx_615C  ; // 9EA4
    UnknownBuf*        dw_bx_6176  ; // 9EA8
    UnknownBuf*        dw_bx_6154  ; // 9EAC
    Unit*           m_addr_Units    ; // 9EC2
    Hero_Choice*    m_addr_Chosen_Hero_Names;   // 9EC6

    UnitView_Lines*       m_Offset_UnitView_Lines;  // C192
    Battle_Figure*   m_addr_Battle_figures_256;         // D15A
    ClickableArea*   m_addr_ClickableArea;          // E8AC
};

static MoMGamePointers gMoMGamePointers;    // TODO: For DEBUG purposes

MoMGameMemory::MoMGameMemory(void)
: MoMGameBase(),
  m_process(),
  m_WizardsExe(),
  m_filename_WizardsExe()
{
}

MoMGameMemory::~MoMGameMemory()
{
    closeGame();
}

void MoMGameMemory::closeGame() throw()
{
    try
    {
        m_process.reset();
        m_WizardsExe.reset();
        m_filename_WizardsExe.clear();
    }
    catch (...)
    {
        // Make sure this cleanup function does not throw
    }
}

bool MoMGameMemory::commitData(void* ptr, const void* pNewValue, size_t size)
{
    bool ok = true;

    // TODO: Compare old value with new value BEFORE commit
    //       What to do if the memory has changed? Perhaps significantly?
    //       - DOSBox restarted
    //       - MoM MAGIC.EXE or WIZARDS.EXE restarted
    //       - Content of memory reordered, such as the order of the Units
    //       Presumably we want to warn the user of this and allow him to overrule
    if ((0 != m_WizardsExe.get()) && m_WizardsExe->writeData(ptr, pNewValue, size))
    {
        memcpy(ptr, pNewValue, size);
        /*
        // TODO: Give warning with confirm to modify the file
        // TODO: Make backup of original if not already done so
        //       Let's call it "WIZARDS.EXE_TweakerBackup"
        std::fstream::pos_type pos = (std::fstream::pos_type)(ptrByte - (uint8_t*)m_WizardsExe.get());
        std::fstream fs(m_filename_WizardsExe.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
        if (!fs)
        {
            std::cout << "WARN: Could not open file for writing '" << m_filename_WizardsExe << "'" << std::endl;
            ok = false;
        }
        else if (!fs.seekp(pos))
        {
            std::cout << "WARN: Could not seek to pos 0x" << std::hex << pos << std::dec << " in file '" << m_filename_WizardsExe << "'" << std::endl;
            ok = false;
        }
        else if (!fs.write((const char*)pNewValue, size))
        {
            std::cout << "WARN: Could not write data to pos 0x" << std::hex << pos << std::dec << " in file '" << m_filename_WizardsExe << "'" << std::endl;
            ok = false;
        }
        else
        {
            std::cout << "INFO: " << size << " bytes written to pos 0x" << std::hex << pos << std::dec << " in file '" << m_filename_WizardsExe << "'" << std::endl;
            memcpy(ptr, pNewValue, size);
        }
        */
    }
    else if (0 != m_process.get())
    {
        memcpy(ptr, pNewValue, size);
        ok = m_process->writeData(ptr, size);
    }
    else
    {
        ok = false;
    }
    return ok;
}

#define SET_FIELD_OFFSET(field) \
        gMoMGamePointers.field = (char*)dseg + dseg->field;
 
#define SET_ARRAY_OFFSET(field) \
        for (size_t i = 0; i < ARRAYSIZE(gMoMGamePointers.field); ++i) \
        { \
            gMoMGamePointers.field[i] = (char*)dseg + dseg->field[i]; \
        }

#define SET_RELOC_POINTER(type, field) \
        gMoMGamePointers.field = derefHeapPointer<type>(dseg->field, 1);

bool MoMGameMemory::readData()
{
    bool ok = false;
    setErrorString("");
    ok = ((0 != m_process.get()) && m_process->readData());

    // TODO: FOR DEBUGGING
    if (ok)
    {
        MoMDataSegment* dseg = getDataSegment();

        SET_ARRAY_OFFSET(m_Offsets_SkillNames);
        SET_ARRAY_OFFSET(m_Offsets_CitySizeNames);
        SET_ARRAY_OFFSET(m_Offsets_CitySpecialNames);
        SET_FIELD_OFFSET(m_Offset_Arcane);
        SET_ARRAY_OFFSET(m_Offsets_RealmNames_5);
        SET_ARRAY_OFFSET(m_Offsets_CityEnchantmentNames);
        SET_ARRAY_OFFSET(m_Offsets_PersonalityNames);
        SET_ARRAY_OFFSET(m_Offsets_ObjectiveNames);
        SET_ARRAY_OFFSET(m_Offsets_UnitLevelNames);
        SET_ARRAY_OFFSET(m_Offsets_HeroLevelNames);

        EXE_Reloc ems_page = { 0, dseg->m_EMS_data_reference_point };
        gMoMGamePointers.m_EMS_data_reference_point = (UnknownBuf*)m_process->derefPointer(ems_page, 0x4000);

        SET_RELOC_POINTER(Spell_Data, m_addr_Spell_Data);
        SET_RELOC_POINTER(Item, m_addr_Items);
        SET_RELOC_POINTER(UnknownBuf, m_addr_Artifacts_in_Game);
        SET_RELOC_POINTER(Battle_Unit, m_addr_Battle_Unit_View);
        SET_RELOC_POINTER(Battle_Unit, m_addr_Battle_Unit);

        SET_RELOC_POINTER(Spells_Cast_in_Battle, m_addr_Spells_Cast_in_Battle);
        for (size_t i = 0; i < ARRAYSIZE(gMoMGamePointers.m_addr_Hero_stat); ++i)
        {
            SET_RELOC_POINTER(Hero_stats, m_addr_Hero_stat[i]);
        }
        SET_RELOC_POINTER(Battlefield, m_addr_Battlefield);
        SET_RELOC_POINTER(UnknownBuf, dword_3FD50);
        SET_RELOC_POINTER(UnknownBuf, addr_3FDE6);
        SET_RELOC_POINTER(Building_Data, m_addr_Building_Data);
        SET_RELOC_POINTER(UnknownBuf, dword_3FE32);
        SET_RELOC_POINTER(UnknownBuf, word_3FF00);
        SET_RELOC_POINTER(UnknownBuf, word_3FFBE);
        SET_RELOC_POINTER(UnknownBuf, word_3FFC8);
        SET_RELOC_POINTER(UnknownBuf, word_403FE);
        SET_RELOC_POINTER(UnknownBuf, word_40428);
        SET_RELOC_POINTER(Events_Status, m_addr_events);
        SET_RELOC_POINTER(UnknownBuf, dword_40730);
        SET_RELOC_POINTER(UnknownBuf, dword_4073C);
        SET_RELOC_POINTER(UnknownBuf, dword_40740);
        SET_RELOC_POINTER(UnknownBuf, m_addr_terrain_Movement_copy);
        SET_RELOC_POINTER(UnknownBuf, dword_40748);
        SET_RELOC_POINTER(Map_Movement, m_addr_Terrain_Movement);
        SET_RELOC_POINTER(MapRow_Exploration, m_addr_Terrain_Explored);
        SET_RELOC_POINTER(MapRow_Terrain_Changes, m_addr_Terrain_Changes);
        SET_RELOC_POINTER(MapRow_Bonus, m_addr_Terrain_Bonuses);
        SET_RELOC_POINTER(City, m_addr_Cities);
        SET_RELOC_POINTER(Tower_Node_Lair, m_addr_Lairs_data);
        SET_RELOC_POINTER(Tower_Attr, m_addr_tower_attr);
        SET_RELOC_POINTER(Fortress, m_addr_fortress_data);
        SET_RELOC_POINTER(Node_Attr, m_addr_Nodes_Attr);
        SET_RELOC_POINTER(MapRow_LandMassID, m_addr_Terrain_LandMassID);
        SET_RELOC_POINTER(MapRow_Terrain, m_addr_Terrain_Types);
        for (size_t i = 0; i < ARRAYSIZE(gMoMGamePointers.m_addr_Unrest_Table); ++i)
        {
            SET_RELOC_POINTER(int8_t, m_addr_Unrest_Table[i]);
        }
        SET_RELOC_POINTER(UnknownBuf, dword_407CA);
        SET_RELOC_POINTER(UnknownBuf, dword_407CE);
        SET_RELOC_POINTER(UnknownBuf, dw_Nr_in_stack);
        SET_RELOC_POINTER(UnknownBuf, dw_stack_place);
        SET_RELOC_POINTER(UnknownBuf, dw_pla_stacks_loc);
        SET_RELOC_POINTER(UnknownBuf, dw_Y_stacks_loc);
        SET_RELOC_POINTER(UnknownBuf, dw_X_stacks_loc);
        SET_RELOC_POINTER(UnknownBuf, dw_bx_6160);
        SET_RELOC_POINTER(UnknownBuf, dw_bx_615C);
        SET_RELOC_POINTER(UnknownBuf, dw_bx_6176);
        SET_RELOC_POINTER(UnknownBuf, dw_bx_6154);
        SET_RELOC_POINTER(Unit, m_addr_Units);
        SET_RELOC_POINTER(Hero_Choice, m_addr_Chosen_Hero_Names);

        gMoMGamePointers.m_Offset_UnitView_Lines = validateStaticPointer(
            reinterpret_cast<UnitView_Lines*>((uint8_t*)dseg + dseg->m_Offset_UnitView_Lines), 1);
        gMoMGamePointers.m_addr_Battle_figures_256 = derefHeapPointer<Battle_Figure>(dseg->m_addr_Battle_figures_256, 256);

        SET_RELOC_POINTER(ClickableArea, m_addr_ClickableArea);
    }

    return ok;
}

bool MoMGameMemory::load(const char* filename)
{
    bool ok = false;
    setErrorString("");
    if (0 == m_process.get())
    {
        setErrorString("no connected process");
    }
    else if (!m_process->load(filename))
    {
        setErrorString("process proxy failed to load");
    }
    else
    {
        ok = true;
    }
    return ok;
}

bool MoMGameMemory::save(const char* filename)
{
    bool ok = false;
    setErrorString("");
    if (0 == m_process.get())
    {
        setErrorString("no connected process");
    }
    else if (!m_process->save(filename))
    {
        setErrorString("process proxy failed to save");
    }
    else
    {
        ok = true;
    }
    return ok;
}

uint8_t *MoMGameMemory::getArtifacts_in_game()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<uint8_t>(pMoMDataSegment->m_addr_Artifacts_in_Game, gMAX_ARTIFACTS_IN_GAME);
}

Available_spell_page* MoMGameMemory::getAvailable_spell_pages()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return (Available_spell_page*)((uint8_t*)pMoMDataSegment + pMoMDataSegment->m_offset_Available_spell_pages);
}

Battlefield* MoMGameMemory::getBattlefield()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    if (0 == pMoMDataSegment)
        return 0;
    return derefHeapPointer<Battlefield>(pMoMDataSegment->m_addr_Battlefield, 1);
}

Battle_Unit* MoMGameMemory::getBattleUnitViewed()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Battle_Unit>(pMoMDataSegment->m_addr_Battle_Unit_View, 1);
}

uint16_t* MoMGameMemory::getNumber_of_Battle_Units()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(&pMoMDataSegment->m_Nr_Battle_Units, 1);
}

Battle_Unit* MoMGameMemory::getBattle_Units()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Battle_Unit>(pMoMDataSegment->m_addr_Battle_Unit, MoM::gMAX_BATTLE_UNITS);
}

City* MoMGameMemory::getCities() const
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<City>(pMoMDataSegment->m_addr_Cities, MoM::gMAX_CITIES);
}

MoMDataSegment* MoMGameMemory::getDataSegment()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(pMoMDataSegment, 1);
}

Building_Data* MoMGameMemory::getBuildingData() const
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Building_Data>(pMoMDataSegment->m_addr_Building_Data, eBuilding_array_MAX);
}

Hero_Choice *MoMGameMemory::getChosen_Hero_Names()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Hero_Choice>(pMoMDataSegment->m_addr_Chosen_Hero_Names, gMAX_HERO_TYPES);
}

Events_Status* MoMGameMemory::getEvents_Status()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Events_Status>(pMoMDataSegment->m_addr_events, 1);
}

Fortress* MoMGameMemory::getFortresses()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Fortress>(pMoMDataSegment->m_addr_fortress_data, gMAX_WIZARD_RECORDS);
}

std::string MoMGameMemory::getGameDirectory() const
{
    std::string dir;
    if (0 != m_process.get())
    {
        dir = dirFromFilepath(m_process->getExeFilepath());
    }
    return dir;
}

WizardsExe_Game_Data* MoMGameMemory::getGameData_WizardsExe()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(&pMoMDataSegment->m_Game_Data, 1);
}

Game_Settings* MoMGameMemory::getGameSettings()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(&pMoMDataSegment->m_Game_Settings, 1);
}

eGameState* MoMGameMemory::getGameState()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(&pMoMDataSegment->m_Game_flow, 1);
}

uint16_t* MoMGameMemory::getGameTurn()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(&pMoMDataSegment->m_Game_Data.m_Current_Turn, 1);
}

Item* MoMGameMemory::getItems()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Item>(pMoMDataSegment->m_addr_Items, MoM::gMAX_ITEMS_VALID);
}

eSpell16* MoMGameMemory::getHero_spells()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<eSpell16>(pMoMDataSegment->m_addr_Hero_Spells, 4);
}

Hero_stats* MoMGameMemory::getList_Hero_stats(ePlayer playerNr)
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Hero_stats>(pMoMDataSegment->m_addr_Hero_stat[playerNr], MoM::gMAX_HERO_TYPES);
}

Tower_Node_Lair* MoMGameMemory::getLairs()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Tower_Node_Lair>(pMoMDataSegment->m_addr_Lairs_data, MoM::gMAX_NODES_LAIRS_TOWERS);
}

Node_Attr *MoMGameMemory::getNodeAttributes()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Node_Attr>(pMoMDataSegment->m_addr_Nodes_Attr, MoM::gMAX_NODES);
}

const char* MoMGameMemory::getNameByOffset(DS_Offset offset)
{
    if (0 == m_process.get())
        return 0;
    char* namebuffer = (char*)m_process->getDatasegmentData();
    if (0 == validateStaticPointer(namebuffer, gMAX_NAME_SIZE))
        return 0;
    // Check if there is a terminating zero in range
    for (size_t i = 0; i < gMAX_NAME_SIZE; ++i)
    {
        if (namebuffer[i] == '\0')
        {
            return namebuffer + offset;
        }
    }
    // No terminating zero found
    return 0;
}

bool MoMGameMemory::isBattleInProgress() const
{
    bool battleInProgress = false;
    std::string gameDirectory = getGameDirectory();
    if (!gameDirectory.empty())
    {
        std::string combatTmpFile = gameDirectory + "/" + "COMBAT.TMP";
        std::ifstream ifs(combatTmpFile.c_str(), std::ios::binary | std::ios::in);
        City tmpCity = City();
        if (ifs && ifs.seekg(0x0020, std::ios::beg) && ifs.read(reinterpret_cast<char*>(&tmpCity), sizeof(City)))
        {
            battleInProgress = (0 != memcmp(tmpCity.m_City_Name, getCity(0)->m_City_Name, sizeof(tmpCity.m_City_Name)));
        }
    }
    return battleInProgress;
}

uint16_t* MoMGameMemory::getNumber_of_Cities()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(&pMoMDataSegment->m_Game_Data.m_Number_of_Cities, 1);
}

uint16_t* MoMGameMemory::getNumber_of_Units()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(&pMoMDataSegment->m_Game_Data.m_Number_of_Units, 1);
}

uint16_t* MoMGameMemory::getNumber_of_Wizards()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(&pMoMDataSegment->m_Game_Data.m_Number_of_Wizards, 1);
}

uint8_t* MoMGameMemory::getWizardsOverlay(size_t ovlNr)
{
    if (0 == m_WizardsExe.get())
        return 0;

    uint8_t* ptr = m_WizardsExe->getOverlay(ovlNr);

    return ptr;
}

Spell_Data* MoMGameMemory::getSpellData()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Spell_Data>(pMoMDataSegment->m_addr_Spell_Data, MoM::eSpell_MAX);
}

Spells_Cast_in_Battle* MoMGameMemory::getSpells_Cast_in_Battle()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Spells_Cast_in_Battle>(pMoMDataSegment->m_addr_Spells_Cast_in_Battle, 1);
}

uint16_t* MoMGameMemory::getTaxUnrestTable()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(pMoMDataSegment->m_Tax_Unrest_Table, eTax_Rate_MAX);
}

eTerrainBonusDeposit* MoMGameMemory::getTerrain_Bonuses()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<eTerrainBonusDeposit>(pMoMDataSegment->m_addr_Terrain_Bonuses,
            ePlane_MAX * gMAX_MAP_ROWS * gMAX_MAP_COLS);
}

Terrain_Changes* MoMGameMemory::getTerrain_Changes()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Terrain_Changes>(pMoMDataSegment->m_addr_Terrain_Changes,
            ePlane_MAX * gMAX_MAP_ROWS * gMAX_MAP_COLS);
}

uint8_t* MoMGameMemory::getTerrain_Explored()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<uint8_t>(pMoMDataSegment->m_addr_Terrain_Explored,
            ePlane_MAX * gMAX_MAP_ROWS * gMAX_MAP_COLS);
}

int8_t* MoMGameMemory::getTerrain_LandMassID()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<int8_t>(pMoMDataSegment->m_addr_Terrain_LandMassID,
            ePlane_MAX * gMAX_MAP_ROWS * gMAX_MAP_COLS);
}

//Map_Movement* MoMGameMemory::getTerrain_Movements()
//{
//    if (0 == m_process.get())
//        return 0;
//    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
//    return derefHeapPointer<Map_Movement>(pMoMDataSegment->m_addr_Terrain_Movement,
//            ePlane_MAX);
//}

Map_Movement* MoMGameMemory::getTerrain_Movements_copy()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Map_Movement>(pMoMDataSegment->m_addr_terrain_Movement_copy,
            ePlane_MAX);
}

eTerrainType* MoMGameMemory::getTerrain_Types()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<eTerrainType>(pMoMDataSegment->m_addr_Terrain_Types,
            ePlane_MAX * gMAX_MAP_ROWS * gMAX_MAP_COLS);
}

Unit* MoMGameMemory::getUnits()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<Unit>(pMoMDataSegment->m_addr_Units, MoM::gMAX_UNITS);
}

Unit_Type_Data* MoMGameMemory::getUnit_Types()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(&pMoMDataSegment->m_Hero_Types.Dwarf, MoM::eUnit_Type_MAX);
}

Wizard* MoMGameMemory::getWizards()
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(pMoMDataSegment->m_Wizards, MoM::gMAX_WIZARD_RECORDS);
}

int8_t* MoMGameMemory::getUnrest_Table(eRace homeRace)
{
    if (0 == m_process.get())
        return 0;
    MoMDataSegment* pMoMDataSegment = (MoMDataSegment*)m_process->getDatasegmentData();
    return derefHeapPointer<int8_t>(pMoMDataSegment->m_addr_Unrest_Table[homeRace], MoM::gMAX_RACES);
}

bool MoMGameMemory::openGame(std::unique_ptr<MoMProcess>& momProcess)
{
    if (0 == momProcess.get())
        return false;
    if (MoM::gOFFSET_WIZARDS_DSEG_CODE != momProcess->getOffset_DS_Code())
        return false;

    closeGame();

    // Transfer ownership
    m_process.swap(momProcess);

    m_filename_WizardsExe = m_process->getExeFilepath();
    if (m_filename_WizardsExe.empty())
    {
        // TODO: errorhandling??
        return true;
    }

    m_WizardsExe.reset(new MoMExeWizards);
    if (!m_WizardsExe->load(m_filename_WizardsExe))
    {
        std::cout << "Could not (fully) read file '"<< m_filename_WizardsExe << "'" << std::endl;
        m_WizardsExe.reset();
        // TODO: errorhandling??
        return true;
    }

    return true;
}

}
