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
		uintptr_t sub_NoTradeItemConfirmMessage_0 = 0;
		uintptr_t sub_NoTradeItemConfirmMessage_1 = 0;

		namespace Impl
		{
			static void* APIENTRY GameApplySellOrBuyItemFromPlayer()
			{
				return Utils::FastCall<void*>(sub_NoTradeItemConfirmMessage_1);
			}
		}

		void APIENTRY NoTradeItemConfirmMessage() noexcept(true)
		{
			sub_NoTradeItemConfirmMessage_0 = REL::ID(410449);	// 6753400
			sub_NoTradeItemConfirmMessage_1 = REL::ID(410475);	// 6756810

			// Sell / Buy
			REL::Patch(sub_NoTradeItemConfirmMessage_1 + 0x9, { 0xB0 });
			REL::PatchNop(sub_NoTradeItemConfirmMessage_1 + 0xB, 0x6);
			// Remove trigger menu
			REL::PatchNop(sub_NoTradeItemConfirmMessage_0 + 0x6A5, 0x5);
			REL::DetourCall(sub_NoTradeItemConfirmMessage_0 + 0x6D1, (uintptr_t)&Impl::GameApplySellOrBuyItemFromPlayer);
			
			_MESSAGE("Install NoTradeItemConfirmMessage patch");
		}
	}
}
