#include "UnrealTypes.h"

RelocPtr <FNameStorage> g_fnameStorage(0x09061500);

FNameEntry * FNameStorage::Get(u32 idx)
{
	u32	entry = (idx >> 16) & 0xFFFF;	// chunk offset
	u32 offset = (idx & 0xFFFF) * 2;	// byte offset

	u8 * buf = (u8 *)m_chunks[entry];

	return (FNameEntry *)(buf + offset);
}

FNameEntry * FNameStorage::Get(const FName & fname)
{
	return Get(fname.m_idx);
}
