// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#include "MoMGameBase.h"
#include "MoMProcess.h"
#include "MoMExeMagic.h"

namespace MoM {


class MoMGameCustom : public MoMGameBase
{
public:
    MoMGameCustom(void);
    virtual ~MoMGameCustom();

    bool openGame(std::unique_ptr<MoMProcess>& momProcess);

protected:
public:
    virtual void closeGame() throw();

    virtual bool commitData(void* ptr, const void* pNewValue, size_t size);

    virtual std::string getGameDirectory() const;

    virtual const char* getNameByOffset(DS_Offset offset);

    virtual bool isOpen() const
    {
        return ((0 != m_process.get()) && m_process->isOpen());
    }

    virtual bool readData()
    {
        return ((0 != m_process.get()) && m_process->readData());
    }
    virtual std::string getSources() const
    {
        return "Connected setup " + m_filename_MagicExe;
    }
    virtual MoM::City* getCities() const
    {
        return 0;
    }
    virtual WizardsExe_Game_Data* getGameData_WizardsExe();
    virtual Game_Settings* getGameSettings();
    virtual MoM::Hero_stats* getList_Hero_stats(MoM::ePlayer)
    {
        return 0;
    }
    virtual Hero_Stats_Initializer* getList_Hero_Stats_Initializer();
    virtual MoM::Item* getItems()
    {
        return 0;
    }
    virtual MoM::Tower_Node_Lair* getLairs()
    {
        return 0;
    }
    virtual Node_Attr* getNodeAttributes()
    {
        return 0;
    }
    virtual uint16_t* getNumber_of_Cities()
    {
        return 0;
    }
    virtual uint16_t* getNumber_of_Units()
    {
        return 0;
    }
    virtual uint16_t* getNumber_of_Wizards();
    virtual MoM::Unit* getUnits()
    {
        return 0;
    }
    virtual MoM::Wizard* getWizards();

    virtual MoMMagicDataSegment* getMagicDataSegment();

    virtual uint8_t* getMagicOverlay(size_t ovlNr);

    virtual uint8_t* getMagicExeContents()
    {
        if (0 == m_MagicExe.get())
            return 0;
        return m_MagicExe->getExeContents();
    }

    virtual MoMMagicDataSegment* getMagicExeDataSegment()
    {
        if (0 == m_MagicExe.get())
            return 0;
        return (MoMMagicDataSegment*)m_MagicExe->getDataSegment();
    }

    virtual MoMExeMagic* getMagicExe()
    {
        return m_MagicExe.get();
    }

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

private:
    template< typename T >
    T* derefHeapPointer(const EXE_Reloc& pointer, size_t nr_elements)
    {
        if (0 == m_process.get())
            return 0;

        if ((EXE_Reloc*)0 == validateStaticPointer((EXE_Reloc*)&pointer, 1))
            return 0;
        MoMMagicDataSegment* pMoMDataSegment = (MoMMagicDataSegment*)m_process->getDatasegmentData();
        if (0 == pMoMDataSegment)
            return 0;
        T* ptr = (T*)m_process->derefPointer(pointer, nr_elements * sizeof(T));
        if ((uint8_t*)ptr < (uint8_t*)pMoMDataSegment + sizeof(MoMMagicDataSegment))
        {
            ptr = 0;
        }
        return ptr;
    }

    template< typename T >
    T* validateStaticPointer(T* ptr, size_t nr_elements)
    {
        if (0 == m_process.get())
            return 0;

        MoMMagicDataSegment* pMoMDataSegment = (MoMMagicDataSegment*)m_process->getDatasegmentData();
        if (0 == pMoMDataSegment)
            return 0;
        if (!m_process->validPointer(ptr, nr_elements * sizeof(T)))
            return 0;
        if ((uint8_t*)ptr < (uint8_t*)pMoMDataSegment)
            return 0;
        if ((uint8_t*)ptr + nr_elements * sizeof(T) > (uint8_t*)pMoMDataSegment + sizeof(MoMMagicDataSegment))
            return 0;

        return ptr;
    }


    std::unique_ptr<MoMProcess> m_process;
    std::unique_ptr<MoMExeMagic> m_MagicExe;
    std::string m_filename_MagicExe;
};

}
