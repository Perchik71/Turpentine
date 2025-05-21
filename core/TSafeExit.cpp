// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TSafeExit.h>
#include <TRelocation.h>

extern uintptr_t GlobalBase;

namespace Turpentine
{
	namespace Fixes
	{
		namespace Impl
		{
			static void WINAPI HKPostQuitMessage(int nExitCode) noexcept(true)
			{
				if (!TerminateProcess(GetCurrentProcess(), 0))
					std::abort();
				__assume(0);
			}
		}

		void APIENTRY SafeExit() noexcept(true)
		{
			REL::DetourIAT(GlobalBase, "user32.dll", "PostQuitMessage", (uintptr_t)&Impl::HKPostQuitMessage);
			_MESSAGE("Install SafeExit fixes");
		}
	}
}
