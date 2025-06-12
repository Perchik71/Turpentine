#pragma once

#include "obse64_common/Types.h"

// base types/containers

// 20
// derives from NiTMapBase, we don't bother
template <typename T_key, typename T_data>
class NiTMap
{
public:
	virtual ~NiTMap();

	struct NiTMapItem
	{
		NiTMapItem * next;
		T_key		key;
		T_data		data;
	};

	T_data	Get(T_key key)
	{
		u32	bucket = GetBucket(key);

		for(NiTMapItem * iter = buckets[bucket]; iter; iter = iter->next)
		{
			if(Compare(iter->key, key))
			{
				return iter->data;
			}
		}

		return T_data();
	}

	virtual u32				GetBucket(T_key key);					// return hash % numBuckets;
	virtual bool			Compare(T_key lhs, T_key rhs);			// return lhs == rhs;
	virtual void			FillItem(NiTMapItem * item, T_key key, T_data data);
	// item->key = key; item->data = data;
	virtual void			ResetItem(NiTMapItem * item);
	virtual NiTMapItem *	AllocItem(void);					// return new NiTMapItem;
	virtual void			FreeItem(NiTMapItem * item);			// item->data = 0; delete item;

//	void		** _vtbl;	// 00
	u32			numBuckets;	// 08
	u32			pad0C;		// 0C
	NiTMapItem ** buckets;	// 10
	u32			numEntries;	// 18
	u32			pad1C;		// 1C
};

static_assert(sizeof(NiTMap<u32, u32>) == 0x20);

// C
struct NiPoint3
{
	float	x, y, z;
};

static_assert(sizeof(NiPoint3) == 0xC);

// 18
template <typename T>
struct NiTArray
{
	virtual ~NiTArray();

//	void	** _vtbl;			// 00
	T		* m_data;			// 08
	u16		m_capacity;			// 10 - init'd to size of preallocation
	u16		m_firstFreeEntry;	// 12 - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	u16		m_numObjs;			// 14 - init'd to 0
	u16		m_growSize;			// 16 - init'd to size of preallocation
};

static_assert(sizeof(NiTArray<void *>) == 0x18);

// 20
template <typename T>
struct NiTLargeArray
{
	virtual ~NiTLargeArray();

//	void	** _vtbl;			// 00
	T		* m_data;			// 08
	u32		m_capacity;			// 10 - init'd to size of preallocation
	u32		m_firstFreeEntry;	// 14 - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	u32		m_numObjs;			// 18 - init'd to 0
	u32		m_growSize;			// 1C - init'd to size of preallocation
};

static_assert(sizeof(NiTLargeArray<void *>) == 0x20);
