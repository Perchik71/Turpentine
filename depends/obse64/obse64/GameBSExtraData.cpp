#include "GameBSExtraData.h"
#include "obse64_common/Relocation.h"
#include "obse64_common/Log.h"
#include <Windows.h>

RelocPtr <CRITICAL_SECTION> g_extraListCS(0x09474300);

void BaseExtraList::Lock()
{
	EnterCriticalSection(g_extraListCS);
}

void BaseExtraList::Unlock()
{
	LeaveCriticalSection(g_extraListCS);
}

BSExtraData * BaseExtraList::Get(u8 type) const
{
	BSExtraData * result = nullptr;

	if(Contains(type))
	{
		for(auto * iter = m_data; iter; iter = iter->m_next)
		{
			if(iter->m_type == type)
			{
				result = iter;
				break;
			}
		}

		if(!result)
		{
			_WARNING("BaseExtraList: mismatch between presence bitfield and data %02X", type);
		}
	}

	return result;
}

bool BaseExtraList::Contains(u8 type) const
{
	if(type >= kExtraData_Max)
		return false;

	return (m_presence[type >> 3] >> (type & 7)) & 1;
}
