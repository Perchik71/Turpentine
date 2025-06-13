// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TNoPoisonConfirmMessage.h>
#include <TRelocation.h>
#include <TUtils.h>

namespace Turpentine
{
	namespace Patches
	{
		uintptr_t sub_NoPoisonConfirmMessage_0 = 0;
		uintptr_t sub_NoPoisonConfirmMessage_1 = 0;

		namespace Impl
		{
			static void* APIENTRY GameApplyPoisonOnWeaponOrBow()
			{
				return Utils::FastCall<void*>(sub_NoPoisonConfirmMessage_1);
			}
		}

		void APIENTRY NoPoisonConfirmMessage() noexcept(true)
		{
			sub_NoPoisonConfirmMessage_0 = REL::ID(406775); // 65F0450
			sub_NoPoisonConfirmMessage_1 = REL::ID(406910); // 65FF430

			REL::Patch(sub_NoPoisonConfirmMessage_0 + 0x24D, { 0x00 });
			REL::Patch(sub_NoPoisonConfirmMessage_1 + 0x41, { 0x00 });
			REL::Patch(sub_NoPoisonConfirmMessage_0 + 0x2FE, { 0x31, 0xD2, 0x90, 0x90, 0x90, 0x90, 0x90 });
			REL::DetourCall(sub_NoPoisonConfirmMessage_0 + 0x25C, (uintptr_t)&Impl::GameApplyPoisonOnWeaponOrBow);
			REL::PatchNop(sub_NoPoisonConfirmMessage_1 + 0x42, 0x6);
			REL::PatchNop(sub_NoPoisonConfirmMessage_0 + 0xAF, 0x5);
			REL::PatchNop(sub_NoPoisonConfirmMessage_0 + 0x9A, 0x5);
			
			_MESSAGE("Install NoPoisonConfirmMessage patch");
		}
	}
}
