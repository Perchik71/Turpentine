// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TNoTradeSpellConfirmMessage.h>
#include <TRelocation.h>
#include <TUtils.h>

namespace Turpentine
{
	namespace Patches
	{
		uintptr_t sub_NoTradeSpellConfirmMessage_0 = 0;
		uintptr_t sub_NoTradeSpellConfirmMessage_1 = 0;

		namespace Impl
		{
			static void* APIENTRY GameApplyTradeSpellByPlayer()
			{
				return Utils::FastCall<void*>(sub_NoTradeSpellConfirmMessage_1);
			}
		}

		void APIENTRY NoTradeSpellConfirmMessage() noexcept(true)
		{
			sub_NoTradeSpellConfirmMessage_0 = REL::ID(411431);	// 67BFF30
			sub_NoTradeSpellConfirmMessage_1 = REL::ID(411430);	// 67BFD80
			
			REL::Patch(sub_NoTradeSpellConfirmMessage_0 + 0x3D3, { 0x00 });
			REL::Patch(sub_NoTradeSpellConfirmMessage_1 + 0x59, { 0x00 });
			REL::Patch(sub_NoTradeSpellConfirmMessage_0 + 0x3CA, { 0x31, 0xD2, 0x90, 0x90, 0x90, 0x90, 0x90 });
			REL::DetourCall(sub_NoTradeSpellConfirmMessage_0 + 0x3E3, (uintptr_t)&Impl::GameApplyTradeSpellByPlayer);
			REL::PatchNop(sub_NoTradeSpellConfirmMessage_1 + 0x5A, 0x6);

			_MESSAGE("Install NoTradeSpellConfirmMessage patch");
		}
	}
}
