// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace Turpentine
{
	namespace RE
	{
		struct ParamInfo
		{
			const char* typeStr;
			uint32_t typeID;		// ParamType
			uint32_t isOptional;	// do other bits do things?
		};
		static_assert(sizeof(ParamInfo) == 0x10);

		struct CommandInfo
		{
			using ScriptType = char(__stdcall*)(ParamInfo* paramInfo, void* a2, void* thisObj, __int64 a4,
				void* scriptObj, void* eventList, int result, uint32_t* opcodeOffsetPtr);
			using ConsoleType = char(__stdcall*)(void* thisObj, RE::ParamInfo* paramInfo, void* a3, int result);
	
			const char* longName;	// 00
			const char* shortName;	// 08
			uint32_t opcode;		// 10
			char pad14[0x4];		// 14
			const char* helpText;	// 18
			uint16_t needsParent;	// 20
			uint16_t numParams;		// 22
			char pad24[0x4];		// 24
			ParamInfo* params;		// 28
			ScriptType script;		// 30
			ConsoleType console;	// 38
			char pad40[0x8];		// 40 
			uint32_t flags;			// 48
			char pad4C[0x4];		// 4C
		};
		static_assert(sizeof(CommandInfo) == 0x50);

		extern CommandInfo* OldTableCommands;
		extern CommandInfo* NewTableCommands;

		void APIENTRY DumpCommandTable(const char* fname);
	}

	namespace Hooks
	{
		void APIENTRY CommandTable() noexcept(true);
	}
}