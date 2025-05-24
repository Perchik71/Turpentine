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
				return Utils::FastCall<void*>(REL::Offset(0x67E0120));
			}

			static void* APIENTRY GameApplyRepairItemAllByPlayer()
			{
				return Utils::FastCall<void*>(REL::Offset(0x67E2120));
			}
		}

		void APIENTRY NoRepairConfirmMessage() noexcept(true)
		{
			// One
			REL::Patch(REL::Offset(0x67E0173), { 0x00 });
			REL::PatchNop(REL::Offset(0x67E0174), 0x6);
			REL::DetourCall(REL::Offset(0x67E10CD), (uintptr_t)&Impl::GameApplyRepairItemByPlayer);
			REL::PatchNop(REL::Offset(0x67E10D2), 0x8);
			
			// All
			REL::Patch(REL::Offset(0x67E2156), { 0x00 });
			REL::PatchNop(REL::Offset(0x67E2157), 0x6);
			REL::DetourCall(REL::Offset(0x67E0585), (uintptr_t)&Impl::GameApplyRepairItemAllByPlayer);
			REL::PatchNop(REL::Offset(0x67E058A), 0x8);
		
			_MESSAGE("Install NoRepairConfirmMessage patch");
		}
	}
}
