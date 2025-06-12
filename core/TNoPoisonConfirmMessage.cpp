// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TNoPoisonConfirmMessage.h>
#include <THookCommandTable.h>
#include <TRelocation.h>
#include <TUtils.h>

namespace Turpentine
{
	namespace Patches
	{
		namespace Impl
		{
			static void* APIENTRY GameApplyPoisonOnWeaponOrBow()
			{
				return Utils::FastCall<void*>(REL::Offset(0x65FF430));
			}
		}

		void APIENTRY NoPoisonConfirmMessage() noexcept(true)
		{
			REL::Patch(REL::Offset(0x65F069D), { 0x00 });
			REL::Patch(REL::Offset(0x65FF471), { 0x00 });
			REL::Patch(REL::Offset(0x65F074E), { 0x31, 0xD2, 0x90, 0x90, 0x90, 0x90, 0x90 });
			REL::DetourCall(REL::Offset(0x65F06AC), (uintptr_t)&Impl::GameApplyPoisonOnWeaponOrBow);
			REL::PatchNop(REL::Offset(0x65FF472), 0x6);
			REL::PatchNop(REL::Offset(0x65F04FF), 0x5);
			REL::PatchNop(REL::Offset(0x65F04EA), 0x5);

			_MESSAGE("Install NoPoisonConfirmMessage patch");
		}
	}
}
