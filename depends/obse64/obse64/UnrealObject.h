#pragma once

#include "UnrealTypes.h"
#include "obse64_common/Relocation.h"

// object and RTTI system

class UClass;
class UPackage;
class UFunction;

// 28
class UObject
{
public:
	virtual ~UObject();

//	void	** _vtbl;		// 00
	u32		m_flags;		// 08
	u32		m_objArrayIdx;	// 0C - index in to global object array
	UClass	* m_type;		// 10
	FName	m_name;			// 18
	UObject	* m_outer;		// 20 - parent for purposes of package ownership
};

static_assert(sizeof(UObject) == 0x28);

// 18
class FUObjectItem
{
public:
	UObject	* m_obj;	// 00
	u32		m_flags;	// 08
	u32		m_cluster;	// 0C
	u32		m_serial;	// 10
	u32		pad14;		// 14
};

static_assert(sizeof(FUObjectItem) == 0x18);

// 20
class FChunkedFixedUObjectArray
{
public:
	enum
	{
		kChunkSize = 0x10000,
	};

	FUObjectItem	** m_chunks;	// 00
	void			* unk08;		// 08
	u32				m_objSize;		// 10 objects allocated
	u32				m_objLen;		// 14 objects used
	u32				m_chunkSize;	// 18 chunks allocated
	u32				m_chunkLen;		// 1C chunks used
};

static_assert(sizeof(FChunkedFixedUObjectArray) == 0x20);

// B8
class FUObjectArray
{
public:
	u32		unk00;					// 00
	u32		unk04;					// 04 init'd to FFFFFFFF
	u32		unk08;					// 08
	u8		unk0C;					// 0C
	u8		pad0D[3];				// 0D

	FChunkedFixedUObjectArray	m_objects;	// 10

	u8				m_lock[0x28];	// 30 CRITICAL_SECTION around m_objects
	TArray <u32>	unk58;			// 58
	TArray <void *>	unk68;			// 68
	TArray <void *>	unk78;			// 78
	u8				m_lock2[0x28];	// 88 CRITICAL_SECTION
	u32				m_serial;		// B0
	u8				unkB4;			// B4
	u8				padB5[3];		// B5

	void Lock();
	void Unlock();
};

static_assert(sizeof(FUObjectArray) == 0xB8);

extern RelocPtr <FUObjectArray> g_uObjectArray;

// 30
class UField : public UObject
{
public:
	virtual ~UField();

	UField	* m_next;	// 28
};

static_assert(sizeof(UField) == 0x30);

// B0
class UStruct : public UField
{
public:
	virtual ~UStruct();

	void	* unk30;	// 30
	u32		unk38;		// 38
	u32		unk3C;		// 3C

	UStruct	* m_parent;	// 40
	UField	* m_fields;	// 48
	UField	* m_field2;	// 50

	u32		unk58;		// 58
	u32		unk5C;		// 5C

	TArray <u8>	unk60;	// 60
	void	* unk70;	// 70
	void	* unk78;	// 78
	void	* unk80;	// 80
	void	* unk88;	// 88
	TArray <u8>	unk90;	// 90
	void	* unkA0;	// A0
	void	* unkA8;	// A8
};

static_assert(sizeof(UStruct) == 0xB0);

// 200
class UClass : public UStruct
{
public:
	virtual ~UClass();

	struct NativeFunction
	{
		FName	name;
		void	* fn;
	};

	void	* m_constructor;	// 0B0
	void	* m_constructor2;	// 0B8
	void	* m_refCollector;	// 0C0
	u32		unk0C8;				// 0C8
	u32		unk0CC;				// 0CC
	u16		unk0D0;				// 0D0
	u16		pad0D2;				// 0D2
	u32		unk0D4;				// 0D4
	u64		unk0D8;				// 0D8
	UClass	* unk0E0;			// 0E0
	FName	unk0E8;				// 0E8 - "Engine" or "Game"
	TArray <void *>	unk0F0;		// 0F0
	TArray <void *>	unk100;		// 100
	UObject	* m_cdo;			// 110
	void	* unk118;			// 118
	void	* unk120;			// 120
	TMap <FName, UFunction *>	m_functions;	// 128
	void	* m_fnLock;			// 178 SRWLOCK
	TMap <FName, UFunction *>	m_functions2;	// 180
	void	* m_fnLock2;		// 188 SRWLOCK
	TArray <void *>	unk1D8;		// 1D8
	u64		unk1E8;				// 1E8
	TArray <NativeFunction>		m_nativeFunctions;	// 1F0
};

static_assert(sizeof(UClass) == 0x200);

// 260
class FUObjectHashTable
{
public:
	u8	m_lock[0x28];	// 00 CRITICAL_SECTION (entire thing?)

	// 10
	struct ObjList
	{
		// todo

		u64	unk0;
		u64	unk8;
	};

	TMap <u32, ObjList>				unk028;				// 028
	TMap <u32, u32>					unk078;				// 078
	TMap <UObject *, ObjList>		m_outers;			// 0C8
	TMap <UClass *, ObjList>		m_objectsOfType;	// 118
	TMap <UClass *, void *>			m_typeHierarchy;	// 168
	u64								unk1B8;				// 1B8
	TMap <UPackage *, ObjList>		unk1C0;				// 1C0
	TMap <UObject *, UPackage *>	m_objToPackage;		// 210

	void Lock();
	void Unlock();
};

static_assert(sizeof(FUObjectHashTable) == 0x260);

extern RelocPtr <FUObjectHashTable> g_uObjectHashTable;
