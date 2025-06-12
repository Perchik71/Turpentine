#include "UnrealObject.h"
#include <Windows.h>

void FUObjectArray::Lock()
{
	EnterCriticalSection((CRITICAL_SECTION *)&m_lock);
}

void FUObjectArray::Unlock()
{
	LeaveCriticalSection((CRITICAL_SECTION *)&m_lock);
}

void FUObjectHashTable::Lock()
{
	EnterCriticalSection((CRITICAL_SECTION *)&m_lock);
}

void FUObjectHashTable::Unlock()
{
	LeaveCriticalSection((CRITICAL_SECTION *)&m_lock);
}

RelocPtr <FUObjectArray> g_uObjectArray(0x091077E0);

RelocPtr <FUObjectHashTable> g_uObjectHashTable(0x09107998);
