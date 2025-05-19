// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <THookDataHandler.h>
#include <TRelocation.h>
#include <TUtils.h>

extern std::unordered_map<std::string, int> GlobalActivePlugins;

namespace Turpentine
{
	namespace Hooks
	{
		namespace Impl
		{
			static bool APIENTRY TESDataHandler_Hook_LoadPlugin(const void* self, __int64 file, char a3)
			{
				bool r = Utils::ThisCall<bool>(REL::Offset(0x664FD60), self, file, a3);
				
				if (file)
				{
					if (!r)
						_MESSAGE("[FAILED] Load plugin: %s", (char*)(file + 0x28));
					else
					{
						_MESSAGE("[SUCCEEDED] Load plugin: %s", (char*)(file + 0x28));
						GlobalActivePlugins.insert(std::make_pair<std::string, int>((char*)(file + 0x28), 0));
					}
				}

				return r;
			}
		}
		
		void APIENTRY DataHandler() noexcept(true)
		{
			Turpentine::REL::DetourCall(REL::Offset(0x664EE86), (uintptr_t)&Impl::TESDataHandler_Hook_LoadPlugin);
			Turpentine::REL::PatchNop(REL::Offset(0x66500C9), 5);
		}
	}
}
