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
				return Utils::FastCall<void*>(REL::Offset(0x6756810));
			}
		}

		void APIENTRY NoTradeItemConfirmMessage() noexcept(true)
		{
			// Sell / Buy
			REL::Patch(REL::Offset(0x675681A), { 0x00 });
			REL::PatchNop(REL::Offset(0x675681B), 0x6);
			REL::DetourCall(REL::Offset(0x6753AA5), (uintptr_t)&Impl::GameApplySellOrBuyItemFromPlayer);

			_MESSAGE("Install NoTradeItemConfirmMessage patch");
		}
	}
}
