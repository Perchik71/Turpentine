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
		uintptr_t sub_LoadGameConfirmMessage_0 = 0;
		uintptr_t sub_LoadGameConfirmMessage_1 = 0;

		namespace Impl
		{
			static void* APIENTRY GameLoad()
			{
				// 1.5 0x67C42A0
				return Utils::FastCall<void*>(sub_LoadGameConfirmMessage_0);
			}
		}

		void APIENTRY NoLoadGameConfirmMessage() noexcept(true)
		{
			sub_LoadGameConfirmMessage_0 = REL::ID(411475);
			sub_LoadGameConfirmMessage_1 = REL::ID(411477);

			REL::Patch(sub_LoadGameConfirmMessage_0 + 0x224, { 0x00 });
			REL::PatchNop(sub_LoadGameConfirmMessage_0 + 0x22B, 0x2);							// 1.5 0x67C44CB
			REL::DetourCall(sub_LoadGameConfirmMessage_1 + 0x2C9, (uintptr_t)&Impl::GameLoad);	// 1.5 0x67C4859
			REL::DetourCall(sub_LoadGameConfirmMessage_1 + 0x409, (uintptr_t)&Impl::GameLoad);	// 1.5 0x67C4999
			
			_MESSAGE("Install NoLoadGameConfirmMessage patch");
		}
	}
}
