#include <cassert>
#include <cstdio>
#include <iostream>

#include "MoMGameBase.h"
#include "MoMProcess.h"
#include "MoMUtility.h"

#include "MoMHookManager.h"

namespace MoM {

MoMHookManager::MoMHookManager(MoMGameBase* game) :
    m_game(game),
    m_process()
{
    if (0 != m_game)
    {
        m_process = m_game->getMoMProcess();
    }
}

///
/// MoM assembly interface for combat
///
/// \param[in] loc_HOOK identifies the value that needs to be set to zero to disable the hook
/// \param[in] battleUnitID     whose turn it is to move
/// \param[in,out] targetID     that is targeted by the battle unit
/// \param[in,out] tactic       that is used by the battle unit (100=melee, 101=shoot, ...)
/// \param[in,out] target_xpos
/// \param[in,out] target_xpos
/// \param[in,out] parm1
/// \param[in,out] parm2

/// \param[in] battleUnitID whose turn it is to move

//dseg            segment byte public 'DATA' use16

//ORG 922Ah
//addr_Battle_unit        dd 0

//ORG 9294h
//baseptr                 dw 0
//ORG 9296h
//bait                    dw 0

//dseg            ends

//ovr114          segment	byte public 'CODE' use16
//                assume cs:ovr114
//                assume es:nothing, ss:nothing, ds:dseg

//ORG 0344h

//Unit_strategy_exe       proc far       ; CODE XREF: ___69
//                                       ; Army_autocombat+2E0
//                                       ; Army_autocombat+3A2

//parm1                    = word ptr -0Ah
//parm2                    = word ptr -8
//target_unit_at_gate      = word ptr -6
//casting_failed           = word ptr -4
//targetID_in_gate_OR_moves_left= word ptr -2
//battleUnitID             = word ptr  6
//target_xpos              = word ptr  8
//target_ypos              = word ptr  0Ah

//si_battleUnitID = si
//di_target_xpos = di
//                ;; Original
//                push    bp
//                mov     bp, sp
//                sub     sp, 0Ah
//                push    si
//                push    di

//                ;; Added
//                mov     ax, bp
//                mov     [baseptr], ax               ; Make stack variables accessible
//loc_HOOK:
//                mov     ax, 1                       ;; The "hook"
//                mov     [bait], ax
//loc_wait_external_AI:
//                mov     ax, [bait]
//                or      ax, ax
//                jnz     loc_wait_external_AI

//                xor     ax, ax
//                mov     [baseptr], ax               ; Restore dseg:9294

//                ;; Original
//                mov     si_battleUnitID, [bp+battleUnitID]
//                mov     di_target_xpos, [bp+target_xpos]
//                or      di_target_xpos, di_target_xpos
//                jnz     short loc_92521

//                cmp     [bp+target_ypos], 0
//                jnz     short loc_92521

//                mov     ax, si_battleUnitID
//                mov     dx, 6Eh ; 'n'
//                imul    dx
//                les     bx, addr_Battle_unit
//                add     bx, ax
//                mov     al, es:[bx+43h]
//                cbw
//                mov     dx, 6Eh ; 'n'
//                imul    dx
//                les     bx, addr_Battle_unit
//                add     bx, ax
//                mov     di_target_xpos, es:[bx+44h]
//                mov     [bp+target_ypos], ax

//                ;; Jump to regular continuation of code (could also be NOP's)
//                jmp     loc_92521

//                ;; Original
//ORG 03A1h
//loc_92521:                              ; CODE XREF: Unit_strategy_exe+10
//                                        ; Unit_strategy_exe+16
//                push   si_battleUnitID ; battleUnitID
//                ;; call   j_choose_next_unit_for_action
//                ;; ...

//Unit_strategy_exe       endp

//                ;; ...

//ovr114           ends
//
//Notice the value where it is marked "hook".
//If you replace the 1 by 0, then external ai is disabled.
//It's purpose is choose whether to use the original ai or wait for the external program.
//When the hook != 0, then MoM waits until dseg:9296 is reset to zero.
//
//Code:
//Replace opcodes in 0x889E4
//558BEC83EC0656578B76068B7E080BFF754B837E0A0075458BC6BA6E00F7EAC41E2A9203D8268A474398BA6E00F7EAC41E2A9203D8268B7F448BC6BA6E00F7EAC41E2A9203D8268A474398BA6E00F7EAC41E2A9203D8268B474689460A
//
//with the code shown below
//

const size_t gHookCode_exeOffset = 0x889E4;
const char gHook_unit_strategy_exe_str[] =
    "55 8B EC 83 EC 0A 56 57  8B C5 A3 94 92 B8 01 00\n"
    "A3 96 92 A1 96 92 0B C0  75 F9 33 C0 A3 94 92 8B\n"
    "76 06 8B 7E 08 0B FF 75  34 83 7E 0A 00 75 2E 8B\n"
    "C6 BA 6E 00 F7 EA C4 1E  2A 92 03 D8 26 8A 47 43\n"
    "98 BA 6E 00 F7 EA C4 1E  2A 92 03 D8 26 8B 7F 44\n"
    "89 46 0A EB 08 90                               \n";
const size_t gHookFlag_exeOffset = gHookCode_exeOffset + 0x0E;


bool MoMHookManager::insertHook()
{
    m_errorString.clear();
    if(0 == m_game)
        return false;

    bool ok = true;
    uint8_t* codeBase = m_game->getWizardsExeContents();
    if (0 == codeBase)
    {
        std::cout << "failed to retrieve WIZARDS.EXE contents" << std::endl;
        ok = false;
    }

    std::vector<uint8_t> hook_unit_strategy_exe_code;
    if (ok)
    {
        const char* s = gHook_unit_strategy_exe_str;
        bool scannedSuccessful = true;
        while (scannedSuccessful)
        {
            unsigned hexbyte = 0;
            int nread = 0;
            scannedSuccessful = (1 <= std::sscanf(s, "%02X%n", &hexbyte, &nread));
            if (scannedSuccessful)
            {
                hook_unit_strategy_exe_code.push_back(static_cast<uint8_t>(hexbyte));
                s += nread;
            }
        }
        std::cout << "Scanned " << hook_unit_strategy_exe_code.size() << " bytes for hook code" << std::endl;
    }

    if (ok)
    {
        uint8_t* codeHook = codeBase + gHookCode_exeOffset;
        ok = m_game->commitData(codeHook, &hook_unit_strategy_exe_code[0], hook_unit_strategy_exe_code.size());
        if (!ok)
        {
            std::cout << "failed to commit hook_unit_strategy_exe_code change" << std::endl;
        }
    }

    return ok;
}

void MoMHookManager::removeHook()
{
    m_errorString.clear();
    std::cout << "NOT IMPLEMENTED" << std::endl;
}

bool MoMHookManager::raiseHook()
{
    m_errorString.clear();
    if ((0 == m_process) || (0 == m_game) || (0 == m_game->getDataSegment()))
        return false;

    bool ok = true;
    uint8_t* codeBase = m_game->getWizardsExeContents();
    if (0 == codeBase)
    {
        std::cout << "failed to retrieve WIZARDS.EXE contents" << std::endl;
        ok = false;
    }

    if (ok)
    {
        uint8_t* codeHookFlag = codeBase + gHookFlag_exeOffset;
        uint16_t hookValue = 1;
        ok = m_game->commitData(codeHookFlag, &hookValue, 1);
        if (!ok)
        {
            std::cout << "failed to commit hookFlag change" << std::endl;
        }
    }

    return ok;
}

bool MoMHookManager::retractHook()
{
    m_errorString.clear();
    if ((0 == m_process) || (0 == m_game) || (0 == m_game->getDataSegment()))
        return false;

    bool ok = true;
    uint8_t* codeBase = m_game->getWizardsExeContents();
    if (0 == codeBase)
    {
        std::cout << "failed to retrieve WIZARDS.EXE contents" << std::endl;
        ok = false;
    }

    if (ok)
    {
        uint8_t* codeHookFlag = codeBase + gHookFlag_exeOffset;
        uint16_t hookValue = 0;
        ok = m_game->commitData(codeHookFlag, &hookValue, 1);
        if (!ok)
        {
            std::cout << "failed to commit hookFlag change" << std::endl;
        }
    }

    return ok;
}

bool MoMHookManager::waitForBait(double timeout)
{
    m_errorString.clear();
    if ((0 == m_process) || (0 == m_game) || (0 == m_game->getDataSegment()))
        return false;

    const double sleepTime = 0.001;

    // Wait till dseg:9296 is 1
    uint8_t* ptrBait = (uint8_t*)&m_game->getDataSegment()->m_kyrub_dseg_9296;
    bool ok = true;
    bool baitReady = false;
    int nrTries = (int)(timeout / sleepTime + 0.5);
    for (int tryNr = 0; ok && !baitReady && (tryNr < nrTries); ++tryNr)
    {
        ok = m_process->readData(ptrBait, 1);
        if (!ok)
        {
            std::cout << "failed to read from dseg:9297" << std::endl;
        }
        else
        {
            baitReady = (0 != *ptrBait);
        }
        m_process->sleepSec(0.001);
    }
    return baitReady;
}

bool MoMHookManager::releaseBait()
{
    m_errorString.clear();
    if ((0 == m_process) || (0 == m_game) || (0 == m_game->getDataSegment()))
        return false;

    // Set dseg:9296 to 0
    uint8_t* ptrBait = (uint8_t*)&m_game->getDataSegment()->m_kyrub_dseg_9296;
    *ptrBait = 0;
    bool ok = m_process->writeData(ptrBait, 1);
    if (!ok)
    {
        std::cout << "failed to commit dseg:9296 reset" << std::endl;
    }
    return ok;
}

bool MoMHookManager::isBaitReady()
{
    m_errorString.clear();
    if ((0 == m_process) || (0 == m_game) || (0 == m_game->getDataSegment()))
        return false;

    return pdIsBaitReady();
}

bool MoMHookManager::readBaitData(MoMHookData& data)
{
    bool ok = true;
    m_errorString = "Bait data read";

    if (ok)
    {
        ok = m_game->readData();
        if (!ok)
        {
            m_errorString = "Could not read game data";
        }
    }

    MoM::MoMDataSegment* dseg = 0;
    if (ok)
    {
        dseg = m_game->getDataSegment();
        ok = (0 != dseg);
        if (!ok)
        {
            m_errorString = "Could not get wizards.exe data segment";
        }
    }

    if (ok)
    {
        uint16_t basePointer = (int16_t)dseg->m_kyrub_dseg_9294;
        uint8_t* sseg = (uint8_t*)dseg;

        data.battleUnitNr = *(int16_t*)(&sseg[basePointer + 6]);
        data.targetXpos = *(int16_t*)(&sseg[basePointer + 8]);
        data.targetYpos = *(int16_t*)(&sseg[basePointer + 0x0A]);
        data.parm1 = *(int16_t*)(&sseg[basePointer - 0x0A]);
        data.parm2 = *(int16_t*)(&sseg[basePointer - 8]);
        data.targetID = -1;
        data.tactic = -1;

        MoM::Battle_Unit* battleUnit = m_game->getBattleUnit(data.battleUnitNr);
        if (0 != battleUnit)
        {
            data.targetID = battleUnit->m_Target_BattleUnitID;
            data.tactic = battleUnit->m_Tactic;
        }
    }

    std::cout << m_errorString << std::endl;

    return ok;
}

bool MoMHookManager::writeBaitData(const MoMHookData &data)
{
    bool ok = true;
    m_errorString = "Bait data written";

    ok = pdIsBaitReady();
    if (!ok)
    {
        m_errorString = "Bait was not ready";
    }

    uint16_t basePointer = (int16_t)m_game->getDataSegment()->m_kyrub_dseg_9294;
    uint8_t* sseg = (uint8_t*)m_game->getDataSegment();

    MoM::Battle_Unit* battleUnit = m_game->getBattleUnit(data.battleUnitNr);

    if (ok)
    {
        ok = ((0 != sseg) && (0 != battleUnit));
        if (!ok)
        {
            m_errorString = "Cannot access sseg or the battle unit";
        }
    }

    if (ok)
    {
        battleUnit->m_Target_BattleUnitID = data.targetID;
        battleUnit->m_Tactic = (MoM::eBattleUnitTactic)data.tactic;

        *(int16_t*)(&sseg[basePointer + 6]) = data.battleUnitNr;
        *(int16_t*)(&sseg[basePointer + 8]) = data.targetXpos;
        *(int16_t*)(&sseg[basePointer + 0x0A]) = data.targetYpos;
        *(int16_t*)(&sseg[basePointer - 0x0A]) = data.parm1;
        *(int16_t*)(&sseg[basePointer - 8]) = data.parm2;
    }

    if (ok)
    {
        ok = m_game->commitData(battleUnit, sizeof(*battleUnit));
        if (!ok)
        {
            m_errorString = "Could not commit battle unit data";
        }
    }

    if (ok)
    {
        ok = m_game->commitData(&sseg[basePointer - 8], 0x0C + 8);
        if (!ok)
        {
            m_errorString = "Could not commit battle unit data";
        }
    }

    std::cout << m_errorString << std::endl;

    return ok;
}

bool MoMHookManager::pdIsBaitReady()
{
    assert((0 != m_process) && (0 != m_game) && (0 != m_game->getDataSegment()));

    // Check if dseg:9296 is 1
    uint8_t* ptrBait = (uint8_t*)&m_game->getDataSegment()->m_kyrub_dseg_9296;
    bool ok = true;
    ok = m_process->readData(ptrBait, 1);
    bool baitReady = (ok && (0 != *ptrBait));

    return baitReady;
}

}
