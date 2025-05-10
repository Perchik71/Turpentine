// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace Turpentine
{
	namespace Patches
	{
		namespace Impl
		{
			class ThreadIntf
			{
			public:
				static BOOL WINAPI HKSetThreadPriority(HANDLE Thread, int Priority) noexcept(true);
				static DWORD_PTR WINAPI HKSetThreadAffinityMask(HANDLE Thread, DWORD_PTR AffinityMask) noexcept(true);
			};
		}

		void APIENTRY PatchThreads() noexcept(true);
	}
}