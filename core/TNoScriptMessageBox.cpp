// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TNoScriptMessageBox.h>
#include <TRelocation.h>

namespace Turpentine
{
	namespace Patches
	{
		void APIENTRY NoScriptMessageBox() noexcept(true)
		{
			REL::Patch(REL::Offset(0x6A0AD2D), { 0x31, 0xC0, 0xFE, 0xC0, 0xE9, 0x02, 0x01, 0x00, 0x00, 0x90 });
			_MESSAGE("Install NoScriptMessageBox patch");
		}
	}
}
