#pragma once

#include "obse64_common/Relocation.h"
#include "obse64_common/Utilities.h"

class Script;
class TESForm;
class TESObjectREFR;
class TESQuest;

struct SCRIPT_PARAMETER;
struct ScriptLocals;

typedef bool (*ExecuteFunction)(const SCRIPT_PARAMETER* paramInfo, const char*, TESObjectREFR* thisObj, TESObjectREFR* containingObj, Script* script, ScriptLocals* locals, float* result, u32* opcodeOffsetPtr);

class Script
{
public:
	struct SCRIPT_FUNCTION
	{
		const char*			pFunctionName;			// 00
		const char*			pShortName;				// 08
		u32					eOutput;				// 10
		u32					pad14;					// 14
		const char*			pHelpString;			// 18
		u8					bReferenceFunction;		// 20
		u8					pad21;					// 21
		u16					sParamCount;			// 22
		u32					pad24;					// 24
		SCRIPT_PARAMETER*	pParameters;			// 28
		ExecuteFunction		pExecuteFunction;		// 30
		void*				pCompileFunction;		// 38
		void*				pConditionFunction;		// 40
		u8					bEditorFilter;			// 48
		u8					bInvalidatesCellList;	// 49
		u8					unk4A;					// 4A
		u8					unk4B;					// 4B
		u8					unk4C;					// 4C
		u8					unk4E;					// 4D
		u8					unk4F;					// 4F
		u8					unk50;					// 50
	};

	enum
	{
		kScript_NumScriptCommands = 0x172,
		kScript_NumConsoleCommands = 0x086,

		kScript_ScriptOpBase = 0x1000,
		kScript_ConsoleOpBase = 0x0100,
	};

	// unk
};

extern RelocPtr<Script::SCRIPT_FUNCTION>	g_firstScriptCommand;
extern RelocPtr<Script::SCRIPT_FUNCTION>	g_firstConsoleCommand;
