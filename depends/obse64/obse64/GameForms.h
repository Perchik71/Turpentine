#pragma once

#include "GameTypes.h"
#include "AltarSync.h"
#include "GameBSExtraData.h"

class TESForm;
class TESFile;
class TESObjectCELL;
class TESObjectLAND;
class TESObjectREFR;
class TESObject;
class TESSound;
class TESPathGrid;
class TESWorldSpace;
class Script;
class EnchantmentItem;
class NiNode;
class ScriptEffectInfo;
class EffectSetting;

const char * GetFullName(TESForm * baseForm);

typedef TESForm * (* _LookupFormByID)(u32 id);
extern RelocAddr <_LookupFormByID> LookupFormByID;

// 08
class BaseFormComponent
{
public:
	BaseFormComponent();
	~BaseFormComponent();	// non-virtual dtor

	virtual void	Unk_00();
	virtual void	Unk_01();
	virtual void	CopyFromBase(BaseFormComponent * src);
	virtual bool	CompareBase(BaseFormComponent * src);

//	void	** _vtbl;	// 00
};

static_assert(sizeof(BaseFormComponent) == 0x08);

// 18
class TESFullName : public BaseFormComponent
{
public:
	BSStringT	name;	// 08
};

static_assert(sizeof(TESFullName) == 0x18);

// 18
class TESScriptableForm : public BaseFormComponent
{
public:
	Script	* script;	// 8
	u8		unk10;		// 10
	u8		pad11[7];	// 11
};

static_assert(sizeof(TESScriptableForm) == 0x18);

// 10
class TESWeightForm : public BaseFormComponent
{
public:
	float	weight;	// 08
};

static_assert(sizeof(TESWeightForm) == 0x10);

// 10
class TESValueForm : public BaseFormComponent
{
public:
	u32	value;	// 08
};

static_assert(sizeof(TESValueForm) == 0x10);

// 18
class TESEnchantableForm : public BaseFormComponent
{
public:
	EnchantmentItem	* enchantItem;	// 08
	u16				enchantment;	// 10 max charge, valid only for weapons
	u16				pad12;			// 12
	u32				castingType;	// 14
};

static_assert(sizeof(TESEnchantableForm) == 0x18);

// 10
class TESHealthForm : public BaseFormComponent
{
public:
	u32	health;	// 8
};

static_assert(sizeof(TESHealthForm) == 0x10);

// 10
class TESAttackDamageForm : public BaseFormComponent
{
public:
	u16	damage;		// 8
	u8	padA[6];	// A
};

static_assert(sizeof(TESAttackDamageForm) == 0x10);

enum FormType
{
	kFormType_None = 0,
	kFormType_TES4,
	kFormType_Group,
	kFormType_GMST,
	kFormType_Global,
	kFormType_Class,
	kFormType_Faction,
	kFormType_Hair,
	kFormType_Eyes,
	kFormType_Race,
	kFormType_Sound,
	kFormType_Skill,
	kFormType_Effect,
	kFormType_Script,
	kFormType_LandTexture,
	kFormType_Enchantment,
	kFormType_Spell,		// 10
	kFormType_BirthSign,
	kFormType_Activator,
	kFormType_Apparatus,
	kFormType_Armor,
	kFormType_Book,
	kFormType_Clothing,
	kFormType_Container,
	kFormType_Door,
	kFormType_Ingredient,
	kFormType_Light,
	kFormType_Misc,
	kFormType_Stat,	// ???
	kFormType_Grass,
	kFormType_Tree,
	kFormType_Flora,
	kFormType_Furniture,	// 20
	kFormType_Weapon,
	kFormType_Ammo,
	kFormType_NPC,
	kFormType_Creature,
	kFormType_LeveledCreature,
	kFormType_SoulGem,
	kFormType_Key,
	kFormType_AlchemyItem,
	kFormType_SubSpace,
	kFormType_SigilStone,
	kFormType_LeveledItem,
	kFormType_SNDG,
	kFormType_Weather,
	kFormType_Climate,
	kFormType_Region,
	kFormType_Cell,			// 30
	kFormType_REFR,
	kFormType_ACHR,
	kFormType_ACRE,
	kFormType_PathGrid,
	kFormType_WorldSpace,
	kFormType_Land,
	kFormType_TLOD,	// tile low lod?
	kFormType_Road,
	kFormType_Dialog,
	kFormType_DialogInfo,
	kFormType_Quest,
	kFormType_Idle,
	kFormType_Package,
	kFormType_CombatStyle,
	kFormType_LoadScreen,
	kFormType_LeveledSpell,	// 40
	kFormType_ANIO,
	kFormType_WaterForm,
	kFormType_EffectShader,
	kFormType_TOFT
};

