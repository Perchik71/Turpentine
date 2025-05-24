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
				return Utils::FastCall<void*>(REL::Offset(0x67DBE80));
			}

			static void* APIENTRY GameApplyRechargeItemAllByPlayer()
			{
				return Utils::FastCall<void*>(REL::Offset(0x67DD620));
			}
		}

		void APIENTRY NoRechargeConfirmMessage() noexcept(true)
		{
			REL::PatchNop(REL::Offset(0x67DBE8B), 0x6);
			REL::Patch(REL::Offset(0x67DBE8A), { 0x00 });
			REL::DetourCall(REL::Offset(0x67DC846), (uintptr_t)&Impl::GameApplyRechargeItemByPlayer);

			REL::PatchNop(REL::Offset(0x67DD645), 0x6);
			REL::Patch(REL::Offset(0x67DD644), { 0x00 });
			REL::DetourCall(REL::Offset(0x67DDED1), (uintptr_t)&Impl::GameApplyRechargeItemAllByPlayer);

			_MESSAGE("Install NoRechargeConfirmMessage patch");
		}
	}
}
