// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace Turpentine
{
	namespace REL
	{
		namespace Impl
		{
			class ScopeLock
			{
				bool _locked;
				uint32_t _old;
				uintptr_t _target, _size;
			public:
				ScopeLock(uintptr_t a_target, uintptr_t a_size);
				ScopeLock(void* a_target, uintptr_t a_size);
				virtual ~ScopeLock();

				[[nodiscard]] inline virtual bool HasUnlocked() const noexcept(true) { return _locked; }
			};
		}

		uintptr_t APIENTRY FindPattern(uintptr_t Address, uintptr_t MaxSize, const char* Mask) noexcept(true);
		vector<uintptr_t> APIENTRY FindPatterns(uintptr_t Address, uintptr_t MaxSize, const char* Mask) noexcept(true);
		void APIENTRY Patch(uintptr_t Target, uint8_t* Data, size_t Size) noexcept(true);
		void APIENTRY Patch(uintptr_t Target, initializer_list<uint8_t> Data) noexcept(true);
		void APIENTRY PatchNop(uintptr_t Target, size_t Size) noexcept(true);
		uintptr_t APIENTRY DetourJump(uintptr_t Target, uintptr_t Function) noexcept(true);
		uintptr_t APIENTRY DetourCall(uintptr_t Target, uintptr_t Function) noexcept(true);
		uintptr_t APIENTRY DetourVTable(uintptr_t Target, uintptr_t Function, uint32_t Index) noexcept(true);
		uintptr_t APIENTRY DetourIAT(uintptr_t TargetModule, const char* Import, const char* FunctionName, uintptr_t Function) noexcept(true);
		uintptr_t APIENTRY DetourIATDelayed(uintptr_t TargetModule, const char* Import, const char* FunctionName, uintptr_t Function) noexcept(true);

		[[nodiscard]] uintptr_t APIENTRY Offset(uint32_t rva) noexcept(true);
	}
}