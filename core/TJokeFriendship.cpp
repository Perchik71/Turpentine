// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TJokeFriendship.h>
#include <TRelocation.h>

namespace Turpentine
{
	namespace Jokes
	{
		void APIENTRY Friendship() noexcept(true)
		{
			Turpentine::REL::PatchNop(REL::Offset(0x65FD3FB), 5);
		}
	}
}
