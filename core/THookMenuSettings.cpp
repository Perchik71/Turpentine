// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <THookMenuSettings.h>
#include <TRelocation.h>
#include <TModVersion.h>
#include <TUtils.h>

namespace Turpentine
{
	namespace Hooks
	{
		void APIENTRY MenuSettings() noexcept(true)
		{
			// Change App
			wchar_t* versionSubApp = new wchar_t[96];
			swprintf_s(versionSubApp, 50, L" " MOD_NAME " [OBSE64] (%d.%d.%d) by " MOD_AUTHOR,
				MOD_VERSION_MAJOR, MOD_VERSION_MINOR, MOD_VERSION_BETA);
			wchar_t* versionApp = new wchar_t[250];
			wcscpy_s(versionApp, 250, L"v%s(%s) CorrelationID:%s");
			wcscat_s(versionApp, 250, versionSubApp);
			delete[] versionSubApp;

			// Change App
			// Since the author of OBSE64 decided to intercept this too, it goes into conflict.
			//Turpentine::REL::Patch(REL::Offset(0x491B0EF), { 0xEB, 0x06, 0x90 });
			//Turpentine::REL::Patch(REL::Offset(0x493EBF5), { 0x8B });
			//Turpentine::REL::Patch(REL::Offset(0x7DB14E8), (uint8_t*)&versionApp, sizeof(uintptr_t));
		}
	}
}
