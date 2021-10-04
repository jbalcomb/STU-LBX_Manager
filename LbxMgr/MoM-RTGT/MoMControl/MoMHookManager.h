#ifndef MOMHOOKMANAGER_H
#define MOMHOOKMANAGER_H

#include <string>

namespace MoM {

class MoMGameBase;
class MoMProcess;

// This structure is defined outside the MoMHookManager
// because SWIG currently does not support nested structures.
struct MoMHookData
{
    int battleUnitNr;
    int targetID;
    int tactic;
    int targetXpos;
    int targetYpos;
    int parm1;
    int parm2;
};

class MoMHookManager
{
public:
    explicit MoMHookManager(MoMGameBase* game);

    std::string errorString() const
    {
        return m_errorString;
    }

    /// Write the hook code into WIZARDS.EXE
    bool insertHook();

    /// Restore the original code in WIZARDS.EXE
    /// NOT IMPLEMENTED
    void removeHook();

    /// Make MoM stop when it encounters the hook.
    bool raiseHook();

    /// Make MoM ignore the hook.
    bool retractHook();

    /// Wait for MoM to trigger the hook.
    /// \retval true if the hook was triggered
    /// \retval false if the connection is broken or a timeout occurred
    bool waitForBait(double timeout);

    /// Let MoM continue from the hook.
    bool releaseBait();

    /// Check if MoM is currently hooked
    bool isBaitReady();

    /// Read the bait data
    /// \pre The bait is currently hooked
    /// \param[out] data The relevant data for this hook
    bool readBaitData(MoMHookData& data);

    /// Write the bait data
    /// \pre The bait is currently hooked
    /// \param[in] data The relevant data for this hook
    bool writeBaitData(const MoMHookData& data);

private:
    bool pdIsBaitReady();

    MoMGameBase* m_game;
    MoMProcess*  m_process;
    std::string  m_errorString;
};

}

#endif // MOMHOOKMANAGER_H
