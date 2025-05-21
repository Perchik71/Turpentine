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
				return Utils::FastCall<void*>(REL::Offset(0x662B070));
			}
		}

		void APIENTRY NoPoisonConfirmMessage() noexcept(true)
		{
			REL::Patch(REL::Offset(0x661C2DD), { 0x00 });
			REL::Patch(REL::Offset(0x662B0B1), { 0x00 });
			REL::Patch(REL::Offset(0x661C38E), { 0x31, 0xD2, 0x90, 0x90, 0x90, 0x90, 0x90 });
			REL::DetourCall(REL::Offset(0x661C2EC), (uintptr_t)&Impl::GameApplyPoisonOnWeaponOrBow);
			REL::PatchNop(REL::Offset(0x662B0B2), 0x6);
			REL::PatchNop(REL::Offset(0x661C13F), 0x5);
			REL::PatchNop(REL::Offset(0x661C12A), 0x5);

			_MESSAGE("Install NoPoisonConfirmMessage patch");
		}
	}
}
