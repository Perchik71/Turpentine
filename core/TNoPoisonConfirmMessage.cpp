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
		namespace Impl
		{
			static void* APIENTRY sub_146592440()
			{
				return Utils::FastCall<void*>(REL::Offset(0x662B070));
			}
		}

		void APIENTRY NoPoisonConfirmMessage() noexcept(true)
		{
			REL::Patch(REL::Offset(0x661C2C8), { 0x31, 0xC0, 0x90, 0x90, 0x90, 0x90, 0x90 });
			REL::Patch(REL::Offset(0x661C2DD), { 0x00 });
			REL::Patch(REL::Offset(0x662B0B1), { 0x00 });
			REL::Patch(REL::Offset(0x661C2D4), { 0x45, 0x31, 0xC9, 0x90, 0x90, 0x90, 0x90 });
			REL::DetourCall(REL::Offset(0x661C2EC), (uintptr_t)&Impl::sub_146592440);	
			REL::DetourCall(REL::Offset(0x661C395), (uintptr_t)&Impl::sub_146592440);
			REL::PatchNop(REL::Offset(0x662B0B2), 0x6);

			_MESSAGE("Install NoPoisonConfirmMessage patch");
		}
	}
}
