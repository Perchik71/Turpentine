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
		namespace Impl
		{
			static void* APIENTRY GameApplyRepairItemByPlayer()
			{
				return Utils::FastCall<void*>(REL::Offset(0x67B40A0));
			}

			static void* APIENTRY GameApplyRepairItemAllByPlayer()
			{
				return Utils::FastCall<void*>(REL::Offset(0x67B60A0));
			}
		}

		void APIENTRY NoRepairConfirmMessage() noexcept(true)
		{
			// One
			REL::Patch(REL::Offset(0x67B40F3), { 0x00 });
			REL::PatchNop(REL::Offset(0x67B40F4), 0x6);
			REL::DetourCall(REL::Offset(0x67B504D), (uintptr_t)&Impl::GameApplyRepairItemByPlayer);
			REL::PatchNop(REL::Offset(0x67B5052), 0x8);
			
			// All
			REL::Patch(REL::Offset(0x67B60D6), { 0x00 });
			REL::PatchNop(REL::Offset(0x67B60D7), 0x6);
			REL::DetourCall(REL::Offset(0x67B4505), (uintptr_t)&Impl::GameApplyRepairItemAllByPlayer);
			REL::PatchNop(REL::Offset(0x67B450A), 0x8);
		
			_MESSAGE("Install NoRepairConfirmMessage patch");
		}
	}
}
