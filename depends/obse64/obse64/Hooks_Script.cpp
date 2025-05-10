#include "Hooks_Script.h"
#include "obse64/GameConsole.h"
#include "obse64/GameScript.h"
#include "obse64_common/SafeWrite.h"
#include "obse64_common/obse64_version.h"
#include "obse64_common/BranchTrampoline.h"
#include "obse64_common/Relocation.h"
#include "obse64_common/Log.h"
#include "xbyak/xbyak.h"

bool GetOBSEVersion_Execute(const SCRIPT_PARAMETER* paramInfo, const char*, TESObjectREFR* thisObj, TESObjectREFR* containingObj, Script* script, ScriptLocals* locals, float* result, u32* opcodeOffsetPtr)
{
	_MESSAGE("GetOBSEVersion_Execute");

	Console_Print("OBSE64 version: %d.%d.%d, release idx %d, runtime %08X",
		OBSE_VERSION_INTEGER, OBSE_VERSION_INTEGER_MINOR, OBSE_VERSION_INTEGER_BETA,
		OBSE_VERSION_RELEASEIDX, RUNTIME_VERSION);

	return true;
}

void ConsoleCommandInit_Hook()
{
	for (Script::SCRIPT_FUNCTION* iter = g_firstConsoleCommand; iter->eOutput < (Script::kScript_NumConsoleCommands + Script::kScript_ConsoleOpBase); ++iter)
	{
		if (!iter->pExecuteFunction)
			continue;

		if (!strcmp(iter->pFunctionName, "WasteMemory"))
		{
			Script::SCRIPT_FUNCTION& cmd = *iter;
			cmd.pFunctionName = "GetOBSEVersion";
			cmd.pShortName = "";
			cmd.pHelpString = "";
			cmd.bReferenceFunction = 0;
			cmd.sParamCount = 0;
			cmd.pExecuteFunction = GetOBSEVersion_Execute;
			cmd.bEditorFilter = 0;
			cmd.bInvalidatesCellList = 0;
		}
	}
}

void Hooks_Script_Apply()
{
	ConsoleCommandInit_Hook();
}