// 30
class TESForm : public BaseFormComponent
{
public:
	TESForm();

	virtual ~TESForm();	// 04
	virtual void Unk_05();
	virtual void Unk_06();
	virtual void Unk_07();
	virtual void Unk_08();
	virtual void Save();
	virtual void Unk_0A();
	virtual void Unk_0B();
	virtual void Unk_0C();
	virtual void Unk_0D();
	virtual void Unk_0E();
	virtual void Unk_0F();
	virtual void MarkAsModified(u32 mask);
	virtual void ClearModified(u32 mask);
	virtual void Unk_12();
	virtual u32 GetSaveSize(u32 mask);
	virtual void SaveGame(u32 mask);
	virtual void LoadGame(u32 mask);
	virtual void Unk_16();
	virtual void Unk_17();
	virtual void Unk_18();
	virtual void Unk_19();
	virtual void Unk_1A();
	virtual void Unk_1B();
	virtual void Unk_1C();
	virtual void Unk_1D();
	virtual void Unk_1E();
	virtual void Unk_1F();
	virtual void Unk_20();
	virtual void Unk_21();
	virtual void Unk_22();
	virtual void Unk_23();
	virtual void Unk_24();
	virtual void Unk_25();
	virtual void Unk_26();
	virtual void Unk_27();
	virtual void Unk_28();
	virtual void Unk_29();
	virtual void Unk_2A();
	virtual void Unk_2B();
	virtual void Unk_2C();
	virtual void Unk_2D();
	virtual void Unk_2E();
	virtual void Unk_2F();
	virtual void Unk_30();
	virtual void Unk_31();
	virtual void Unk_32();
	virtual void Unk_33();
	virtual void Unk_34();
	virtual void Unk_35();
	virtual void Unk_36();
	virtual void Unk_37();
	virtual void Unk_38();
	virtual void Unk_39();
	virtual void Unk_3A();
	virtual void Unk_3B();

	u8	typeID;		// 08
	u8	pad09[3];	// 09
	u32	flags;		// 0C
	u32	refID;		// 10

	BSSimpleList <TESFile *>	modRefList;	// 18

	void	* unk28;	// 28 new in altar

	TESFullName * GetFullName();
};

static_assert(sizeof(TESForm) == 0x30);

// 20
class ObjectListHead
{
	u32	numEntries;				// 00
	u32	pad04;					// 04
	TESObject		* start;	// 08
	TESObject		* end;		// 10
	TESObjectCELL	* parent;	// 18
};

static_assert(sizeof(ObjectListHead) == 0x20);

// 48
class TESObject : public TESForm
{
public:
	TESObject();
	virtual ~TESObject();

	ObjectListHead	* objectListHead;	// 30
	TESObject		* objectListPrev;	// 38
	TESObject		* objectListNext;	// 40
};

static_assert(sizeof(TESObject) == 0x48);

// 48
class TESBoundObject : public TESObject
{
public:
	TESBoundObject();
	virtual ~TESBoundObject();
};

static_assert(sizeof(TESBoundObject) == 0x48);

// 48
class TESBoundAnimObject : public TESBoundObject
{
public:
	virtual ~TESBoundAnimObject();

	// size limited by TESSound
};

static_assert(sizeof(TESBoundAnimObject) == 0x48);

// 20
class TESContainer : public BaseFormComponent
{
public:
	struct Entry
	{
		s32 count;
		TESForm * type;
	};

	u8	type;		// 08
	u8	pad09[7];	// 09

	BSSimpleList <Entry *> list;	// 10
};

static_assert(sizeof(TESContainer) == 0x20);

// 20
class TESLeveledList : public BaseFormComponent
{
public:
	enum
	{
		kFlags_CalcAllLevels = 1,
		kFlags_CalcEachInCount = 2,
	};

	// 18
	struct Entry
	{
		u16	level;		// 00
		u8	pad02[6];	// 02
		TESForm	* form;	// 08
		u16	count;		// 10
		u8	pad12[6];	// 12
	};

