// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TJokeFriendship.h>
#include <TRelocation.h>
#include "TUtils.h"

extern msrtti::section GlobalSection[3];

namespace Turpentine
{
	namespace Jokes
	{
		void APIENTRY Friendship() noexcept(true)
		{
			auto offset = Turpentine::REL::FindPattern(GlobalSection[0].base, GlobalSection[0].end - GlobalSection[0].base,
				"BA 05 00 00 00 48 8B 0D ? ? ? ? E8 ? ? ? ? F3 0F 10 15 ? ? ? ? BA 02 00 00 00 48 8B 0D ? ? ? ? "
				"E8 ? ? ? ? 48 8B CD E8 ? ? ? ?");
			
			if (!offset)
				_MESSAGE("Install Friendship joke [SIG:FAILED]");
			else
			{
				Turpentine::REL::PatchNop(offset - 0x2D, 5);

				_MESSAGE("Install Friendship joke");
			}
		}
	}
}
