#pragma once

#include "obse64_common/Types.h"

enum ExtraDataType
{
	kExtraData_Havok =					0x02,
	kExtraData_Cell3D =					0x03,
	kExtraData_WaterHeight =			0x04,
	kExtraData_CellWaterType =			0x05,
	// 06
	// 07
	kExtraData_RegionList =				0x08,
	kExtraData_SeenData =				0x09,
	kExtraData_EditorID =				0x0A,
	kExtraData_CellMusicType =			0x0B,
	kExtraData_CellClimate =			0x0C,
	kExtraData_ProcessMiddleLow =		0x0D,
	kExtraData_EditorCommonDialogData = 0x0E,
	kExtraData_CellCanopyShadowMask =	0x0F,
	kExtraData_DetachTime =				0x10,
	kExtraData_PersistentCell =			0x11,
	kExtraData_Script =					0x12,
	kExtraData_Action =					0x13,
	kExtraData_StartingPosition =		0x14,
	kExtraData_Anim =					0x15,
	// 16
	kExtraData_UsedMarkers =			0x17,
	kExtraData_DistantData =			0x18,
	kExtraData_RagDollData =			0x19,
	kExtraData_ContainerChanges =		0x1A,
	kExtraData_Worn =					0x1B,
	kExtraData_WornLeft =				0x1C,
	// 1D
	kExtraData_StartLocation =			0x1E,
	kExtraData_Package =				0x1F,
	kExtraData_TrespassPackage =		0x20,
	kExtraData_RunOncePacks =			0x21,
	kExtraData_ReferencePointer =		0x22,
	kExtraData_Follower =				0x23,
	kExtraData_LevCreaModifier =		0x24,
	kExtraData_Ghost =					0x25,
	kExtraData_OriginalReference =		0x26,
	kExtraData_Ownership =				0x27,
	kExtraData_Global =					0x28,
	kExtraData_Rank =					0x29,
	kExtraData_Count =					0x2A,
	kExtraData_Health =					0x2B,
	kExtraData_Uses =					0x2C,
	kExtraData_TimeLeft =				0x2D,
	kExtraData_Charge =					0x2E,
	kExtraData_Soul =					0x2F,
	kExtraData_Light =					0x30,
	kExtraData_Lock =					0x31,
	kExtraData_Teleport =				0x32,
	kExtraData_MapMarker =				0x33,
	// 34
	kExtraData_LeveledCreature =		0x35,
	kExtraData_LeveledItem =			0x36,
	kExtraData_Scale =					0x37,
	kExtraData_Seed =					0x38,
	kExtraData_NonActorMagicCaster =	0x39,
	kExtraData_NonActorMagicTarget =	0x3A,
	// 3B
	// 3C
	kExtraData_CrimeGold =				0x3D,
	kExtraData_OblivionEntry =			0x3E,
	kExtraData_EnableStateParent =		0x3F,
	kExtraData_EnableStateChildren =	0x40,
	kExtraData_ItemDropper =			0x41,
	kExtraData_DroppedItemList =		0x42,
	kExtraData_RandomTeleportMarker =	0x43,
	kExtraData_MerchantContainer =		0x44,
	// 45
	kExtraData_PersuasionPercent =		0x46,
	kExtraData_CannotWear =				0x47,
	kExtraData_Poison =					0x48,
	// 49
	kExtraData_LastFinishedSequence =	0x4A,
	kExtraData_SavedMovementData =		0x4B,
	kExtraData_NorthRotation =			0x4C,
	kExtraData_XTarget =				0x4D,
	kExtraData_FriendHitList =			0x4E,
	kExtraData_HeadingTarget =			0x4F,
	kExtraData_BoundArmor =				0x50,
	kExtraData_RefractionProperty =		0x51,
	kExtraData_InvestmentGold =			0x52,
	kExtraData_StartingWorldOrCell =	0x53,
	// 54
	kExtraData_QuickKey =				0x55,
	// 56
	kExtraData_EditorRefMoveData =		0x57,
	kExtraData_TravelHorse =			0x58,
	kExtraData_InfoGeneralTopic =		0x59,
	kExtraData_HasNoRumors =			0x5A,
	kExtraData_ExtraSound =				0x5B,
	kExtraData_HaggleAmount =			0x5C,

	// added for altar
	kExtraData_AltarBPVariant =			0x5D,	// ExtraAltarBPVariant
	kExtraData_AltarActorGUID =			0x5E,	// ExtraAltarActorGUID
	kExtraData_AltarCellName =			0x5F,	// VExtraAltarCellName
	kExtraData_AltarData =				0x60,	// ExtraAltarData
	kExtraData_AltarThreatLevel =		0x61,	// VExtraAltarThreatLevel
	kExtraData_AltarBigWorldLeveledRgn =0x62,	// VExtraAltarBigWorldLeveledRegion

	// for size of presence list
	kExtraData_Max =					0x80,
};

// 18
class BSExtraData
{
public:
	virtual ~BSExtraData();

	virtual bool	Compare(BSExtraData * rhs);

//	void		** _vtbl;	// 00
	u8			m_type;		// 08
	u8			pad05[7];	// 09 lol
	BSExtraData	* m_next;	// 10
};

static_assert(sizeof(BSExtraData) == 0x18);

// 20
class BaseExtraList
{
public:
	virtual ~BaseExtraList();

//	void		** _vtbl;	// 00
	BSExtraData	* m_data;	// 08

	u8	m_presence[kExtraData_Max / 8];	// 10 starts from lsb, larger than original

	static void Lock();
	static void Unlock();

	template <typename T>
	T * Get() const
	{
		return static_cast<T *>(Get(T::kID));
	}

	BSExtraData * Get(u8 type) const;
	bool Contains(u8 type) const;

	class Locker
	{
	public:
		Locker() { BaseExtraList::Lock(); }
		~Locker() { BaseExtraList::Unlock(); }
	};
};

static_assert(sizeof(BaseExtraList) == 0x20);

// 20
struct ExtraDataList : public BaseExtraList
{
};

static_assert(sizeof(ExtraDataList) == 0x20);
