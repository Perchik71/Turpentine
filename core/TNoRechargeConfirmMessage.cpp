// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TNoRechargeConfirmMessage.h>
#include <TRelocation.h>
#include <TUtils.h>

namespace Turpentine
{
	namespace Patches
	{
		uintptr_t sub_NoRechargeConfirmMessage_0 = 0;
		uintptr_t sub_NoRechargeConfirmMessage_1 = 0;
		uintptr_t sub_NoRechargeConfirmMessage_2 = 0;
		uintptr_t sub_NoRechargeConfirmMessage_3 = 0;

		namespace Impl
		{
			static void* APIENTRY GameApplyRechargeItemByPlayer()
			{
				return Utils::FastCall<void*>(sub_NoRechargeConfirmMessage_1);
			}

			static void* APIENTRY GameApplyRechargeItemAllByPlayer()
			{
				return Utils::FastCall<void*>(sub_NoRechargeConfirmMessage_3);
			}
		}

		void APIENTRY NoRechargeConfirmMessage() noexcept(true)
		{
			sub_NoRechargeConfirmMessage_0 = REL::ID(411278);	// 67B00D0
			sub_NoRechargeConfirmMessage_1 = REL::ID(411277);	// 67AFDE0
			sub_NoRechargeConfirmMessage_2 = REL::ID(411299);	// 67B1CE0
			sub_NoRechargeConfirmMessage_3 = REL::ID(411293);	// 67B1580

			REL::PatchNop(sub_NoRechargeConfirmMessage_1 + 0xB, 0x6);
			REL::Patch(sub_NoRechargeConfirmMessage_1 + 0xA, { 0x00 });
			REL::DetourCall(sub_NoRechargeConfirmMessage_0 + 0x6D6, (uintptr_t)&Impl::GameApplyRechargeItemByPlayer);
			
			REL::PatchNop(sub_NoRechargeConfirmMessage_3 + 0x25, 0x6);
			REL::Patch(sub_NoRechargeConfirmMessage_3 + 0x24, { 0x00 });
			REL::PatchNop(sub_NoRechargeConfirmMessage_2 + 0x50, 0x6);
			REL::DetourCall(sub_NoRechargeConfirmMessage_2 + 0x151, (uintptr_t)&Impl::GameApplyRechargeItemAllByPlayer);

			_MESSAGE("Install NoRechargeConfirmMessage patch");
		}
	}
}
