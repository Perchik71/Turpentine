// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TNoScriptMessageBox.h>
#include <TRelocation.h>
#include <TUtils.h>

namespace Turpentine
{
	namespace Patches
	{
		namespace Impl
		{
			struct Command
			{
				using ScriptFunctionType = char(__stdcall*)(__int64, __int64, __int64, __int64,
					__int64, __int64, int, unsigned int*);
				using ConsoleFunctionType = char(__stdcall*)(__int64, __int64, __int64, __int64);

				char* name;
				char pad08[0x28];
				ScriptFunctionType scriptFunction;
				ConsoleFunctionType consoleFunction;
				char pad40[0x10];
			};
			static_assert(sizeof(Command) == 0x50);

			Command* OldCommands = nullptr;
			Command* NewCommands = nullptr;
		}

		void APIENTRY NoScriptMessageBox() noexcept(true)
		{
			Impl::OldCommands = (Impl::Command*)REL::Offset(0x8FEC850);
			Impl::NewCommands = (Impl::Command*)REL::Offset(0x8FF3C40);
			
			if (!_stricmp(Impl::OldCommands[0].name, "MessageBox"))
			{
				Turpentine::REL::Patch((uintptr_t)Impl::OldCommands[0].scriptFunction, { 0x31, 0xC0, 0xFE, 0xC0, 0xC3, 0x90 });
				Turpentine::REL::Patch((uintptr_t)Impl::OldCommands[0].consoleFunction, { 0x31, 0xC0, 0xFE, 0xC0, 0xC3, 0x90 });

				_MESSAGE("Install NoScriptMessageBox patch");
			}
		}
	}
}
