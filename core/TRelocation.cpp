// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

//#include <sha256.h>
#include "TRelocation.h"
#include <detours/Detours.h>
#include <obse64_common/obse64_version.h>
#include <obse64_common/FileStream.h>

extern uintptr_t GlobalBase;

namespace Turpentine
{
	namespace REL
	{
		static IDDB* siddb = nullptr;

		IDDB::IDDB(const std::wstring& fname, uint32_t runtimeVersion) noexcept(true) :
			_runtimeVersion(runtimeVersion)
		{
			FileStream stream;
			if (stream.open(fname.c_str()))
			{
				_WIN32_FILE_ATTRIBUTE_DATA fd{};
				GetFileAttributesExW(fname.c_str(), GetFileExInfoStandard, &fd);

				ULARGE_INTEGER ul{};
				ul.HighPart = fd.nFileSizeHigh;
				ul.LowPart = fd.nFileSizeLow;

				if (ul.QuadPart > 0x60)
				{
					stream.seek(0x60);

					_count = (ul.QuadPart - 0x60) / sizeof(mapping_t);
					_id2offset = new mapping_t[_count];
					if (_id2offset) stream.read(_id2offset, ul.QuadPart - 0x60);
				}

				stream.close();
			}
		}

		IDDB::~IDDB()
		{
			if (_id2offset)
			{
				delete[] _id2offset;
				_id2offset = nullptr;
			}
		}

		const IDDB* IDDB::Create(const wchar_t* fname, uint32_t runtimeVersion) noexcept(true)
		{
			siddb = new IDDB(fname, runtimeVersion);	
			if (!siddb->_count)
			{
				delete siddb;
				siddb = nullptr;
			}
			return siddb;
		}

		void IDDB::Release() noexcept(true)
		{
			if (siddb)
			{
				delete siddb;
				siddb = nullptr;
			}
		}

		const IDDB* IDDB::GetSingleton() noexcept(true)
		{
			return siddb;
		}

		uintptr_t IDDB::ID2Offset(uintptr_t id) const noexcept(true)
		{
			return static_cast<uintptr_t>(_id2offset[id].offset);
		}

		uintptr_t ID(uintptr_t id) noexcept(true)
		{
			auto off = siddb ? siddb->ID2Offset(id) : 0;
			return !off ? 0 : Offset(off);
		}

		namespace Impl
		{
			ScopeLock::ScopeLock(uintptr_t Target, uintptr_t Size) :
				_locked(false), _old(0), _target(Target), _size(Size)
			{
				_locked = VirtualProtect(reinterpret_cast<void*>(Target), (SIZE_T)Size, PAGE_EXECUTE_READWRITE, (PDWORD)&_old);
			}

			ScopeLock::ScopeLock(void* Target, uintptr_t Size) :
				_locked(false), _old(0), _target((uintptr_t)Target), _size(Size)
			{
				_locked = VirtualProtect(Target, (SIZE_T)Size, PAGE_EXECUTE_READWRITE, (PDWORD)&_old);
			}

			ScopeLock::~ScopeLock()
			{
				if (_locked)
				{
					// Ignore if this fails, the memory was copied either way
					VirtualProtect(reinterpret_cast<void*>(_target), (SIZE_T)_size, _old, (PDWORD)&_old);
					FlushInstructionCache(GetCurrentProcess(), (LPVOID)_target, _size);
					_locked = false;
				}
			}
		}

		uintptr_t APIENTRY FindPattern(uintptr_t Address, uintptr_t MaxSize, const char* Mask) noexcept(true)
		{
			vector<pair<uint8_t, bool>> pattern;

			for (size_t i = 0; i < strlen(Mask);)
			{
				if (Mask[i] != '?')
				{
					pattern.emplace_back((uint8_t)strtoul(&Mask[i], nullptr, 16), false);
					i += 3;
				}
				else
				{
					pattern.emplace_back(0x00, true);
					i += 2;
				}
			}

			const uint8_t* dataStart = (uint8_t*)Address;
			const uint8_t* dataEnd = (uint8_t*)Address + MaxSize + 1;

			auto ret = search(dataStart, dataEnd, pattern.begin(), pattern.end(),
				[](uint8_t CurrentByte, pair<uint8_t, bool>& Pattern) {
					return Pattern.second || (CurrentByte == Pattern.first);
				});

			if (ret == dataEnd)
				return 0;

			return (uintptr_t)(distance(dataStart, ret) + Address);
		}

