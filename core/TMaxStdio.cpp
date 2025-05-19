// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TMaxStdio.h>

namespace Turpentine
{
	namespace Patches
	{
		void APIENTRY MaxStdio(int value) noexcept(true)
		{
			int v = std::max(512, std::min(8192, value));

			if (_setmaxstdio(v) == -1)
				_ERROR("When trying to sets %d _setmaxstdio returned failed", v);
			else
				_MESSAGE("_setmaxstdio sets %d", v);
		}
	}
}
