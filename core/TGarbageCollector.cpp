// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TGarbageCollector.h>
#include <TRelocation.h>

extern uintptr_t GlobalBase;

namespace Turpentine
{
	namespace Patches
	{
		namespace Impl
		{
			DWORD WINAPI GarbageCollectorIntf::HKGarbageCollectorIntf_WaitForSingleObject
				(HANDLE hHandle, DWORD dwMilliseconds) noexcept(true)
			{
				if (dwMilliseconds == INFINITE)
				{
				repeat:
					auto Result = WaitForSingleObject(hHandle, 10);
					if (Result == WAIT_TIMEOUT)
					{
						SwitchToThread();
						goto repeat;
					}

					return Result;
				}

				return WaitForSingleObject(hHandle, dwMilliseconds);
			}
		}

		void APIENTRY PatchGarbageCollector() noexcept(true)
		{
			// Remove the cleaning call from one place, there is no leak, it looks like there is another call,
			// which led to blocking of each other.
			REL::Patch(GlobalBase + 0xDD9388, { 0x90, 0x90, 0x90 });

			REL::DetourCall(GlobalBase + 0xF69FF0, 
				(uintptr_t)&Impl::GarbageCollectorIntf::HKGarbageCollectorIntf_WaitForSingleObject);
			REL::DetourCall(GlobalBase + 0xF6673E,
				(uintptr_t)&Impl::GarbageCollectorIntf::HKGarbageCollectorIntf_WaitForSingleObject);
			/* Crashes Exit
			REL::DetourCall(GlobalBase + 0xF6A089,
				(uintptr_t)&Impl::GarbageCollectorIntf::HKGarbageCollectorIntf_WaitForSingleObject);*/
			REL::DetourCall(GlobalBase + 0xF62BF8,
				(uintptr_t)&Impl::GarbageCollectorIntf::HKGarbageCollectorIntf_WaitForSingleObject);
		}
	}
}
