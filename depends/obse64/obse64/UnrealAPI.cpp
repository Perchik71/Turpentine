#include "UnrealAPI.h"

RelocPtr <UEngine> g_ueEngine(0x092734C0);

typedef void (* _FMemory_Free)(void * buf);
RelocAddr <_FMemory_Free> FMemory_Free(0x00E24570);

typedef void * (*_FMemory_Alloc)(size_t size, u32 align);
RelocAddr <_FMemory_Alloc> FMemory_Alloc(0x00E30FB0);

void FMemory::Free(void * buf)
{
	FMemory_Free(buf);
}

void * FMemory::Malloc(size_t size, u32 align)
{
	return FMemory_Alloc(size, align);
}
