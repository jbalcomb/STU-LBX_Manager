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

# ----------------------------------------------------------------------------
#
# Define helper functions (that depend on the variable 'game')
#
# ----------------------------------------------------------------------------

def display_battleunits(list = range(game.getNrBattleUnits())):
    """Display the main characteristics of a list of battle units.

    in:  list   The list of battle unit ID's to enumerate
                Default all battle units are listed
    """
    for battleunit_nr in list:
        battleunit = game.getBattleUnit(battleunit_nr)
        print("battleUnitNr=", battleunit_nr)
        print("\tOwner=", battleunit.m_Owner, "\t(0=YOU, 1-4=wizards, 5=neutral)")
        print("\tActive=", battleunit.m_Active, "\t(0=alive, 4 and others=dead)")
        print("\ttactic=", battleunit.m_Tactic, "\t(0=ready, 4=done, 100=melee, 101=shoot)")
        print("\ttargetID=", battleunit.m_Target_BattleUnitID)

def display_combat_turn(prev_combat_turn = -1):
    """Display the combat turn, but only if it was changed

    in:  prev_combat_turn The previous combat turn to check if it was changed
    ret: The current combat turn
    """
    try:
        combat_turn = game.getDataSegment().m_Combat_turn
        if combat_turn != prev_combat_turn:
            print("Combat turn {}".format(combat_turn))
            prev_combat_turn = combat_turn
    except: pass
    return prev_combat_turn

def find_targetable_battleunits(attacker):
    """Find the battle units that are alive and opposing.

    in:  attacker   The attacking battle unit.
    ret: The list of battle unit ID's.
    """
    targets = []
    for battleunit_nr in range(game.getNrBattleUnits()):
        target = game.getBattleUnit(battleunit_nr)
        if (target.m_Active == BATTLEUNITACTIVE_alive
            and target.m_Owner != attacker.m_Owner
            ):
            targets.append(battleunit_nr)
    return targets

# ----------------------------------------------------------------------------
#
# AI engine
#
# ----------------------------------------------------------------------------

def run_ai_engine():
    prev_combat_turn = -1
    while True:
        ok = hook_mgr.waitForBait(0.5)  # Wait half a second between updates
        if ok: ok = game.readData()     # Update local game data if hooked
        if ok: prev_combat_turn = display_combat_turn(prev_combat_turn)
        hookdata = MoMHookData()
        if ok: ok = hook_mgr.readBaitData(hookdata)
        if ok: print("AI engine processes battleunit[{}]".format(hookdata.battleUnitNr))
        if ok: dispatch_hook(hookdata)
        if ok: ok = hook_mgr.writeBaitData(hookdata)
        if ok: ok = hook_mgr.releaseBait()

# ----------------------------------------------------------------------------
#
# AI rules (example)
#
# ----------------------------------------------------------------------------

# Make AI always attack the first attackable unit

def dispatch_hook(hookdata):
    attacker = game.getBattleUnit(hookdata.battleUnitNr)
    targets = find_targetable_battleunits(attacker)
    if targets != []:
        hookdata.targetID = targets[0]
        hookdata.tactic = TACTIC_melee
    
                                  
# ----------------------------------------------------------------------------
#
# Show an example AI
#
# ----------------------------------------------------------------------------

print("")
print("# ---------------------------------------------------------------------")
print("#")
print("# Show an example AI")
print("#")
print("# ---------------------------------------------------------------------")
print("")

print("Set up the hook")
hook_mgr = MoMHookManager(game)
print("Write the hook code into WIZARDS.EXE")
ok = hook_mgr.insertHook()
print("Hook inserted:", ok)

ok = hook_mgr.raiseHook()
print("Raise the hook:", ok)

print("\nVerify the helper functions")

is_hooked = hook_mgr.waitForBait(1.0)
print("Wait for MoM to get caught by the hook:", is_hooked)
print("This will fail if we're not in combat or if it's not AI's turn")

hookdata = MoMHookData()
ok = hook_mgr.readBaitData(hookdata)
print("Read bait data:", ok)

print("Update our local copy of the game data:", ok)
ok = game.readData()
print("\tReading game data:", ok)

print(display_battleunits.__doc__)
display_battleunits()

if is_hooked:
    battleunit = game.getBattleUnit(hookdata.battleUnitNr)
    list_targets = find_targetable_battleunits(battleunit)
    print(find_targetable_battleunits.__doc__)
    print("List targets:", list_targets)
    display_battleunits(list_targets)
                
print("\nRun the AI engine")
run_ai_engine()
