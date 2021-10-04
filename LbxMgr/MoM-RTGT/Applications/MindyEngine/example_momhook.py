from mom import *

# ----------------------------------------------------------------------------
#
# Make a connection to MoM
#
# ----------------------------------------------------------------------------

# Connect tot MoM
print("\nConnect tot MoM...")
game = connectToMoM()
if not game.isOpen():
    print("Failed to find MoM")
    exit
print("Connected")
print("GameDirectory=", game.getGameDirectory())
print("Version=", game.getMoMVersion())

print("")
print("# ---------------------------------------------------------------------")
print("#")
print("# Show the functionality of MoMHookManager")
print("#")
print("# ---------------------------------------------------------------------")
print("")

hook_mgr = MoMHookManager(game)
print("Write the hook code into WIZARDS.EXE")
ok = hook_mgr.insertHook()
print("Hook inserted:", ok)

ok = hook_mgr.raiseHook()
print("Raise the hook:", ok)

ok = hook_mgr.waitForBait(1.0)
print("Wait for MoM to get caught by the hook:", ok)
print("This will fail if we're not in combat or if it's not AI's turn")

hookdata = MoMHookData()
ok = hook_mgr.readBaitData(hookdata)
print("Read the bait data:", ok)
if ok:
    print("\tbattleUnitNr[{}]".format(hookdata.battleUnitNr))
    print("\ttargetID=", hookdata.targetID)
    print("\ttactic=", hookdata.tactic)
    print("\ttargetXpos=", hookdata.targetXpos)
    print("\ttargetYpos=", hookdata.targetYpos)
    print("\tparm1=", hookdata.parm1)
    print("\tparm2=", hookdata.parm2)

print("Set unit to attack targetID=0")
hookdata.targetID = 0
hookdata.tactic = TACTIC_melee
if ok:
    print("\tbattleUnitNr=", hookdata.battleUnitNr)
    print("\ttargetID=", hookdata.targetID)
    print("\ttactic=", hookdata.tactic)
    print("\ttargetXpos=", hookdata.targetXpos)
    print("\ttargetYpos=", hookdata.targetYpos)
    print("\tparm1=", hookdata.parm1)
    print("\tparm2=", hookdata.parm2)
ok = hook_mgr.writeBaitData(hookdata)
print("Write the bait data:", ok)

ok = hook_mgr.releaseBait()
print("Release MoM from the hook:", ok)

