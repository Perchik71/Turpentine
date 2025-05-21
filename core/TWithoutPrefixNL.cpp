// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TWithoutPrefixNL.h>
#include <TRelocation.h>

namespace Turpentine
{
	namespace Fixes
	{
		void APIENTRY WithoutPrefixNL() noexcept(true)
		{
			Turpentine::REL::Patch(REL::Offset(0x7DABBAC), { 0x00 });

			_MESSAGE("Install WithoutPrefixNL fixes");
		}
	}
}
