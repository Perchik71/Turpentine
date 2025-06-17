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
		uintptr_t sub_UnknownSub_6770550 = 0;
		uintptr_t sub_UnknownSub_6770550_old = 0;

		namespace Impl
		{
			static void* APIENTRY GameApplyTradeSpellByPlayer()
			{
				return Utils::FastCall<void*>(sub_NoTradeSpellConfirmMessage_1);
			}

			/*static std::uint32_t APIENTRY GameCheckPtrSpellByPlayer(void** rcx)
			{
				typedef std::uint32_t(*Sub30)(void**);

				__try
				{
					auto vtable = *rcx;
					return ((Sub30)((std::uintptr_t)vtable + 0x30))(rcx);
				}
				__except (1)
				{
					return 0;
				}
			}*/

			static void* APIENTRY UnknownSub_6770550(void* s)
			{
				//
				// It looks like the function is responsible for filling out the list for sale
				// If do not check obj_2 + 0x10, then the last item will appear, but after buying it, 
				// the function will issue a crash since there is nothing further down the list.
				//

				//if (!s) return nullptr;
				//auto obj_1 = *((void**)((std::uintptr_t)s + 0x58));
				//if (!obj_1) return nullptr;
				//auto obj_2 = *((void**)((std::uintptr_t)obj_1 + 0x70));
				//if (!obj_2) return nullptr;
				//auto obj_3 = *((void**)((std::uintptr_t)obj_2 + 0x10));
				//if (!obj_3) return nullptr;
				//return Utils::FastCall<void*>(sub_UnknownSub_6770550_old, s);

				__try
				{
					return Utils::FastCall<void*>(sub_UnknownSub_6770550_old, s);
				}
				__except (1)
				{
					return nullptr;
				}
			}
		}

		void APIENTRY NoTradeSpellConfirmMessage() noexcept(true)
		{
			sub_NoTradeSpellConfirmMessage_0 = REL::ID(411431);	// 67BFF30
			sub_NoTradeSpellConfirmMessage_1 = REL::ID(411430);	// 67BFD80
			sub_UnknownSub_6770550 = REL::ID(410718);			// 6770550
				
			REL::Patch(sub_NoTradeSpellConfirmMessage_0 + 0x3D3, { 0x00 });
			REL::Patch(sub_NoTradeSpellConfirmMessage_1 + 0x59, { 0x00 });
			REL::Patch(sub_NoTradeSpellConfirmMessage_0 + 0x3CA, { 0x31, 0xD2, 0x90, 0x90, 0x90, 0x90, 0x90 });
			REL::DetourCall(sub_NoTradeSpellConfirmMessage_0 + 0x3E3, (uintptr_t)&Impl::GameApplyTradeSpellByPlayer);
			REL::Patch(sub_NoTradeSpellConfirmMessage_0 + 0x3E8, { 0xE9, 0x59, 0xFD, 0xFF, 0xFF });
			// Crash when buy last spell
			// Same
			//REL::DetourCall(sub_NoTradeSpellConfirmMessage_0 + 0x250, (uintptr_t)&Impl::GameCheckPtrSpellByPlayer);
			// Same
			//REL::Patch(sub_NoTradeSpellConfirmMessage_0 + 0x4B2, { 0x90, 0xE9 });
			// Fix crash when buy last spell
			sub_UnknownSub_6770550_old = REL::DetourJump(sub_UnknownSub_6770550, (uintptr_t)&Impl::UnknownSub_6770550);

			REL::PatchNop(sub_NoTradeSpellConfirmMessage_1 + 0x5A, 0x6);
			
			_MESSAGE("Install NoTradeSpellConfirmMessage patch");
		}
	}
}
