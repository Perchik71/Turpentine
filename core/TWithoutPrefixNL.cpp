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
			auto sub = (uint8_t*)REL::ID(308068); // 4883B20
			if (!sub)
			{
				_MESSAGE("Install WithoutPrefixNL fixes [ID:FAILED]");
				return;
			}

			sub += 0x6D5;
			if ((sub[0] == 0x48) && (sub[1] == 0x8D) && (sub[2] == 0x15))
			{
				auto offset = (uintptr_t)(*(uint32_t*)(sub + 3)) + (uintptr_t)sub + 7;
				Turpentine::REL::Patch(offset, { 0x00 });
				
				_MESSAGE("Install WithoutPrefixNL fixes");
			}
			else
				_MESSAGE("Install WithoutPrefixNL fixes [SIG:FAILED]");
		}
	}
}
