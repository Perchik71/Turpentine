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
		namespace Impl
		{
			static void* APIENTRY GameApplyRechargeItemByPlayer()
			{
				return Utils::FastCall<void*>(REL::Offset(0x67AFDE0));
			}

			static void* APIENTRY GameApplyRechargeItemAllByPlayer()
			{
				return Utils::FastCall<void*>(REL::Offset(0x67B1580));
			}
		}

		void APIENTRY NoRechargeConfirmMessage() noexcept(true)
		{
			REL::PatchNop(REL::Offset(0x67AFDEB), 0x6);
			REL::Patch(REL::Offset(0x67AFDEA), { 0x00 });
			REL::DetourCall(REL::Offset(0x67B07A6), (uintptr_t)&Impl::GameApplyRechargeItemByPlayer);

			REL::PatchNop(REL::Offset(0x67B15A5), 0x6);
			REL::Patch(REL::Offset(0x67B15A4), { 0x00 });
			REL::DetourCall(REL::Offset(0x67B1E31), (uintptr_t)&Impl::GameApplyRechargeItemAllByPlayer);

			_MESSAGE("Install NoRechargeConfirmMessage patch");
		}
	}
}
