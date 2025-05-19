// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TLoadScreen.h>
#include <TRelocation.h>

namespace Turpentine
{
	namespace Patches
	{
		namespace Impl
		{
			static void* APIENTRY sub_146966DD0() noexcept(true)
			{
				_MESSAGE("%p", _ReturnAddress());
				return nullptr;
			}

			static bool APIENTRY sub_1465FD750() noexcept(true)
			{
				_MESSAGE("%p", _ReturnAddress());
				return 1;
			}
			
		}
		
		void APIENTRY LoadScreen() noexcept(true)
		{
			// remove hints to load screen
			Turpentine::REL::Patch(REL::Offset(0x6966DD0), { 0x31, 0xC0, 0xC3, 0x90 });
			
			
			

			//Turpentine::REL::DetourJump(GlobalBase + 0x66D9FD0, (uintptr_t)&Impl::sub_146966DD0);
			
			//
			// remove big useless code
			//Turpentine::REL::Patch(GlobalBase + 0x65FD305, { 0x90, 0xE9 });
			// Idk menu something (remove)
			//Turpentine::REL::PatchNop(GlobalBase + 0x65FD400, 0xE);


			/*Turpentine::REL::PatchNop(GlobalBase + 0x66DB40F, 0x5);
			Turpentine::REL::PatchNop(GlobalBase + 0x66DB476, 0x5);
			
			Turpentine::REL::PatchNop(GlobalBase + 0x66DB57D, 0x5);
			Turpentine::REL::PatchNop(GlobalBase + 0x66DB595, 0x5);
			Turpentine::REL::PatchNop(GlobalBase + 0x66DB5B9, 0x5);
			Turpentine::REL::PatchNop(GlobalBase + 0x66DB5F8, 0x5);
			
			Turpentine::REL::Patch(GlobalBase + 0x66DB62C, { 0xEB });
			Turpentine::REL::Patch(GlobalBase + 0x66DB673, { 0xEB });

			Turpentine::REL::PatchNop(GlobalBase + 0x65FD830, 0x21);*/
			

			//Turpentine::REL::Patch(GlobalBase + 0x66DB26C, { 0x90, 0xE9 });
			
			//
			
			//Turpentine::REL::DetourJump(GlobalBase + 0x65FD750, (uintptr_t)&Impl::sub_1465FD750);
			

			//Turpentine::REL::PatchNop(GlobalBase + 0x65FC2B5, 5);
			

			// bik main menu
			//Turpentine::REL::Patch(GlobalBase + 0x65FC256, { 0xEB });
			

			//Turpentine::REL::Patch(GlobalBase + 0x66D9FD0, { 0x31, 0xC0, 0xC3, 0x90 });
			


			//Turpentine::REL::DetourJump(GlobalBase + 0x6593DE0, (uintptr_t)&Impl::sub_146966DD0);
			//Turpentine::REL::Patch(GlobalBase + 0x6603328, { 0x31, 0xC0, 0xC3, 0x90 });
			//Turpentine::REL::PatchNop(GlobalBase + 0x66500D8, 5);
			//Turpentine::REL::Patch(GlobalBase + 0x66500D8, { 0x90, 0x90, 0x90, 0x90, 0x90 });
			//Turpentine::REL::Patch(GlobalBase + 0x66500D8, { 0x90, 0x90, 0x90, 0x90, 0x90 });
			//
			
			//Turpentine::REL::DetourJump(GlobalBase + 0x6799C40, (uintptr_t)&Impl::sub_146966DD0);
			//Turpentine::REL::DetourJump(GlobalBase + 0x6798F30, (uintptr_t)&Impl::sub_146966DD0);
			
			//Turpentine::REL::DetourJump(GlobalBase + 0x67994B0, (uintptr_t)&Impl::sub_146966DD0);
			//Turpentine::REL::DetourJump(GlobalBase + 0x6966DD0, (uintptr_t)&Impl::sub_146966DD0);


			//Turpentine::REL::Patch(GlobalBase + 0x6966DD0, { 0x31, 0xC0, 0xC3, 0x90 });
		}
	}
}