		vector<uintptr_t> APIENTRY FindPatterns(uintptr_t Address, uintptr_t MaxSize, const char* Mask) noexcept(true)
		{
			vector<uintptr_t> results;
			vector<pair<uint8_t, bool>> pattern;

			for (size_t i = 0; i < strlen(Mask);)
			{
				if (Mask[i] != '?')
				{
					pattern.emplace_back((uint8_t)strtoul(&Mask[i], nullptr, 16), false);
					i += 3;
				}
				else
				{
					pattern.emplace_back(0x00, true);
					i += 2;
				}
			}

			const uint8_t* dataStart = (uint8_t*)Address;
			const uint8_t* dataEnd = (uint8_t*)Address + MaxSize + 1;

			for (const uint8_t* i = dataStart;;)
			{
				auto ret = search(i, dataEnd, pattern.begin(), pattern.end(),
					[](uint8_t CurrentByte, pair<uint8_t, bool>& Pattern) {
						return Pattern.second || (CurrentByte == Pattern.first);
					});

				if (ret == dataEnd)
					break;

				uintptr_t addr = distance(dataStart, ret) + Address;
				results.push_back(addr);

				i = (uint8_t*)(addr + 1);
			}

			return results;
		}

		void APIENTRY Patch(uintptr_t Target, uint8_t* Data, size_t Size) noexcept(true)
		{
			if (!Target || !Data || !Size) return;
			Impl::ScopeLock Lock(Target, Size);
			if (Lock.HasUnlocked())
			{
				for (uintptr_t i = Target; i < (Target + Size); i++)
					*(volatile uint8_t*)i = *Data++;
			}
		}

		void APIENTRY Patch(uintptr_t Target, initializer_list<uint8_t> Data) noexcept(true)
		{
			if (!Target || !Data.size()) return;
			Impl::ScopeLock Lock(Target, Data.size());
			if (Lock.HasUnlocked())
			{
				uintptr_t i = Target;
				for (auto value : Data)
					*(volatile uint8_t*)i++ = value;
			}
		}

		void APIENTRY PatchNop(uintptr_t Target, size_t Size) noexcept(true)
		{
			if (!Target || !Size) return;
			Impl::ScopeLock Lock(Target, Size);
			if (Lock.HasUnlocked())
				memset((LPVOID)Target, 0x90, Size);
		}

		uintptr_t APIENTRY DetourJump(uintptr_t Target, uintptr_t Function) noexcept(true)
		{
			if (!Target || !Function) return 0;
			return Detours::X64::DetourFunction(Target, Function, Detours::X64Option::USE_REL32_JUMP);
		}

		uintptr_t APIENTRY DetourCall(uintptr_t Target, uintptr_t Function) noexcept(true)
		{
			if (!Target || !Function) return 0;
			return (uintptr_t)Detours::X64::DetourFunction(Target, Function, Detours::X64Option::USE_REL32_CALL);
		}

		uintptr_t APIENTRY DetourVTable(uintptr_t Target, uintptr_t Function, uint32_t Index) noexcept(true)
		{
			if (!Target || !Function) return 0;
			return (uintptr_t)Detours::X64::DetourVTable(Target, Function, Index);
		}

		uintptr_t APIENTRY DetourIAT(uintptr_t TargetModule, const char* Import, const char* FunctionName, uintptr_t Function) noexcept(true)
		{
			if (!TargetModule || !Function || !Import || !FunctionName) return 0;
			return (uintptr_t)Detours::IATHook(TargetModule, Import, FunctionName, Function);
		}

		uintptr_t APIENTRY DetourIATDelayed(uintptr_t TargetModule, const char* Import, const char* FunctionName, uintptr_t Function) noexcept(true)
		{
			if (!TargetModule || !Function || !Import || !FunctionName) return 0;
			return (uintptr_t)Detours::IATDelayedHook(TargetModule, Import, FunctionName, Function);
		}

		uintptr_t APIENTRY Offset(uint32_t rva) noexcept(true)
		{
			return (uintptr_t)(GlobalBase + rva);
		}
	}
}
