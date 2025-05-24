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
			static void* APIENTRY GameApplyRepairItemAllByPlayer()
			{
				return Utils::FastCall<void*>(REL::Offset(0x67E2120));
			}
		}

		void APIENTRY NoRepairConfirmMessage() noexcept(true)
		{
			REL::PatchNop(REL::Offset(0x67E2157), 0x6);
			REL::Patch(REL::Offset(0x67E2156), { 0x00 });
			REL::DetourCall(REL::Offset(0x67E0585), (uintptr_t)&Impl::GameApplyRepairItemAllByPlayer);

			_MESSAGE("Install NoRepairConfirmMessage patch");
		}
	}
}
