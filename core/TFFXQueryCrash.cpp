// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TFFXQueryCrash.h>
#include <TRelocation.h>
#include <xbyak/xbyak/xbyak.h>

namespace Turpentine
{
	namespace Fixes
	{
		void APIENTRY FFXQueryCrash() noexcept(true)
		{
			// In 1_511_102:
			// The function has completely changed, but there is still no check ptr,
			// perhaps the developers fixed the crash.

			class ffxQueryHook : public Xbyak::CodeGenerator
			{
			public:
				ffxQueryHook(uintptr_t a1, uintptr_t a2) : Xbyak::CodeGenerator()
				{
					mov(rdx, ptr[rdi + 0x10]);
					test(rdx, rdx);
					jz(".jmp_table");
					xor_(ebx, ebx);				// added 1_511_102
					lea(r8, ptr[rax + 0x30]);
					jmp(ptr[rip]);
					dq(a1);
					L(".jmp_table");
					jmp(ptr[rip]);
					dq(a2);
				}
			} 
			static ffxQueryInstance(REL::Offset(0x45906D5), REL::Offset(0x45907F6));

			REL::DetourJump(REL::Offset(0x45906CB), (uintptr_t)ffxQueryInstance.getCode());

			_MESSAGE("Install FFXQueryCrash fixes");
		}
	}
}
