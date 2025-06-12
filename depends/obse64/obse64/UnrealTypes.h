#pragma once

#include "obse64_common/Types.h"
#include "obse64_common/Relocation.h"

// base types/containers

// 10
template <typename T>
class TArray
{
public:
	T	* m_data;	// 0
	u32	m_len;		// 8 valid elements
	u32	m_size;		// C size of buffer (elements)

	// todo: allocator
};

static_assert(sizeof(TArray <u8>) == 0x10);

// 8
class FName
{
public:
	u32		m_idx;	// 0 - index in to global name table
	u32		m_num;	// 4 - unreal names have a number so you can have multiple instances
};

static_assert(sizeof(FName) == 8);

// 10
class FString
{
public:
	TArray <wchar_t>	m_data;
};

static_assert(sizeof(FString) == 0x10);

// size represents max allocation, actual instances are as short as possible
class FNameEntry
{
public:
	enum
	{
		kWide =				0x0001,	// selects between char and wchar_t
		kUnkMask =			0x003E,	// ?
		kUnkMaskShift =		1,		// ?
		kLengthMask =		0xFFC0,	// length in character units
		kLengthMaskShift =	6,		// 

		kMaxLen = 1024,
	};

	u16	header;	// 00

	union
	{
		char	str[kMaxLen];	// underallocated
		wchar_t	wstr[kMaxLen];
	};

	bool Wide() const { return header & kWide; }
	u8 GetUnk() const { return (header & kUnkMask) >> kUnkMaskShift; }
	u16 Length() const { return (header & kLengthMask) >> kLengthMaskShift; }
};

static_assert(sizeof(FNameEntry) == 2 + (1024 * 2));

// statically allocated
class FNameStorage
{
public:
	FNameEntry * Get(u32 idx);
	FNameEntry * Get(const FName & fname);

	void		* m_lock;			// 00 - SRWLOCK
	u32			m_curChunk;			// 08
	u32			m_curOffset;		// 0C
	FNameEntry	* m_chunks[8192];	// 10

	// more stuff I don't care about right now
};

extern RelocPtr <FNameStorage> g_fnameStorage;

// 20
class TBitArray
{
public:
	u32	m_data[4];		// 00 bits allocated in u32 blocks
	u32	* m_dataBuf;	// 10 null if m_data is used instead
	u32	m_len;			// 18 valid bits
	u32	m_size;			// 1C size of buffer in bits
};

static_assert(sizeof(TBitArray) == 0x20);

// 38
template <typename T>
class TSparseArray
{
public:
	TArray <T>	m_data;				// 00
	TBitArray	m_presence;			// 10
	u32			m_sparseEntryHead;	// 30 start of a linked list? tbd
	u32			m_numSparseEntries;	// 34 actual used entries is m_data.m_len - m_numSparseEntries
};

static_assert(sizeof(TSparseArray<u8>) == 0x38);

// 50
template <typename T>
class TSet
{
public:
	// todo: needs ability to hash T
	// presumably check entry hash(data) % m_hashSize

	struct Entry
	{
		T	data;
		u32	unk0;	// hash links
		u32	unk1;	// hash links
	};

	TSparseArray <Entry>	m_entries;	// 00
	u32	m_defaultHashData;				// 38
	u32	pad3C;							// 3C
	u32	* m_hashData;					// 40 null if m_defaultHashData is used instead
	u32	m_hashSize;						// 48 alloc size (power of 2)
	u32	pad4C;							// 4C
};

static_assert(sizeof(TSet<u8>) == 0x50);

// 50
template <typename T_Key, typename T_Data>
class TMap
{
public:
	struct Entry
	{
		T_Key key;
		T_Data data;
	};

	TSet <Entry> m_data;
};

static_assert(sizeof(TMap<u8, u8>) == 0x50);
