// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TNoRepairConfirmMessage.h>
#include <TRelocation.h>
#include <TUtils.h>

namespace Turpentine
{
	namespace Patches
	{
		uintptr_t sub_NoRepairConfirmMessage_0 = 0;
		uintptr_t sub_NoRepairConfirmMessage_1 = 0;
		uintptr_t sub_NoRepairConfirmMessage_2 = 0;

		namespace Impl
		{
			static void* APIENTRY GameApplyRepairItemByPlayer()
			{
				return Utils::FastCall<void*>(sub_NoRepairConfirmMessage_1);
			}

			static void* APIENTRY GameApplyRepairItemAllByPlayer()
			{
				return Utils::FastCall<void*>(sub_NoRepairConfirmMessage_2);
			}
		}

		void APIENTRY NoRepairConfirmMessage() noexcept(true)
		{
			sub_NoRepairConfirmMessage_0 = REL::ID(411317);	// 67B4330
			sub_NoRepairConfirmMessage_1 = REL::ID(411316);	// 67B40A0
			sub_NoRepairConfirmMessage_2 = REL::ID(411335);	// 67B60A0

			// One
			REL::Patch(sub_NoRepairConfirmMessage_1 + 0x53, { 0x00 });
			REL::PatchNop(sub_NoRepairConfirmMessage_1 + 0x54, 0x6);
			REL::DetourCall(sub_NoRepairConfirmMessage_0 + 0xD1D, (uintptr_t)&Impl::GameApplyRepairItemByPlayer);
			REL::PatchNop(sub_NoRepairConfirmMessage_0 + 0xD22, 0x8);
			
			// All
			REL::Patch(sub_NoRepairConfirmMessage_2 + 0x36, { 0x00 });
			REL::PatchNop(sub_NoRepairConfirmMessage_2 + 0x37, 0x6);
			REL::DetourCall(sub_NoRepairConfirmMessage_0 + 0x1D5, (uintptr_t)&Impl::GameApplyRepairItemAllByPlayer);
			REL::PatchNop(sub_NoRepairConfirmMessage_0 + 0x1DA, 0x8);
		
			_MESSAGE("Install NoRepairConfirmMessage patch");
		}
	}
}
