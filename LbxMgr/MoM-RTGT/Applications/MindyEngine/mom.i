%module mom

%{
#include "MoMCommon.h"
#include "MoMCore.h"
#include "MoMExeMagic.h"
#include "MoMExeWizards.h"
#include "MoMGameBase.h"
#include "MoMProcess.h"
#include "MoMHookManager.h"
%}

// Swig wrappers for the standard C++ library
%include "cdata.i"
%include "std_string.i"

%include "cpointer.i"
//%pointer_class(uint8_t, uint8_ptr);
%pointer_class(uint16_t, uint16_ptr);
%pointer_class(uint32_t, uint32_ptr);
%pointer_class(int8_t, int8_ptr);
%pointer_class(int16_t, int16_ptr);
%pointer_class(int32_t, int32_ptr);
%pointer_class(MoM::eTerrainBonusDeposit, eTerrainBonusDeposit_ptr);
%pointer_class(MoM::eTerrainType, eTerrainType_ptr);

%inline %{
struct uint8_ptr {
	uint8_ptr(void* ptr = 0) : self((uint8_t*)ptr) {}
	~uint8_ptr() {}
	void assign(uint8_t value) {
		*self = value;
	}
	uint8_t value() {
		return *self;
	}
	uint8_t* cast() {
		return self;
	}
	static uint8_ptr* frompointer(uint8_t* ptr) {
		return new uint8_ptr(ptr);
	}

    uint16_ptr* cast_to_uint16() {
		return (uint16_ptr*)self;
	}
    uint32_ptr* cast_to_uint32() {
		return (uint32_ptr*)self;
	}
    int8_ptr* cast_to_int8() {
		return (int8_ptr*)self;
	}
    int16_ptr* cast_to_int16() {
		return (int16_ptr*)self;
	}
    int32_ptr* cast_to_int32() {
		return (int32_ptr*)self;
	}
private:
	uint8_t* self;
};
%}

%include "carrays.i"
//%array_class(uint8_t, uint8_array);
%array_class(uint16_t, uint16_array);
%array_class(uint32_t, uint32_array);
%array_class(int8_t, int8_array);
%array_class(int16_t, int16_array);
%array_class(int32_t, int32_array);
%array_class(MoM::DS_Offset, DS_Offset_array);
%array_class(MoM::eSpell, eSpell_array);
%array_class(MoM::eSpell16, eSpell16_array);

%inline %{
struct uint8_array {
	uint8_array(void* ptr) : self((uint8_t*)ptr) {}
	~uint8_array() {}
	uint8_t getitem(int index) {
		return self[index];
	}
	void setitem(int index, uint8_t value) {
		self[index] = value;
	}
	uint8_t *cast() {
		return self;
	}
	static uint8_array *frompointer(uint8_t* ptr) {
		return new uint8_array(ptr);
	}
	
    uint8_t* cast_at_index(int index) {
		return &self[index];
	}
private:
	uint8_t* self;
};
%}

// MoM interfaces
%include "MoMCommon.h"
%include "MoMTemplate.h"
%include "MoMLocation.h"
%include "MoMExeBase.h"
%include "MoMExeMagic.h"
%include "MoMExeWizards.h"
%include "MoMProcess.h"
%include "MoMGameBase.h"
%include "MoMHookManager.h"
%include "MoMCore.h"

