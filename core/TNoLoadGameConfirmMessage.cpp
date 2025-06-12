// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TNoLoadGameConfirmMessage.h>
#include <TRelocation.h>
#include <TUtils.h>

namespace Turpentine
{
	namespace Patches
	{
		namespace Impl
		{
			static void* APIENTRY GameLoad()
			{
				return Utils::FastCall<void*>(REL::Offset(0x67C42A0));
			}
		}

		void APIENTRY NoLoadGameConfirmMessage() noexcept(true)
		{
			REL::Patch(REL::Offset(0x67C44C4), { 0x00 });
			REL::PatchNop(REL::Offset(0x67C44CB), 0x2);
			REL::DetourCall(REL::Offset(0x67C4999), (uintptr_t)&Impl::GameLoad);

			_MESSAGE("Install NoLoadGameConfirmMessage patch");
		}
	}
}
