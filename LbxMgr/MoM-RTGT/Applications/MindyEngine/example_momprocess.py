from mom import *

# ----------------------------------------------------------------------------
#
# Make a connection to MoM
#
# ----------------------------------------------------------------------------

# Connect tot MoM
print("\nConnecting to MoM...")
g = connectToMoM()
try:
    print("\tConnected to MoM:", g.isOpen())
except:
    raise SystemExit("Failed to find MoM")

print("# ---------------------------------------------------------------------")
print("#")
print("# Show the functionality of the low level interface MoMProcess")
print("#")
print("# ---------------------------------------------------------------------")

# Get a handle on the MoMProcess object
p = g.getMoMProcess()

# Show what you can do with the MoMProcess object
exeFilepath = p.getExeFilepath()
print("\nExeFilepath=", exeFilepath)

print("\nSome gory details")
print("\tBaseAddress=", p.getBaseAddress())
print("\tBaseAddressSize=", p.getBaseAddressSize())
print("\tDatasegmentData=", p.getDatasegmentData())
print("\tBaseAddress=", p.getBaseAddress())
print("\tOffsetSegment0=", p.getOffsetSegment0())
print("\tOffsetCode=", p.getOffsetCode())
print("\tOffsetDatasegment=", p.getOffsetDatasegment())
print("\tProcessHandle=", p.getProcessHandle())
print("\tSeg0Pointer=", p.getSeg0Pointer())
print("\tOffset_DS_Code=", p.getOffset_DS_Code())

print("\nManipulating data")
dseg = uint8_array.frompointer(p.getDatasegmentData())

# Copyright_and_Version[41] is at ds:7151  Offset version is at [34]
version = cdata(dseg.cast_at_index(0x7151 + 34), 6)
print("\tMoMVersion=", version)

# Gold reserve is a word at ds:A220
gold_ptr = uint8_ptr(dseg.cast_at_index(0xA220)).cast_to_int16()
gold = gold_ptr.value()
print("\tGold=", gold)
gold += 10
gold_ptr.assign(gold)
ok = p.writeData(gold_ptr.cast(), 2)
print("\tGold ", gold, " written with result", ok)
gold_ptr.assign(0)
ok = p.readData()
print("\tReread data with result", ok)
gold_ptr = uint8_ptr(dseg.cast_at_index(0xA220)).cast_to_int16()
print("\tGold=", gold_ptr.value())

print("\nValidate pointers");
null_ptr = uint8_ptr()
print("\tNull pointer is not valid: valid=", p.validPointer(null_ptr, 2))
gold_ptr = uint8_ptr(dseg.cast_at_index(0xA220)).cast_to_int16()
print("\tGold pointer is valid: valid=", p.validPointer(gold_ptr, 2))

print("\nDerefence pointers");

dsOffset = uint8_ptr(dseg.cast_at_index(0x0130)).cast_to_uint16().value()
print("\tDS_Offset of the first skill is", dsOffset)
print("\tCorresponding name is:", cdata(dseg.cast_at_index(dsOffset), 7))

exeReloc = g.getDataSegment().m_addr_Spell_Data
print("\tEXE_Reloc of spell data is segment", exeReloc.segment, "offset", exeReloc.offset)
spellData = p.derefPointer(exeReloc, 19)
print("\tSpell name of the None spell is:", cdata(spellData, 4))

file = "test.dmp"
print("\nSaving to ", file);
if p.save(file):
    print("Save successful");
else:
    print("Save failed");
