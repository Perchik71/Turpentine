// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TMaxStdio.h>

extern uintptr_t GlobalBase;

namespace Turpentine
{
	namespace Patches
	{
		void APIENTRY PatchMaxStdio() noexcept(true)
		{
			int value = 2048;

			if (_setmaxstdio(value) == -1)
				_ERROR("When trying to sets %d _setmaxstdio returned failed", value);
			else
				_MESSAGE("_setmaxstdio sets %d", value);

			DebugLog::flush();
		}
	}
}
