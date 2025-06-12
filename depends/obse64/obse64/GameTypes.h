#pragma once

#include "obse64_common/Types.h"

// base types/containers

// 10
template <typename T>
class BSSimpleList
{
public:
	struct Node
	{
		T		m_data;
		Node	* m_next;
	};

	Node node;
};

static_assert(sizeof(BSSimpleList<void *>) == 0x10);

// 10
class BSStringT
{
public:
	char	* m_data;	// 0
	u16		m_dataLen;	// 8
	u16		m_bufLen;	// A
};

static_assert(sizeof(BSStringT) == 0x10);
