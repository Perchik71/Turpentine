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
			class GarbageCollectorIntf
			{
			public:
				static DWORD WINAPI HKGarbageCollectorIntf_WaitForSingleObject
					(HANDLE hHandle, DWORD dwMilliseconds) noexcept(true);
			};
		}

		void APIENTRY PatchGarbageCollector() noexcept(true);
	}
}