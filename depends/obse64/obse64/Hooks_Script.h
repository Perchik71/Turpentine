#pragma once

#include "obse64_common/Types.h"

struct CommandInfo;

enum CommandReturnType : u8
{
	kRetnType_Default,
	kRetnType_Form,
	kRetnType_String,
	kRetnType_Array,
	kRetnType_ArrayIndex,
	kRetnType_Ambiguous,

	kRetnType_Max
};

void Hooks_Script_Apply();

void AddScriptCommand(const CommandInfo & cmd, CommandReturnType retnType = kRetnType_Default);
