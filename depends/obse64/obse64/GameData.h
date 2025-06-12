#pragma once

#include "GameTypes.h"
#include "NiTypes.h"
#include "obse64_common/Utilities.h"

// form data and io

class TESForm;
class TESObject;
class TESObjectCELL;

class TESRegionList;
class TESRegionDataManager;

// 
class TESFile
{
public:
};

// 20
struct TESObjectListHead
{
	// linked list of everything?

	u32			count;	// 00
	u32			pad04;	// 04
	TESObject	* first;	// 08
	TESObject	* last;	// 10
	void *		unk18;	// 18
};

// 1860
class TESDataHandler
{
public:
	TESObjectListHead	* allObjects;			// 0000

	// todo: check types
	BSSimpleList <TESForm *>	formList0;	// 0008
	BSSimpleList <TESForm *>	formList1;	// 0018
	BSSimpleList <TESForm *>	formList2;	// 0028
	BSSimpleList <TESForm *>	formList3;	// 0038
	BSSimpleList <TESForm *>	formList4;	// 0048
	BSSimpleList <TESForm *>	formList5;	// 0058
	BSSimpleList <TESForm *>	formList6;	// 0068
	BSSimpleList <TESForm *>	formList7;	// 0078
	BSSimpleList <TESForm *>	formList8;	// 0088
	BSSimpleList <TESForm *>	formList9;	// 0098
	BSSimpleList <TESForm *>	formList10;	// 00A8
	BSSimpleList <TESForm *>	formList11;	// 00B8
	BSSimpleList <TESForm *>	formList12;	// 00C8
	BSSimpleList <TESForm *>	formList13;	// 00D8
	BSSimpleList <TESForm *>	formList14;	// 00E8
	BSSimpleList <TESForm *>	formList15;	// 00F8
	BSSimpleList <TESForm *>	formList16;	// 0108
	BSSimpleList <TESForm *>	formList17;	// 0118
	BSSimpleList <TESForm *>	formList18;	// 0128
	BSSimpleList <TESForm *>	formList19;	// 0138
	BSSimpleList <TESForm *>	formList20;	// 0148
	BSSimpleList <TESForm *>	formList21;	// 0158
	BSSimpleList <TESForm *>	formList22;	// 0168
	
	TESRegionList	* regionList;	// 0178

	NiTLargeArray <TESObjectCELL *>	interiorCells;	// 0180

	u8	skills[0xB0 * 21];	// 01A0 - TESSkill array

	BSSimpleList <TESForm *>	formList1010;	// 1010
	u32	nextFormID;								// 1020
	u32	pad1024;								// 1024

	void						* unk1028;				// 1028
	BSSimpleList <TESFile *>	modList;				// 1030
	u32							unk1040;				// 1040
	u32							pad1044;				// 1044
	TESFile						* modsByID[0xFF];		// 1048
	u8							unk1840[8];				// 1840 - u8 flags?
	TESRegionDataManager		* regionDataManager;	// 1848
	void						* unk1850;				// 1850
	u8							unk1858;				// 1858
	u8							pad1859[7];				// 1859

	static TESDataHandler * GetSingleton();

	MEMBER_FN_PREFIX(TESDataHandler);
	DEFINE_MEMBER_FN(UnkInit, void, 0x0662B9C0);
};

static_assert(sizeof(TESDataHandler) == 0x1860);
