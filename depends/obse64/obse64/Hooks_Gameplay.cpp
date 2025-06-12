#include "Hooks_Gameplay.h"
#include "obse64_common/BranchTrampoline.h"
#include "obse64_common/Relocation.h"

RelocAddr <uintptr_t> OblivionThread_Target(0x065CF640 + 0x1208);
RelocAddr <uintptr_t> UnrealGameThread_Target(0x03907F30 + 0x53);

void OblivionThreadHook(const char * dbgStr)
{

}

void UnrealGameThreadHook()
{

}

void Hooks_Gameplay_Apply()
{
	// replace call to stubbed debug logging function being passed "Oblivion Main loop"
	g_branchTrampoline.write5Call(OblivionThread_Target.getUIntPtr(), uintptr_t(OblivionThreadHook));

	// replace call to stubbed hitch detection function
	g_branchTrampoline.write5Call(UnrealGameThread_Target.getUIntPtr(), uintptr_t(UnrealGameThreadHook));
}