	BSSimpleList <Entry *> list;	// 08
	u8	chanceNone;	// 18
	u8	flags;		// 19
	u8	pad1A[6];	// 1A
};

static_assert(sizeof(TESLeveledList) == 0x20);
static_assert(sizeof(TESLeveledList::Entry) == 0x18);

// 68
class TESLevItem : public TESBoundObject
{
public:
	virtual ~TESLevItem();

	TESLeveledList	leveledList;	// 48
};

static_assert(sizeof(TESLevItem) == 0x68);

// 30
class TESModel : public BaseFormComponent
{
public:
	BSStringT	nifPath;	// 08
	float		editorSize;	// 18
	u32			pad1C;		// 1C
	u8			unk20;		// 20
	u8			pad21[7];	// 21
	void		* unk28;	// 28
};

static_assert(sizeof(TESModel) == 0x30);

// 18
class TESTexture : public BaseFormComponent
{
public:
	BSStringT	ddsPath;	// 8
};

static_assert(sizeof(TESTexture) == 0x18);

// 18
class TESIcon : public TESTexture
{
public:
};

static_assert(sizeof(TESIcon) == 0x18);

// 100
class TESBipedModelForm : public BaseFormComponent
{
public:
	static u32 SlotForMask(u32 mask);
	static u32 MaskForSlot(u32 slot);

	// bit indices starting from lsb
	enum
	{
		kPart_Head = 0,
		kPart_Hair,
		kPart_UpperBody,
		kPart_LowerBody,
		kPart_Hand,
		kPart_Foot,
		kPart_RightRing,
		kPart_LeftRing,
		kPart_Amulet,
		kPart_Weapon,
		kPart_BackWeapon,
		kPart_SideWeapon,
		kPart_Quiver,
		kPart_Shield,
		kPart_Torch,
		kPart_Tail,

		kPart_Max
	};
	enum
	{
		kFlags_HidesRings = 0,
		kFlags_HidesAmulets,
		kFlags_Unk2,
		kFlags_Unk3,
		kFlags_Unk4,
		kFlags_Unk5,
		kFlags_NotPlayable,
		kFlags_HeavyArmor,
		kFlags_Unk8,
	};

	enum {
		kPath_Biped,
		kPath_Ground,
		kPath_Icon,
		kPath_Max
	};

	u16		partMask;			// 08
	u8		flags;				// 0A
	u8		pad0B[5];			// 0B

	// male and female models and icons
	TESModel	bipedModel[2];		// 10
	TESModel	groundModel[2];		// 70
	TESIcon		icon[2];			// D0

	u32 GetSlot() const { return SlotForMask(partMask); }
};

static_assert(sizeof(TESBipedModelForm) == 0x100);

// 108
class TESObjectLIGH : public TESBoundAnimObject
{
public:
	enum
	{
		kLightFlags_Dynamic =		0x001,
		kLightFlags_CanCarry =		0x002,
		kLightFlags_Negative =		0x004,
		kLightFlags_Flicker =		0x008,
		kLightFlags_OffByDefault =	0x020,
		kLightFlags_FlickerSlow =	0x040,
		kLightFlags_Pulse =			0x080,
		kLightFlags_PulseSlow =		0x100,
		kLightFlags_SpotLight =		0x200,
		kLightFlags_SpotShadow =	0x400
	};

	TESFullName			fullName;	// 48
	TESModel			model;		// 60
	TESIcon				icon;		// 90
	TESScriptableForm	scriptable;	// A8
	TESWeightForm		weight;		// C0
	TESValueForm		value;		// D0

	u32			time;			// E0
	u32			radius;			// E4
	u32			colorRGB;		// E8
	u32			lightFlags;		// EC
	float		falloff;		// F0
	float		fov;			// F4
	float		fade;			// F8
	u32			padFC;			// FC
	TESSound	* loopSound;	// 100

	bool IsCarriable() const
	{
		return lightFlags & kLightFlags_CanCarry;
	}
};

static_assert(sizeof(TESObjectLIGH) == 0x108);

// 128
class TESObjectWEAP : public TESBoundAnimObject
{
public:
	enum
	{
		kType_BladeOneHand = 0,
		kType_BladeTwoHand,
		kType_BluntOneHand,
		kType_BluntTwoHand,
		kType_Staff,
		kType_Bow,

