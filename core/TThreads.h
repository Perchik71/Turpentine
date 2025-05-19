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
				static void WINAPI HKSleep(DWORD dwMilliseconds) noexcept(true);
				static void WINAPI HKSleepEx(DWORD dwMilliseconds, BOOL bAlertable) noexcept(true);
				static BOOL WINAPI HKSetThreadPriority(HANDLE Thread, int Priority) noexcept(true);
				static DWORD_PTR WINAPI HKSetThreadAffinityMask(HANDLE Thread, DWORD_PTR AffinityMask) noexcept(true);
				static BOOL WINAPI HKSetThreadGroupAffinity(HANDLE hThread, GROUP_AFFINITY* GroupAffinity,
					PGROUP_AFFINITY PreviousGroupAffinity) noexcept(true);
			};
		}

		void APIENTRY Threads() noexcept(true);
	}
}