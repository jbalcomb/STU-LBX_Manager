from mom import *

# ----------------------------------------------------------------------------
#
# Make a connection to MoM
#
# ----------------------------------------------------------------------------

# Connect tot MoM
g = connectToMoM()
if not g.isOpen():
    print("\nFailed to find MoM")
    exit

print("\n")
print("# ---------------------------------------------------------------------")
print("#")
print("# Show the functionality of MoMGameBase")
print("#")
print("# ---------------------------------------------------------------------")
print("")

print("GameDirectory=", g.getGameDirectory())
print("Version=", g.getMoMVersion())

print("Out-of-range checks")
print("\tBattleUnit[99]=", g.getBattleUnit(99))
try:
    b = g.getBattleUnit(99)
    print("\ttactic=", b.m_Tactic)
except:
    print("\tException: could not manipulate BattleUnit[99]")

print("BattleUnit manipulations")
b = g.getBattleUnit(0)
if b == None:
    print("BattleUnit[0] not available. Skipping")
else:
    print("\tBattleUnit[0]")
    print("\ttactic=", b.m_Tactic)
    print("\tSeting tactic to shoot")
    b.m_Status = TACTIC_shoot
    ok = g.commitData(b,0x6E)
    print("\tCommit data:", ok)

print("Wizard manipulations")
wiz = g.getWizard(PLAYER_YOU)
if wiz == None:
    print("Wizard not available. Skipping")
else:
    print("\tName=", wiz.m_Name)
    print("\tGold=", wiz.m_Gold_Coins)
    print("\tIncreasing gold by 10")
    wiz.m_Gold_Coins += 10
    ok = g.commitData(wiz,0x4C8)
    print("\tCommit data:", ok)
    ok = g.readData()
    print("\tReread data:", ok)
    print("\tGold=", wiz.m_Gold_Coins)

print("City wall manipulations")
try:
    print("Create 1 broken wall of 4 pieces on the battlefield")
    f = g.getBattlefield()
    f.m_City_Walls = True
    a = uint16_array.frompointer(f.m_Wall_is_whole)
    for i in range(4):
        a[i] = 2
    ok = g.commitData(f, 0x159C)
    print("\tCommit data:", ok)
except:
    print("Failed to set the city walls")

