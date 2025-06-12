#include "GameRTTI.h"
#include "obse64_common/Relocation.h"
#include "obse64_common/Types.h"

typedef void * (*_Runtime_DynamicCast_Internal)(void * srcObj, u32 arg1, const void * fromType, const void * toType, u32 arg4);

RelocAddr <_Runtime_DynamicCast_Internal> Runtime_DynamicCast_Internal(0x06BF7A96);	// __RTDynamicCast

void * Runtime_DynamicCast(void * srcObj, const void * fromType, const void * toType)
{
	uintptr_t fromTypeAddr = uintptr_t(fromType) + RelocationManager::s_baseAddr;
	uintptr_t toTypeAddr = uintptr_t(toType) + RelocationManager::s_baseAddr;

	return Runtime_DynamicCast_Internal(srcObj, 0, (void *)fromTypeAddr, (void *)toTypeAddr, 0);
}

#include "GameRTTI.inl"