		kType_Max,
	};

	TESFullName			fullName;		// 048
	TESModel			model;			// 060
	TESIcon				icon;			// 090
	TESScriptableForm	scriptable;		// 0A8
	TESEnchantableForm	enchantable;	// 0C0
	TESValueForm		value;			// 0D8
	TESWeightForm		weight;			// 0E8
	TESHealthForm		health;			// 0F8
	TESAttackDamageForm	attackDmg;		// 108

	u8		type;		// 118
	float	speed;		// 11C
	float	reach;		// 120
	u8		flags;		// 124
	u8		pad125[3];	// 125
};

static_assert(sizeof(TESObjectWEAP) == 0x128);

// D8
class TESObjectCELL : public TESForm
{
public:
	struct CellCoordinates;
	struct LightingData;

	TESFullName	fullName;		// 30
	IVPairableItem pairable;	// 48

	// new in altar
	u8	unk60;		// 60
	u8	pad61[7];	// 61
	BSSimpleList <TESForm *>	unk68;	// 68
	u8	unk78;		// 78
	// end new in altar

	u8	flags0;		// 79
	u8	flags1;		// 7A
	u8	flags2;		// 7B
	u32	pad7C;		// 7C

	ExtraDataList	extraData;		// 80
	union {
		CellCoordinates * coords;	// if exterior
		LightingData * lighting;	// if interior
	};								// A0
	TESObjectLAND	* land;			// A8
	TESPathGrid		* pathGrid;		// B0

	BSSimpleList <TESObjectREFR *>	objectList;	// B8

	TESWorldSpace	* worldSpace;	// C8
	NiNode			* unkD0;		// D0
};

static_assert(sizeof(TESObjectCELL) == 0xD8);

// 38
class EffectItem
{
public:
	enum
	{
		kRange_Self = 0,
		kRange_Touch,
		kRange_Target,
	};

	virtual ~EffectItem();

	virtual void	Unk_01();
	virtual void	Unk_02();

//	void	** _vtbl;	// 00
	u32	effectCode;	// 08
	u32	magnitude;	// 0C
	u32	area;		// 10
	u32	duration;	// 14
	u32	range;		// 18
	u32	actorValue;	// 1C

	ScriptEffectInfo	* scriptEffectInfo;	// 20
	EffectSetting		* setting;			// 28

	float	cost;	// 30
};

// 20
class EffectItemList
{
public:
	virtual void	Unk_00();
	virtual void	Unk_01();
	virtual void	Unk_02();

//	void	** _vtbl;	// 00
	BSSimpleList <EffectItem *>	effects;	// 08
	u32	numHostileEffects;	// 18
	u32	pad1C;				// 1C
};

static_assert(sizeof(EffectItemList) == 0x20);

// 60
class MagicItem : public TESFullName
{
public:
	EffectItemList	list;	// 18

	// new in altar
	u64	unk38[(0x60 - 0x38) / 8];	// 38
};

static_assert(sizeof(MagicItem) == 0x60);

// 90
class MagicItemForm : public TESForm
{
public:
	virtual ~MagicItemForm();

	MagicItem	magicItem;	// 30
};

static_assert(sizeof(MagicItemForm) == 0x90);

// A0
class SpellItem : public MagicItemForm
{
public:
	enum
	{
		kType_Spell = 0,
		kType_Disease,
		kType_Power,
		kType_LesserPower,
		kType_Ability,
		kType_All,
	};

	enum
	{
		kLevel_Novice = 0,
		kLevel_Apprentice,
		kLevel_Journeyman,
		kLevel_Expert,
		kLevel_Master,
	};

	enum
	{
		kFlag_NoAutoCalc = 0x1,
		kFlag_PCStart = 0x4,
		kFlag_ImmuneToSilence = 0xA,	// ### 2 bits?
		kFlag_AreaEffectIgnoresLOS = 0x10,
		kFlag_ScriptEffectAlwaysApplies = 0x20,
		kFlag_DisallowAbsorbReflect = 0x40,
		kFlag_TouchExplodesWithNoTarget = 0x80,
	};

	u32	spellType;		// 90
	u32	magickaCost;	// 94
	u32	masteryLevel;	// 98
	u8	spellFlags;		// 9C
	u8	pad9D[3];		// 9D
};

static_assert(sizeof(SpellItem) == 0xA0);
