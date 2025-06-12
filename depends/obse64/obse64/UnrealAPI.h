#pragma once

#include "obse64_common/Relocation.h"
#include "UnrealObject.h"

// 8
class FExec
{
public:
	virtual ~FExec();
};

static_assert(sizeof(FExec) == 0x08);

// 
class UEngine : public UObject, public FExec
{
	// a bunch of stuff
};

extern RelocPtr <UEngine> g_ueEngine;

// unreal allocator, can be overridden at early startup
struct FMemory
{
	void Free(void * buf);
	void * Malloc(size_t size, u32 align = 0);
};
