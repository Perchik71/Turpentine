#pragma once

#include "GameBSExtraData.h"
#include "STLTypes.h"
#include "AltarSync.h"

// 28
class ExtraAltarActorGUID: public BSExtraData
{
public:
	enum { kID = kExtraData_AltarActorGUID };

	VAltarActorGUID	m_guid;	// 18
};

static_assert(sizeof(ExtraAltarActorGUID) == 0x28);

// 20
class ExtraAltarBPVariant: public BSExtraData
{
public:
	enum { kID = kExtraData_AltarActorGUID };

	u32	m_variantIdx;	// 18
	u32	pad1C;			// 1C
};

static_assert(sizeof(ExtraAltarBPVariant) == 0x20);

// 20
class ExtraAltarData: public BSExtraData
{
public:
	enum { kID = kExtraData_AltarData };

	void * unk18;	// 18 passed to delete, no dtor needed
};

static_assert(sizeof(ExtraAltarData) == 0x20);

// 98
class VExtraAltarCellName: public BSExtraData
{
public:
	enum { kID = kExtraData_AltarCellName };

	char	m_name[0x80];	// 18
};

static_assert(sizeof(VExtraAltarCellName) == 0x98);

// 20
class VExtraAltarThreatLevel: public BSExtraData
{
public:
	enum { kID = kExtraData_AltarThreatLevel };

	u32	m_threatLevel;	// 18
};

static_assert(sizeof(VExtraAltarThreatLevel) == 0x20);

// 30
class VExtraAltarBigWorldLeveledRegion: public BSExtraData
{
public:
	enum { kID = kExtraData_AltarBigWorldLeveledRgn };

	// 14
	struct Entry
	{
		u32 unk00;
		u32 unk04;
		u32 unk08;
		u32 unk0C;
		u32 unk10;
	};

	// pointer-based container, not unreal or netimmerse. matches stl
	STLVector <Entry> a;	// 18
};

static_assert(sizeof(VExtraAltarBigWorldLeveledRegion) == 0x30);
