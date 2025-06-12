#include "Hooks_Data.h"
#include "obse64_common/Relocation.h"
#include "obse64_common/BranchTrampoline.h"
#include "GameData.h"
#include "PluginManager.h"

RelocAddr <uintptr_t> LoadingComplete_Hook(0x065CF640 + 0x9D2);

static void LoadingComplete()
{
	CALL_MEMBER_FN(TESDataHandler::GetSingleton(), UnkInit)();

	// before triggering starting cell
	g_pluginManager.dispatchMessage(0, OBSEMessagingInterface::kMessage_DataLoaded, nullptr, 0, nullptr);
}

void Hooks_Data_Apply()
{
	g_branchTrampoline.write5Call(LoadingComplete_Hook.getUIntPtr(), uintptr_t(&LoadingComplete));
}
