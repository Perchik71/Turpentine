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
		namespace Impl
		{
			static void* APIENTRY GameApplyTradeSpellByPlayer()
			{
				return Utils::FastCall<void*>(REL::Offset(0x67EBDA0));
			}
		}

		void APIENTRY NoTradeSpellConfirmMessage() noexcept(true)
		{
			REL::Patch(REL::Offset(0x67EC323), { 0x00 });
			REL::Patch(REL::Offset(0x67EBDF9), { 0x00 });
			REL::Patch(REL::Offset(0x67EC31A), { 0x31, 0xD2, 0x90, 0x90, 0x90, 0x90, 0x90 });
			REL::DetourCall(REL::Offset(0x67EC333), (uintptr_t)&Impl::GameApplyTradeSpellByPlayer);
			REL::PatchNop(REL::Offset(0x67EBDFA), 0x6);

			_MESSAGE("Install NoTradeSpellConfirmMessage patch");
		}
	}
}
