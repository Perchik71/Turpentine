// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TNoTradeItemConfirmMessage.h>
#include <TRelocation.h>
#include <TUtils.h>

namespace Turpentine
{
	namespace Patches
	{
		namespace Impl
		{
			static void* APIENTRY GameApplySellOrBuyItemFromPlayer()
			{
				return Utils::FastCall<void*>(REL::Offset(0x6782400));
			}
		}

		void APIENTRY NoTradeItemConfirmMessage() noexcept(true)
		{
			// Sell / Buy
			REL::Patch(REL::Offset(0x678240A), { 0x00 });
			REL::PatchNop(REL::Offset(0x678240B), 0x6);
			REL::DetourCall(REL::Offset(0x677F6AD), (uintptr_t)&Impl::GameApplySellOrBuyItemFromPlayer);

			_MESSAGE("Install NoTradeItemConfirmMessage patch");
		}
	}
}
